#include "pch.h"
#include "GroupEffect.h"

#include "Utils.h"
#include "MathUtils.h"
#include "FileUtils.h"
#include "MeshEffect.h"
#include "MeshEffectData.h"
#include "StructuredBuffer.h"

bool Compare_RenderPriority(weak_ptr<GameObject> pSrc, weak_ptr<GameObject> pDest)
{
    return pSrc.lock()->Get_MeshEffect()->Get_RenderPriority() < pDest.lock()->Get_MeshEffect()->Get_RenderPriority();
}

GroupEffect::GroupEffect()
    : Component(COMPONENT_TYPE::GroupEffect)
{
}

GroupEffect::~GroupEffect()
{

}

HRESULT GroupEffect::Init()
{
    // For. Cool Time of each Member effects
    m_vCreateCoolTime.resize(m_vMemberEffectData.size());
    fill(m_vCreateCoolTime.begin(), m_vCreateCoolTime.end(), 0.f);

    // For. RenderingGroup
	map<wstring, vector<shared_ptr<MeshEffectData>>> renderingGroup;


    for (_uint i = 0; i < static_cast<_uint>(m_vMemberEffectData.size()); ++i)
    {
		wstring wstrMeshEffectDataKey = m_vMemberEffectData[i].wstrEffectTag;
		Utils::DetachExt(wstrMeshEffectDataKey);
		shared_ptr<MeshEffectData> meshEffectData = RESOURCES.Get<MeshEffectData>(wstrMeshEffectDataKey);
        if (meshEffectData->Get_Desc().bIsFDistortion)
            m_bDistortionGroup = true;

        renderingGroup[Utils::ToWString(meshEffectData->Get_Desc().strTag)].push_back(meshEffectData);

        _bool bAlreadyCreated = false;
        for (auto& pair : m_RenderGroup)
        {
            if (pair.first == Utils::ToWString(meshEffectData->Get_Desc().strTag))
                bAlreadyCreated = true;
        }
        if (bAlreadyCreated == false)
        {
            m_RenderGroup.push_back(make_pair(Utils::ToWString(meshEffectData->Get_Desc().strTag), vector<shared_ptr<GameObject>>()));
        }

		/* if(meshEffectData->Get_Desc().iMeshCnt ==1 && meshEffectData->Get_Desc().fCreateInterval == 0)
			 continue;

		 if(meshEffectData->Get_Desc().fCreateInterval == 0)
			 m_RenderParamBuffer[i] = make_shared<StructuredBuffer>(nullptr, static_cast<_uint>(sizeof RenderParams), meshEffectData->Get_Desc().iMeshCnt);
		 else
		 {
			 _int count =_int( meshEffectData->Get_Desc().fDuration / meshEffectData->Get_Desc().fCreateInterval * meshEffectData->Get_Desc().iMeshCnt);

			 m_RenderParamBuffer[i] = make_shared<StructuredBuffer>(nullptr, static_cast<_uint>(sizeof RenderParams), count);
		 }*/
         
    }

    for (auto& pair : renderingGroup)
    {
        if(pair.second.empty())
            continue;

        auto meshEffectData = pair.second.front();

		if (pair.second.size() == 1 && meshEffectData->Get_Desc().iMeshCnt == 1 && meshEffectData->Get_Desc().fCreateInterval == 0)
			continue;

        _uint iEffectDataCount = _uint(pair.second.size());

        if (meshEffectData->Get_Desc().fCreateInterval == 0.f)
        {
            m_RenderParamBuffer[pair.first] = make_shared<StructuredBuffer>(nullptr, static_cast<_uint>(sizeof RenderParams), meshEffectData->Get_Desc().iMeshCnt * iEffectDataCount);
        }
        else
        {
            _int count = _int(meshEffectData->Get_Desc().fDuration / meshEffectData->Get_Desc().fCreateInterval * meshEffectData->Get_Desc().iMeshCnt * iEffectDataCount);
            m_RenderParamBuffer[pair.first] = make_shared<StructuredBuffer>(nullptr, static_cast<_uint>(sizeof RenderParams), count);
        }

    }

    // For. Round Axis
    _int iIndex = 0;
    for (auto& iter : m_vMemberEffectData)
    {
        wstring wstrMeshEffectDataKey = iter.wstrEffectTag;
        Utils::DetachExt(wstrMeshEffectDataKey);
        shared_ptr<MeshEffectData> meshEffectData = RESOURCES.Get<MeshEffectData>(wstrMeshEffectDataKey);
        MeshEffectData::Transform_Desc tTransform_Desc = meshEffectData->Get_TransformDesc();

        if (2 == tTransform_Desc.iRoundAxisOption)
        {
            pair<_int, _float3> RoundAxis;
            RoundAxis.first = iIndex;
            RoundAxis.second = _float3(
                MathUtils::Get_RandomFloat(tTransform_Desc.vRoundAxis_Min.x, tTransform_Desc.vRoundAxis_Max.x),
                MathUtils::Get_RandomFloat(tTransform_Desc.vRoundAxis_Min.y, tTransform_Desc.vRoundAxis_Max.y),
                MathUtils::Get_RandomFloat(tTransform_Desc.vRoundAxis_Min.z, tTransform_Desc.vRoundAxis_Max.z)
            );
            m_RoundAxis.push_back(RoundAxis);
        }
        iIndex++;
    }

    return S_OK;
}

void GroupEffect::Tick()
{
	if (!m_bTickOn)
		return;

    _int iIndex = 0;
    for (auto& iter : m_vMemberEffectData)
    {
        if (iter.fCreateTime < m_fCurrAge)
        {
            wstring wstrMeshEffectDataKey = iter.wstrEffectTag;
            Utils::DetachExt(wstrMeshEffectDataKey);
            shared_ptr<MeshEffectData> meshEffectData = RESOURCES.Get<MeshEffectData>(wstrMeshEffectDataKey);
            MeshEffectData::DESC tDesc = meshEffectData->Get_Desc();

            // If. Play only once
            if (0.f == tDesc.fCreateInterval)
            {
                if (!iter.bIsActive)
                {
                    Create_MeshEffect(iIndex);
                    iter.bIsActive = true;
                }
            }

            // If. Play every CreateInterval in duration 
            else if (tDesc.fCreateInterval < m_vCreateCoolTime[iIndex])
            {
                if (tDesc.fDuration + iter.fCreateTime > m_fCurrAge)
                {
                    Create_MeshEffect(iIndex);
                    m_vCreateCoolTime[iIndex] = 0.f;
                }
                else
                    iter.bIsActive = true;
            }
        }
        iIndex++;
    }

    for (auto& meshEffect : m_lMemberEffects)
    {
        if (!meshEffect.expired())
            meshEffect.lock()->Get_MeshEffect()->MeshEffect_Tick();
    }


}

void GroupEffect::Final_Tick()
{
	_float deltaTime = fDT;
	if (!Get_Owner()->Is_TimeSlowed())
		deltaTime = fABT;

    if (!m_bTickOn)
        return;

    m_fCurrAge += deltaTime;
    for (auto iter = m_vCreateCoolTime.begin(); iter != m_vCreateCoolTime.end(); iter++)
        *iter += deltaTime;

    _bool bIsAllActive = true;

    // For. Check all effect finished
    for (auto& iter : m_vMemberEffectData)
    {
        if (!iter.bIsActive)
            bIsAllActive = false;
    }

    if (bIsAllActive)
    {
        _bool bIsAllNullptr = true;
        for (auto& iter : m_lMemberEffects)
        {
            if (!iter.expired())
                bIsAllNullptr = false;
        }

        if (bIsAllNullptr)
        {
			EVENTMGR.Delete_Object(Get_Owner());
            return;
        }
    }

    // For. Update MeshEffect Translation 
    for (auto& iter : m_lMemberEffects)
    {
        if (!iter.expired())
        {
			if (iter.lock()->Get_MeshEffect()->Get_IsFollowGroup_LooKSameDir())
			{
                _float4x4 matSetting = iter.lock()->Get_MeshEffect()->Get_LocalMatrix() * iter.lock()->Get_MeshEffect()->Get_InGroupMatrix() * Get_Transform()->Get_WorldMatrix();
                iter.lock()->Get_Transform()->Set_WorldMat(matSetting);
                iter.lock()->Get_MeshEffect()->BillBoard();
            }
            else
            {
				_float4x4 matSetting = iter.lock()->Get_MeshEffect()->Get_LocalMatrix() * iter.lock()->Get_MeshEffect()->Get_InGroupMatrix() * iter.lock()->Get_MeshEffect()->Get_InitGroupMatrix();
				iter.lock()->Get_Transform()->Set_WorldMat(matSetting);
				iter.lock()->Get_MeshEffect()->BillBoard();
            }
        }
    }

    for (auto& renderGroup : m_RenderGroup)
        renderGroup.second.clear();


	for (auto& meshEffect : m_lMemberEffects)
	{
        if (!meshEffect.expired())
        {
			meshEffect.lock()->Get_MeshEffect()->MeshEffect_Final_Tick();

            if(!meshEffect.lock()->Get_MeshEffect()->Is_Finished())
				for (auto& renderGroup : m_RenderGroup)
				{
					if (renderGroup.first == Utils::ToWString(meshEffect.lock()->Get_MeshEffect()->Get_Desc().strTag))
						renderGroup.second.push_back(meshEffect.lock());
				}
           

            //const wstring instanceID = Utils::ToWString(meshEffect.lock()->Get_MeshEffect()->Get_Desc().strTag);
            //m_RenderGroup[instanceID].push_back(meshEffect.lock());
        }
	}
}

void GroupEffect::Render()
{
    if (!m_bRenderOn)
        return;
    vector<shared_ptr<GameObject>> sortingEffects;
    sortingEffects.reserve(100);

    for (auto& pair : m_RenderGroup)
    {
        vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 0)
			continue;
		if (vec[0]->Get_MeshEffect()->Get_Desc().bIsFDistortion)
			continue;
		if (vec[0]->Get_MeshEffect()->Get_Desc().bIsSSD)
			continue;
        if (vec.size() == 1 ||/*TODO Sorting老 版快*/ vec.front()->Get_MeshEffect()->Get_Desc().bIsFDistortion)
        {

        }

    }

    INSTANCING.Clear_Data();



    for (auto& pair : m_RenderGroup)
    {
        vector<shared_ptr<GameObject>>& vec = pair.second;
        if(vec.size() == 0)
            continue;
        if(vec[0]->Get_MeshEffect()->Get_Desc().bIsFDistortion)
            continue;
        if(vec[0]->Get_MeshEffect()->Get_Desc().bIsSSD)
            continue;



		if (vec.size() == 1 ||/*TODO Sorting老 版快*/ vec.front()->Get_MeshEffect()->Get_Desc().bIsFDistortion)
		{
			vec.front()->Get_MeshEffect()->Render();
		}
		else
		{
            const InstanceID instanceId = make_pair(_ulong(pair.first.c_str()), _ulong(pair.first.c_str()));
            vector<RenderParams> paramInfo;
            paramInfo.reserve(m_RenderParamBuffer[pair.first]->Get_InputCount());

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data{};
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				paramInfo.push_back( gameobject->Get_MeshEffect()->Get_RenderParamDesc());
                INSTANCING.Add_Data(instanceId, data);
			}
            m_RenderParamBuffer[pair.first]->Copy_ToInput(paramInfo.data());

            shared_ptr<InstancingBuffer>& buffer = INSTANCING.Get_Buffer(instanceId);
			vec[0]->Get_MeshEffect()->Render_Instancing(buffer, m_RenderParamBuffer[pair.first]);
		}
    }
}

void GroupEffect::Render_Decal()
{
	if (!m_bRenderOn)
		return;
	INSTANCING.Clear_Data();
	for (auto& pair : m_RenderGroup)
	{
		vector<shared_ptr<GameObject>>& vec = pair.second;
		if (vec.size() == 0)
			continue;

		if (!vec[0]->Get_MeshEffect()->Get_Desc().bIsSSD)
			continue;

		if (vec.size() == 1)
		{
			vec.front()->Get_MeshEffect()->Render();
		}
		else
		{
			const InstanceID instanceId = make_pair(_ulong(pair.first.c_str()), _ulong(pair.first.c_str()));
			vector<RenderParams> paramInfo;
			paramInfo.reserve(m_RenderParamBuffer[pair.first]->Get_InputCount());

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data{};
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				paramInfo.push_back(gameobject->Get_MeshEffect()->Get_RenderParamDesc());
				INSTANCING.Add_Data(instanceId, data);
			}
			m_RenderParamBuffer[pair.first]->Copy_ToInput(paramInfo.data());

			shared_ptr<InstancingBuffer>& buffer = INSTANCING.Get_Buffer(instanceId);
			vec[0]->Get_MeshEffect()->Render_Instancing(buffer, m_RenderParamBuffer[pair.first]);
		}
	}
}

void GroupEffect::Render_Distortion()
{
	if (!m_bRenderOn)
		return;

	INSTANCING.Clear_Data();
	for (auto& pair : m_RenderGroup)
	{
		vector<shared_ptr<GameObject>>& vec = pair.second;
		if (vec.size() == 0)
			continue;

		if (!vec[0]->Get_MeshEffect()->Get_Desc().bIsFDistortion)
			continue;


		if (vec.size() == 1)
		{
			vec.front()->Get_MeshEffect()->Render();
		}
		else
		{
			const InstanceID instanceId = make_pair(_ulong(pair.first.c_str()), _ulong(pair.first.c_str()));
			vector<RenderParams> paramInfo;
			paramInfo.reserve(m_RenderParamBuffer[pair.first]->Get_InputCount());

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data{};
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				paramInfo.push_back(gameobject->Get_MeshEffect()->Get_RenderParamDesc());
				INSTANCING.Add_Data(instanceId, data);
			}
			m_RenderParamBuffer[pair.first]->Copy_ToInput(paramInfo.data());

			shared_ptr<InstancingBuffer>& buffer = INSTANCING.Get_Buffer(instanceId);
			vec[0]->Get_MeshEffect()->Render_Instancing(buffer, m_RenderParamBuffer[pair.first]);
		}
	}

}

void GroupEffect::Save(const wstring& path)
{
    shared_ptr<FileUtils> file = make_shared<FileUtils>();
    file->Open(path, FileMode::Write);

    /* Tag */
    file->Write<string>(Utils::ToString(m_wstrTag));
    file->Write<_int>((_int)m_vMemberEffectData.size());

    /* Member Effect Data */
    for (auto& iter : m_vMemberEffectData)
    {
        file->Write<string>(Utils::ToString(iter.wstrEffectTag));
        file->Write<_int>(iter.eType);
        file->Write<_float>(iter.fCreateTime);

        file->Write<_bool>(false);

        file->Write<_float3>(iter.vPivot_Pos);
        file->Write<_float3>(iter.vPivot_Scale);
        file->Write<_float3>(iter.vPivot_Rotation);
    }
}

void GroupEffect::DeleteMember(_int iIndex)
{
    m_vMemberEffectData.erase(m_vMemberEffectData.begin() + iIndex);

    //_int iIndex = 0;
    //for (auto& iter : m_vMemberEffectData)
    //{
    //    if (iter.wstrEffectTag == wstrTag)
    //        m_vMemberEffectData.erase(m_vMemberEffectData.begin() + iIndex);
    //    iIndex++;
    //}
}

void GroupEffect::FreeLoopMember()
{
    for (auto& iter : m_lMemberEffects)
    {
        if (!iter.expired())
        {
            if (iter.lock()->Get_MeshEffect()->Get_IsLoop())
                iter.lock()->Get_MeshEffect()->Set_Loop(false);
        }
    }
}

void GroupEffect::Kill_All()
{
    for (auto& iter : m_lMemberEffects)
        iter.reset();
}

void GroupEffect::Set_MemberEffectMaterials()
{
    for (auto& iter : m_vMemberEffectData)
    {
        wstring wstrMeshEffectDataKey = iter.wstrEffectTag;
        Utils::DetachExt(wstrMeshEffectDataKey);
        shared_ptr<MeshEffectData> meshEffectData = RESOURCES.Get<MeshEffectData>(wstrMeshEffectDataKey);
        MeshEffectData::DESC tDesc = meshEffectData->Get_Desc();

        if (!RESOURCES.Get<Material>(Utils::ToWString(tDesc.strTag)))
        {
            shared_ptr<Material> material = make_shared<Material>();

            // Shader
			shared_ptr<Shader> shader;
			if (tDesc.bIsFDistortion)
				shader = RESOURCES.Get<Shader>(L"Shader_Distortion.fx");
			else
				shader = RESOURCES.Get<Shader>(L"Shader_Effect2.fx");
            material->Set_Shader(shader);

            // Texture 
            wstring wstrKey = Utils::ToWString(tDesc.strTexture_Op1);
            wstring wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
            if (TEXT("None") != wstrKey)
                material->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE7);

            wstrKey = Utils::ToWString(tDesc.strTexture_Op2);
            wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
            if (TEXT("None") != wstrKey)
                material->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE8);

            wstrKey = Utils::ToWString(tDesc.strTexture_Op3);
            wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
            if (TEXT("None") != wstrKey)
                material->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE9);

            wstrKey = Utils::ToWString(tDesc.strTexture_Blend);
            wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
            if (TEXT("None") != wstrKey)
                material->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE10);

            wstrKey = Utils::ToWString(tDesc.strOverlayTexture);
            wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
            if (TEXT("None") != wstrKey)
                material->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE11);

            wstrKey = Utils::ToWString(tDesc.strDissolveTexture);
            wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
            if (TEXT("None") != wstrKey)
                material->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::DISSOLVE);

            wstrKey = Utils::ToWString(tDesc.strDistortionTexture);
            wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
            if (TEXT("None") != wstrKey)
                material->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::DISTORTION);

            wstrKey = Utils::ToWString(tDesc.strNormalTexture);
            wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
            if (TEXT("None") != wstrKey)
                material->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::NORMAL);

            RESOURCES.Add<Material>(Utils::ToWString(tDesc.strTag), material, true);
        }
    }
}

void GroupEffect::Create_MeshEffect(_int iIndex)
{
    // For.  Mesh Effect  
    const auto& iter = m_vMemberEffectData[iIndex];
    wstring wstrMeshEffectDataKey = iter.wstrEffectTag;
    Utils::DetachExt(wstrMeshEffectDataKey);
    shared_ptr<MeshEffectData> meshEffectData = RESOURCES.Get<MeshEffectData>(wstrMeshEffectDataKey);
    MeshEffectData::DESC tDesc = meshEffectData->Get_Desc();
    MeshEffectData::Transform_Desc tTransform_Desc = meshEffectData->Get_TransformDesc();

    // Check round axis
    if (2 == tTransform_Desc.iRoundAxisOption)
    {
        for (auto& iter : m_RoundAxis)
        {
            if (iter.first == iIndex)
            {
                tTransform_Desc.vRoundAxis_Min = iter.second;
                tTransform_Desc.vRoundAxis_Max = iter.second;
            }
        }
    }

    for (_int i = 0; i < tDesc.iMeshCnt; i++)
    {
        shared_ptr<GameObject> EffectObj = make_shared<GameObject>(); 
        string strTag = tDesc.strTag;
        strTag = strTag + to_string(iIndex) + "_" + to_string(i) + "_";
        EffectObj->Set_Name(Utils::ToWString(strTag));
        EffectObj->Set_TimeSlowed(Get_Owner()->Is_TimeSlowed());
        // For. Transform 
        EffectObj->GetOrAddTransform();

        // For. Shader 
        shared_ptr<Shader> shader;
        if(tDesc.bIsFDistortion)
            shader = RESOURCES.Get<Shader>(L"Shader_Distortion.fx");
        else
            shader = RESOURCES.Get<Shader>(L"Shader_Effect2.fx");

        shared_ptr<MeshEffect> meshEffect = make_shared<MeshEffect>(shader);
        EffectObj->Add_Component(meshEffect);
        EffectObj->Get_MeshEffect()->Init(&tDesc);
        EffectObj->Get_MeshEffect()->Set_TransformDesc(&tTransform_Desc);
        EffectObj->Get_MeshEffect()->InitialTransform(Get_Transform()->Get_WorldMatrix(), iter.vPivot_Pos, iter.vPivot_Scale, iter.vPivot_Rotation);
        EffectObj->Get_MeshEffect()->Set_RenderPriority(iIndex);

        // For. Add Script 
        if (nullptr != m_pScript)
            EffectObj->Add_Component(m_pScript->Copy_Script()); 

        // For. Add to vector 
        m_lMemberEffects.push_back(EffectObj);

        // For. Sort Vector by RenderPriority 
        OrganizeMemberEffectList();
         
        m_lMemberEffects.sort(Compare_RenderPriority);

        // For. Add to scene
        EVENTMGR.Create_Object(EffectObj);
    }
}

void GroupEffect::Create_Particle(_int iIndex)
{

}

void GroupEffect::OrganizeMemberEffectList()
{
    for (auto iter = m_lMemberEffects.begin(); iter != m_lMemberEffects.end();) 
    {
        if (iter->expired())
            iter = m_lMemberEffects.erase(iter);
        else 
            iter++;
    }   
}
