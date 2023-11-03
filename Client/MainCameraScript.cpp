
#include "pch.h"
#include "Camera.h"
#include "BaseCollider.h"
#include "CharacterController.h"
#include "Frustum.h"
#include "MainCameraScript.h"
#include "FSM.h"
#include "MathUtils.h"
#include "RigidBody.h"

MainCameraScript::MainCameraScript(shared_ptr<GameObject> pPlayer)
{
    m_pPlayer = pPlayer;
}

MainCameraScript::~MainCameraScript()
{
}

HRESULT MainCameraScript::Init()
{
    _float3 initDir = _float3(0.f);
    initDir = m_pPlayer.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
    initDir.Normalize();
    m_vOffset = initDir;
    Get_Transform()->Set_State(Transform_State::POS, m_pPlayer.lock()->Get_Transform()->Get_State(Transform_State::POS) + m_vOffset);


    return S_OK;
}

void MainCameraScript::Tick()
{
    if (m_fFixedTime <= 0.f)
        Cal_OffsetDir();
 
    Restrict_Offset();
    Update_Transform();
}

void MainCameraScript::Late_Tick()
{
    if (KEYTAP(KEY_TYPE::Z))
    {
        if (m_pTarget.lock())
        {
            m_pPlayer.lock()->Get_FSM()->Reset_Target();
            m_pTarget.reset();
        }
        else
            Find_Target();
    }

}

void MainCameraScript::Set_PosDirectly(const _float3& vCenterpos, const _float3& vDir)
{
    m_vFixedPos = vCenterpos;
    m_vFixedDir = vDir;

    _float fMinDist = FLT_MAX;
    Ray ray;
    ray.position = vCenterpos;
    ray.direction = m_vOffset;

    auto& objects = CUR_SCENE->Get_Objects();
    for (auto& object : objects)
    {
        if (!object->Get_Collider())
            continue;

        if (object->Get_Collider()->Get_ColliderType() != ColliderType::Mesh)
            continue;

        _float fDist = 0.f;
        if (object->Get_Collider()->Intersects(ray, fDist))
        {
            if (fDist < 0)
                continue;
            if (fMinDist > fDist)
            {
                fMinDist = fDist;
            }
        }
    }

    if ((fMinDist -= 0.5f) >= m_fFixedDist)
        fMinDist = m_fFixedDist;


    _float3 pos =
        vCenterpos + vDir * fMinDist;

    Get_Transform()->Set_State(Transform_State::POS, _float4(pos, 1.f));

    Get_Transform()->LookAt(_float4(vCenterpos, 1.f));

    Get_Transform()->Scaled(_float3(1.f));

}

void MainCameraScript::Cal_OffsetDir()
{
    m_fFollowSpeed = 5.f;
    _float2 mouseDir = INPUT.GetMouseDir();

    
    //auto playerController = m_pPlayer.lock()->Get_CharacterController()->Get_Actor();
    //
    //_float4 vPlayerPos = { _float(playerController->getPosition().x), _float(playerController->getPosition().y), _float(playerController->getPosition().z), 1.f };
   
    _float4 vPlayerPos = m_pPlayer.lock()->Get_Transform()->Get_State(Transform_State::POS);

    _float3 vDir = (vPlayerPos - Get_Transform()->Get_State(Transform_State::POS)).xyz();

    vDir.Normalize();

    if (m_pTarget.lock())
    {
        _float3 vNewOffset = m_vOffset - Get_Transform()->Get_State(Transform_State::LOOK);
        vNewOffset.Normalize();
        vNewOffset.y = m_vOffset.y;

        m_vOffset = vNewOffset;
        // normalize y�� �����ϱ�
    }
    _float3 vRight = _float3::Up.Cross(vDir);
    vRight.Normalize();
    m_vOffset += vRight * -mouseDir.x * m_fRotationSpeed * fDT;
    m_vOffset += _float3::Up * mouseDir.y * m_fRotationSpeed * fDT;

    m_vOffset.Normalize();

}

void MainCameraScript::Restrict_Offset()
{

    _float3 projOffset = m_vOffset;
    projOffset.y = 0.f;
    projOffset.Normalize();
    if ((m_vOffset.Dot(projOffset) < cosf(m_fMaxHeightRadian)))
    {
        m_vOffset.y = m_vOffset.Dot(projOffset) * tanf(m_fMaxHeightRadian);
    }
    if (m_vOffset.y < 0.f)
        m_vOffset.y = 0.f;
    m_vOffset.Normalize();
}

void MainCameraScript::Update_Transform()
{
 /*   auto playerController = m_pPlayer.lock()->Get_CharacterController()->Get_Actor();
    auto playerPos = playerController->getFootPosition();*/
    //_float4 vPlayerPos = { _float(playerPos.x), _float(playerPos.y), _float(playerPos.z), 1.f };
    _float4 vPlayerPos = m_pPlayer.lock()->Get_Transform()->Get_State(Transform_State::POS);

    vPlayerPos += _float4(0.f, 1.f, 0.f, 0.f);
    _float4 vCenterPos = _float4(0.f);
    if (m_fFixedTime > 0.f)
        vCenterPos = _float4(m_vFixedPos, 1.f);
    else
        vCenterPos = vPlayerPos;


    _float fMinDist = 5.f; //FLT_MAX;
    //Ray ray;
    //ray.position = vCenterPos.xyz();
    //ray.direction = m_vOffset;
    //physx::PxRaycastBuffer hit{};
    //physx::PxQueryFilterData filterData;
    //filterData.flags = physx::PxQueryFlag::eSTATIC;
    //if (PHYSX.Get_PxScene()->raycast({ vCenterPos.x,vCenterPos.y,vCenterPos.z }, { m_vOffset.x,m_vOffset.y,m_vOffset.z }, 5.f, hit, PxHitFlags(physx::PxHitFlag::eDEFAULT), filterData))
    //{
    //    _float3 vHitPoint = { hit.getAnyHit(0).position.x, hit.getAnyHit(0).position.y, hit.getAnyHit(0).position.z };
    //    fMinDist = hit.getAnyHit(0).distance;
    //}
    /*auto& objects = CUR_SCENE->Get_Objects();
    for (auto& object : objects)
    {
       if(!object->Get_Collider())
          continue;

       if(object->Get_Collider()->Get_ColliderType() != ColliderType::Mesh)
          continue;

       if (!object->Get_RigidBody())
          continue;
       if(object->Get_RigidBody()->Get_RigidBody()->getActorFlags() & PxActorFlag::eDISABLE_SIMULATION)
          continue;

       _float fDist = 0.f;
       if (object->Get_Collider()->Intersects(ray, fDist))
       {
          if(fDist<0)
             continue;
          if (fMinDist > fDist)
          {
             fMinDist = fDist;
          }
       }
    }*/


    // Set Position
    if (m_fFixedTime > 0.f)
    {
        m_fFixedTime -= fDT;
        if ((fMinDist -= 0.5f) >= m_fFixedDist)
            fMinDist = m_fFixedDist;

        _float4 pos = _float4::Lerp(Get_Transform()->Get_State(Transform_State::POS),
            vCenterPos + _float4(m_vFixedDir, 0.f) * fMinDist,
            fDT * m_fFollowSpeed);

        Get_Transform()->Set_State(Transform_State::POS, pos);
    }
    else
    {
        if ((fMinDist -= 0.5f) >= m_fMaxDistance)
            fMinDist = m_fMaxDistance;

        _float4 pos = _float4::Lerp(Get_Transform()->Get_State(Transform_State::POS),
            vPlayerPos + _float4(m_vOffset, 0.f) * fMinDist,
            fDT * m_fFollowSpeed);

        Get_Transform()->Set_State(Transform_State::POS, pos);
    }


    // Set Lookat
    if (m_fFixedTime > 0.f)
    {
        Get_Transform()->LookAt(vCenterPos);
    }
    else
    {
        if (!m_pTarget.lock())
            Get_Transform()->LookAt(vPlayerPos);
        else
        {
            _float4 targetPos = _float4(0.f);
            if (m_pTarget.lock()->Get_CharacterController())
            {
                auto targetControllerPos = m_pTarget.lock()->Get_CharacterController()->Get_Actor()->getPosition();
                targetPos = { _float(targetControllerPos.x), _float(targetControllerPos.y), _float(targetControllerPos.z),1.f };
            }
            else
                targetPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS);

            targetPos = (vPlayerPos + targetPos) / 2.f;
            Get_Transform()->LookAt(targetPos);
        }
    }

    if (m_fShakeTime > 0.f)
    {
        m_fShakeTime -= TIME.GetAbsoluteTime();
        _float3 vRandDir = MathUtils::Get_RandomVector(_float3(-1.f), _float3(1.f));

        vRandDir.Normalize();
        vRandDir *= m_fShakePower;

        _float3 vCurPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
        vCurPos += vRandDir;

        Get_Transform()->Set_State(Transform_State::POS, _float4(vCurPos, 1.f));
    }
}

void MainCameraScript::Find_Target()
{
    auto& objects = CUR_SCENE->Get_Objects();
    vector<shared_ptr<GameObject>> candidateObject;

    for (auto& object : objects)
    {
        if (object->Get_ObjectGroup() != OBJ_MONSTER)
            continue;

        _float3 cullPos = object->Get_Transform()->Get_State(Transform_State::POS).xyz();
        _float cullRadius = 1.5f;
        Frustum frustum = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_Frustum();
        if (frustum.Contain_Sphere(cullPos, cullRadius))
        {
            candidateObject.push_back(object);
        }
    }

    _float fMinDist = FLT_MAX;
    _float3 vPlayerPos = m_pPlayer.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();

    for (auto& object : candidateObject)
    {
        _float3 vObjectPos = object->Get_Transform()->Get_State(Transform_State::POS).xyz();
        _float distSQ = (vPlayerPos - vObjectPos).LengthSquared();

        if (distSQ >= 20.f * 20.f)
            continue;

        if (fMinDist * fMinDist > distSQ)
        {
            fMinDist = sqrtf(distSQ);
            m_pTarget = object;
            m_pPlayer.lock()->Get_FSM()->Set_Target(m_pTarget.lock());
        }
    }
}

void MainCameraScript::Fix_Camera(_float fTime, _float3 vDir, _float fDist)
{
    m_fFixedTime = fTime;
    m_vFixedDir = vDir;
    m_fFixedDist = fDist;

}

void MainCameraScript::ShakeCamera(_float fTime, _float fPower)
{
    if (m_fShakeTime > 0.f)
    {
        if (m_fShakePower < fPower)
            m_fShakePower = fPower;
    }
    else
        m_fShakePower = fPower;

    m_fShakeTime = fTime;

}

void MainCameraScript::Set_FixedLookTarget(const _float3& vPos)
{
    m_vFixedPos = vPos;
}