#include "pch.h"
#include "GroupEffect.h"

#include "Utils.h"
#include "FileUtils.h"
#include "MeshEffect.h"
#include "MeshEffectData.h"

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

void GroupEffect::Init(void* pArg)
{

}

void GroupEffect::Tick()
{
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
            else if (tDesc.fCreateInterval < m_fTimeAcc_CreatCoolTime)
            {
                if (tDesc.fDuration + iter.fCreateTime > m_fCurrAge)
                {
                    Create_MeshEffect(iIndex);
                    m_fTimeAcc_CreatCoolTime = 0.f;
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
    m_fCurrAge += fDT;
    m_fTimeAcc_CreatCoolTime += fDT;

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
			if (iter.lock()->Get_MeshEffect()->Get_IsFollowGroup_OnlyTranslate() ||
				iter.lock()->Get_MeshEffect()->Get_IsFollowGroup_LooKSameDir())
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

	for (auto& meshEffect : m_lMemberEffects)
	{
		if (!meshEffect.expired())
			meshEffect.lock()->Get_MeshEffect()->MeshEffect_Final_Tick();
	}
}

void GroupEffect::Render()
{
    for (auto& iter : m_lMemberEffects)
    {
        if(!iter.expired())
            iter.lock()->Get_MeshEffect()->Render();
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

void GroupEffect::DeleteMember(const wstring& wstrTag)
{
    _int iIndex = 0;

    for (auto& iter : m_vMemberEffectData)
    {
        if (iter.wstrEffectTag == wstrTag)
            m_vMemberEffectData.erase(m_vMemberEffectData.begin() + iIndex);
        iIndex++;
    }
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
            shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Effect2.fx");
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
    auto iter = m_vMemberEffectData[iIndex];
    wstring wstrMeshEffectDataKey = iter.wstrEffectTag;
    Utils::DetachExt(wstrMeshEffectDataKey);
    shared_ptr<MeshEffectData> meshEffectData = RESOURCES.Get<MeshEffectData>(wstrMeshEffectDataKey);
    MeshEffectData::DESC tDesc = meshEffectData->Get_Desc();
    MeshEffectData::Transform_Desc tTransform_Desc = meshEffectData->Get_TransformDesc();

    for (_int i = 0; i < tDesc.iMeshCnt; i++)
    {
        shared_ptr<GameObject> EffectObj = make_shared<GameObject>(); 
        string strTag = tDesc.strTag;
        strTag = strTag + to_string(iIndex) + "_" + to_string(i) + "_";
        EffectObj->Set_Name(Utils::ToWString(strTag));

        // For. Transform 
        EffectObj->GetOrAddTransform();
       
        Quaternion qRotation = Quaternion::CreateFromYawPitchRoll(iter.vPivot_Rotation.y, iter.vPivot_Rotation.x, iter.vPivot_Rotation.z);
        qRotation.Normalize();

        // For. Shader 
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Effect2.fx");
        shared_ptr<MeshEffect> meshEffect = make_shared<MeshEffect>(shader);
        EffectObj->Add_Component(meshEffect);
        EffectObj->Get_MeshEffect()->Init(&tDesc);
        EffectObj->Get_MeshEffect()->Set_TransformDesc(&tTransform_Desc);
        EffectObj->Get_MeshEffect()->InitialTransform(Get_Transform()->Get_WorldMatrix(), iter.vPivot_Pos, iter.vPivot_Scale, iter.vPivot_Rotation);
        EffectObj->Get_MeshEffect()->Set_RenderPriority(iIndex);

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
    for (auto iter = m_lMemberEffects.begin(); iter != m_lMemberEffects.end();) {
        if (iter->expired())
            iter = m_lMemberEffects.erase(iter);
        else 
            iter++;
    }   
}
