#include "pch.h"
#include "GroupEffect.h"

#include "Utils.h"
#include "FileUtils.h"
#include "MeshEffect.h"
#include "MeshEffectData.h"

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
        for (auto& iter : m_vMemberEffects)
        {
            if (nullptr == iter)
                bIsAllNullptr = false;
        }

        if (bIsAllNullptr)
        {
            CUR_SCENE->Remove_GameObject(Get_Owner());
            return;
        }
    }

    // For. Update MeshEffect Translation 
    //_int iIndex = 0;
    //for (auto& iter : m_vMemberEffects)
    //{
    //    if (nullptr != iter)
    //    {
    //        iter->Get_MeshEffect()->Translate();
    //        iter->Get_MeshEffect()->Scaling();
    //        _float4x4 mLocalWorldMatrix = iter->Get_Transform()->Get_WorldMatrix();
    //        _float4x4 mInGroupWorldMatrix = iter->Get_MeshEffect()->Get_InGroupMatrix();
    //        
    //        if (iter->Get_MeshEffect()->Get_IsFollowGroup())
    //            iter->Get_Transform()->Set_WorldMat(mLocalWorldMatrix * mInGroupWorldMatrix * Get_Transform()->Get_WorldMatrix());
    //        else 
    //            iter->Get_Transform()->Set_WorldMat(mLocalWorldMatrix * mInGroupWorldMatrix * m_mInitWorldMatrix);
    //
    //        iter->Get_MeshEffect()->Turn(); // cause of billbord
    //    }
    //    iIndex++;
    //}

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
    for (auto& iter : m_vMemberEffects)
    {
        if (iter->Get_MeshEffect()->Get_IsLoop())
            iter->Get_MeshEffect()->Set_Loop(false);
    }
}

void GroupEffect::Kill_All()
{
    for (auto& iter : m_vMemberEffects)
    {
        iter.reset();
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
        string strTag = tDesc.pszTag;
        strTag += to_string(iIndex) + "_" + to_string(i) + "_";
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

        // For. Add to vector 
        m_vMemberEffects.push_back(EffectObj);

        // For. Add to scene
        CUR_SCENE->Add_GameObject(EffectObj);
    }
}

void GroupEffect::Create_Particle(_int iIndex)
{

}