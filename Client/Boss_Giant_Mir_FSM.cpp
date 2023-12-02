#include "pch.h"
#include "Boss_Giant_Mir_FSM.h"
#include "ModelAnimator.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"
#include "CharacterController.h"
#include "CounterMotionTrailScript.h"
#include "MainCameraScript.h"
#include "UiDamageCreate.h"
#include "DragonBall_FSM.h"
#include "UIBossHpBar.h"
#include "SimpleMath.h"
#include "DestroyBuilding_FSM.h"

HRESULT Boss_Giant_Mir_FSM::Init()
{
    if (!m_bInitialize)
    {
        auto animator = Get_Owner()->Get_Animator();
        if (animator)
        {
            animator->Set_CurrentAnim(L"SQ_Spawn", false, 1.f);
            m_eCurState = STATE::SQ_Spawn;
        }

        m_fDetectRange = 10.f;

        if (!m_pTarget.expired())
        {
            _float3 vLookPos = _float3{ -0.25f, 0.f, -57.f };
            Get_Transform()->Set_LookDir(vLookPos);
        }

        _float4 vPos = Get_Transform()->Get_State(Transform_State::POS) +
                       Get_Transform()->Get_State(Transform_State::LOOK) * -2.f +
                       Get_Transform()->Get_State(Transform_State::UP) * 2.f;

        Get_Transform()->Set_State(Transform_State::POS, vPos);


        m_iHeadBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Bone067");
        m_iMouseBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Bone066");
  
        HeadBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iHeadBoneIndex) *
            _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

        m_vHeadCamPos = m_vHeadBonePos + (Get_Transform()->Get_State(Transform_State::LOOK) * 30.f);

        m_pCamera = CUR_SCENE->Get_MainCamera();

        m_fRunSpeed = 4.f;
        m_fKnockDownSpeed = 4.f;

        m_fDetectRange = 28.f;

       

        //Setting_DragonBall();

        m_bInitialize = true;
    }

    return S_OK;
}

void Boss_Giant_Mir_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 6.f + _float3::Up);
    }
}

void Boss_Giant_Mir_FSM::State_Tick()
{
    State_Init();

    m_iCurFrame = Get_CurFrame();

    switch (m_eCurState)
    {
    case STATE::SQ_Spawn:
        SQ_Spawn();
        break;
    case STATE::b_idle:
        b_idle();
        break;
    case STATE::skill_1100:
        skill_1100();
        break;
    case STATE::skill_100100:
        skill_100100();
        break;
    case STATE::SQ_Leave:
        SQ_Leave();
        break;
    }


    if (m_iPreFrame != m_iCurFrame)
        m_iPreFrame = m_iCurFrame;
}

void Boss_Giant_Mir_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::SQ_Spawn:
            SQ_Spawn_Init();
            break;
        case STATE::groggy_start:
            groggy_start_Init();
            break;
        case STATE::groggy_loop:
            groggy_loop_Init();
            break;
        case STATE::groggy_end:
            groggy_end_Init();
            break;
        case STATE::b_idle:
            b_idle_Init();
            break;
        case STATE::skill_1100:
            skill_1100_Init();
            break;
        case STATE::skill_100100:
            skill_100100_Init();
            break;
        case STATE::SQ_Leave:
            SQ_Leave_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void Boss_Giant_Mir_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Boss_Giant_Mir_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
    if (pCollider->Get_Owner() == nullptr)
        return;

    if (!pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>())
        return;

    if (pCollider->Get_CollisionGroup() == Player_Attack || pCollider->Get_CollisionGroup() == Player_Skill)
    {

        if (!m_bInvincible)
        {
            wstring strSkillName = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_SkillName();
            _float fAttackDamage = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_AttackDamage();

            shared_ptr<GameObject> targetToLook = nullptr;
            // skillName
            if (strSkillName.find(L"_Skill") != wstring::npos)
                targetToLook = pCollider->Get_Owner(); // Collider owner
            else // 
                targetToLook = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_ColliderOwner(); // Collider

            if (targetToLook == nullptr)
                return;

            Get_Hit(strSkillName, fAttackDamage, targetToLook);
        }
    }
}

void Boss_Giant_Mir_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Boss_Giant_Mir_FSM::Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget)
{
    //Calculate Damage 
    m_pOwner.lock()->Get_Hurt(fDamage);

    CUR_SCENE->Get_UI(L"UI_Damage_Controller")->Get_Script<UiDamageCreate>()->Create_Damage_Font(Get_Owner(), fDamage);

    _float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
    _float3 vOppositePos = pLookTarget->Get_Transform()->Get_State(Transform_State::POS).xyz();

    m_vHitDir = vOppositePos - vMyPos;
    m_vHitDir.y = 0.f;
    m_vHitDir.Normalize();

    if (skillname == NORMAL_ATTACK || skillname == NORMAL_SKILL)
    {
        
    }
    else if (skillname == KNOCKBACK_ATTACK || skillname == KNOCKBACK_SKILL)
    {
        
    }
    else if (skillname == KNOCKDOWN_ATTACK || skillname == KNOCKDOWN_SKILL)
    {
        
    }
    else if (skillname == AIRBORNE_ATTACK || skillname == AIRBORNE_SKILL)
    {
        
    }

}

void Boss_Giant_Mir_FSM::AttackCollider_On(const wstring& skillname, _float fAttackDamage)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(fAttackDamage);
    }
}

void Boss_Giant_Mir_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(0.f);
    }
}

void Boss_Giant_Mir_FSM::Set_State(_uint iIndex)
{
    m_eCurState = (STATE)iIndex;
}

void Boss_Giant_Mir_FSM::SQ_Spawn()
{
    Calculate_IntroHeadCam();

    if (m_iCurFrame < 70)
    {
        if (Init_CurFrame(56))
        {
            if (!m_pOwner.expired())
                m_pOwner.lock()->Get_Animator()->Set_AnimationSpeed(2.f);
        }

        if (!m_pCamera.expired())
        {
            m_fIntroCamDistance = CamDistanceLerp(15.f, 7.f, m_fCamRatio);
            
            m_fCamRatio += fDT * 0.5f;
            
            if (m_fCamRatio >= 1.f)
                m_fCamRatio = 1.f;
            
            _float3 vLookPos = Get_Transform()->Get_State(Transform_State::POS).xyz() +
                               Get_Transform()->Get_State(Transform_State::UP) * 15.f;
            
          
            _float3 vDir = m_vIntroCamPos - vLookPos;
            vDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(5.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(vLookPos);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(1.f, vDir, m_fIntroCamDistance);
        }
    }
    else 
    {
        if (Init_CurFrame(70))
        {
            if (!m_pCamera.expired())
                m_vCamStopPos = m_pCamera.lock()->Get_Transform()->Get_State(Transform_State::POS);
        }
        else if (Init_CurFrame(85))
        {
            if (!m_pOwner.expired())
                m_pOwner.lock()->Get_Animator()->Set_AnimationSpeed(1.f);
        }
        else if (Init_CurFrame(234))
        {
            m_pOwner.lock()->Set_TimeSlowed(false);
            Get_Owner()->Get_Animator()->Set_AnimationSpeed(0.3f);
        }
        else if (Init_CurFrame(235))
        {
            Destroy_MapObject();
        }
        else if (Init_CurFrame(250))
        {
            m_pOwner.lock()->Set_TimeSlowed(true);
            Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f);
        }

        if (m_iCurFrame >= 234 && m_iCurFrame <= 250)
            TIME.Set_TimeSlow(0.1f, 0.3f);



        if (!m_pCamera.expired())
        {
            m_pCamera.lock()->Get_Transform()->Set_State(Transform_State::POS, m_vCamStopPos);
            m_vHeadCamDir = m_vCamStopPos - m_vHeadBonePos.xyz();
            m_vHeadCamDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vHeadBonePos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedTime(1.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedDir(m_vHeadCamDir);
        }
    }

    if (Is_AnimFinished())
    {
        g_bCutScene = false;
        m_eCurState = STATE::b_idle;

        //Add_BossHp UI
        if (!m_pOwner.expired())
        {
            if (!m_pOwner.lock()->Get_Script<UIBossHpBar>())
            {
                auto pScript = make_shared<UIBossHpBar>(BOSS::MIR);
                m_pOwner.lock()->Add_Component(pScript);
                pScript->Init();
            }
        }
    }
}

void Boss_Giant_Mir_FSM::SQ_Spawn_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"SQ_Spawn", 0.3f, false, 1.f);

    Calculate_IntroHeadCam();
    
    m_fCamRatio = 0.f;
    g_bCutScene = true;
    
    if (!m_pCamera.expired())
        m_pCamera.lock()->Get_Transform()->Set_State(Transform_State::POS, m_vIntroCamPos);
}

void Boss_Giant_Mir_FSM::groggy_start()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::groggy_loop;
}

void Boss_Giant_Mir_FSM::groggy_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"groggy_start", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_tBreathCoolTime.fAccTime = 0.f;
}

void Boss_Giant_Mir_FSM::groggy_loop()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::groggy_end;
}

void Boss_Giant_Mir_FSM::groggy_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"groggy_loop", 0.1f, false, 1.f);
}

void Boss_Giant_Mir_FSM::groggy_end()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Giant_Mir_FSM::groggy_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"groggy_end", 0.1f, false, 1.f);
}


void Boss_Giant_Mir_FSM::SQ_Leave()
{
}

void Boss_Giant_Mir_FSM::SQ_Leave_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"SQ_Leave", 0.15f, false, 1.f);

    m_bInvincible = true;
}



void Boss_Giant_Mir_FSM::b_idle()
{
    m_tAttackCoolTime.fAccTime += fDT;

    //Create_Meteor();

    //if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
       //Set_AttackPattern();
}

void Boss_Giant_Mir_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.6f, true, 1.f);

    m_bInvincible = false;

    m_tAttackCoolTime.fAccTime = 0.f;
    m_tBreathCoolTime.fAccTime = 0.f;

    AttackCollider_Off();
   
    //Add_BossHp UI
    if (!m_pOwner.expired())
    {
        if (!m_pOwner.lock()->Get_Script<UIBossHpBar>())
        {
            auto pScript = make_shared<UIBossHpBar>(BOSS::MIR);
            m_pOwner.lock()->Add_Component(pScript);
            pScript->Init();
        }
    }   
}

void Boss_Giant_Mir_FSM::skill_1100()
{
    m_tBreathCoolTime.fAccTime += fDT;

    if (m_iCurFrame >= 154 && m_iCurFrame <= 254)
    {
        if (m_tBreathCoolTime.fAccTime >= m_tBreathCoolTime.fCoolTime)
        {
            m_tBreathCoolTime.fAccTime = 0.f;

            MouseBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iMouseBoneIndex) *
                _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

            _float4 vBonePos = _float4{ MouseBoneMatrix.Translation().x, MouseBoneMatrix.Translation().y, MouseBoneMatrix.Translation().z , 1.f };

            vBonePos = vBonePos + Get_Transform()->Get_State(Transform_State::LOOK);

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 10.f;

            _float4 vSkillPos = vBonePos;

            Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, NORMAL_ATTACK, 10.f);
        }
    }

    if (m_iCurFrame >= Get_FinalFrame() - 5 )
        m_eCurState = STATE::b_idle;    
    

}

void Boss_Giant_Mir_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
    m_tBreathCoolTime.fAccTime = 0.f;
}

void Boss_Giant_Mir_FSM::skill_100100()
{
    m_tBreathCoolTime.fAccTime += fDT;

    if (m_iCurFrame >= 58 && m_iCurFrame <= 168)
    {
        if (m_tBreathCoolTime.fAccTime >= m_tBreathCoolTime.fCoolTime)
        {
            m_tBreathCoolTime.fAccTime = 0.f;

            MouseBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iMouseBoneIndex) *
                _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

            _float4 vBonePos = _float4{ MouseBoneMatrix.Translation().x, MouseBoneMatrix.Translation().y, MouseBoneMatrix.Translation().z , 1.f };

            vBonePos = vBonePos + Get_Transform()->Get_State(Transform_State::LOOK);

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 10.f;

            _float4 vSkillPos = vBonePos;

            Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, NORMAL_ATTACK, 10.f);
        }
    }

    if (m_iCurFrame >= Get_FinalFrame() - 5)
        m_eCurState = STATE::b_idle;
}

void Boss_Giant_Mir_FSM::skill_100100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
    m_tBreathCoolTime.fAccTime = 0.f;
}

void Boss_Giant_Mir_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType, _float fAttackDamage)
{
    shared_ptr<GameObject> SkillCollider = make_shared<GameObject>();

    SkillCollider->GetOrAddTransform();
    SkillCollider->Get_Transform()->Set_State(Transform_State::POS, vPos);

    auto pSphereCollider = make_shared<SphereCollider>(fSkillRange);
    pSphereCollider->Set_CenterPos(_float3{ vPos.x,vPos.y, vPos.z });
    
    SkillCollider->Add_Component(pSphereCollider);
    SkillCollider->Get_Collider()->Set_CollisionGroup(Monster_Skill);

    SkillCollider->Add_Component(make_shared<AttackColliderInfoScript>());
    SkillCollider->Get_Collider()->Set_Activate(true);
    SkillCollider->Get_Script<AttackColliderInfoScript>()->Set_SkillName(SkillType);
    SkillCollider->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(fAttackDamage);
    SkillCollider->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());
    SkillCollider->Set_Name(L"Boss_Mir_SkillCollider");
    SkillCollider->Add_Component(make_shared<ForwardMovingSkillScript>(desc));
    SkillCollider->Get_Script<ForwardMovingSkillScript>()->Init();

    EVENTMGR.Create_Object(SkillCollider);
}

void Boss_Giant_Mir_FSM::Create_Meteor()
{
    m_tMeteorCoolTime.fAccTime += fDT;

    if (m_tMeteorCoolTime.fAccTime >= m_tMeteorCoolTime.fCoolTime)
    {
        if (!m_pTarget.expired())
        {
            _float4 vPlayerPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS);

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = _float3{ 0.f,-1.f,0.f };
            desc.fMoveSpeed = 10.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 20.f;

            for (_uint i = 0; i < 6; i++)
            {
                _float fOffSetX = ((rand() * 2 / _float(RAND_MAX) - 1) * (rand() % 10 + 5));
                _float fOffSetZ = ((rand() * 2 / _float(RAND_MAX) - 1) * (rand() % 10 + 5));

                _float4 vSkillPos = vPlayerPos + _float4{ fOffSetX, 10.f, fOffSetZ, 0.f };

                Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, KNOCKDOWN_SKILL, 10.f);
            }

            m_tMeteorCoolTime.fAccTime = 0.f;
        }
    }

}

void Boss_Giant_Mir_FSM::Set_AttackPattern()
{
    m_eCurState = STATE::SQ_Spawn;
   /* _uint iRan = rand() % 2;

    while (true)
    {
        if (iRan == m_iPreAttack)
            iRan = rand() % 2;
        else
            break;
    }

    if (iRan == 0)
    {
        m_eCurState = STATE::skill_1100;
        m_iPreAttack = 0;
    }
    else if (iRan == 1)
    {
        m_eCurState = STATE::skill_100100;
        m_iPreAttack = 1;
    }*/
  
}

void Boss_Giant_Mir_FSM::Setting_DragonBall()
{
    _uint iDragonBallIndex = 4;
    for (_uint i = 0; i < 3; i++)
    {
        wstring DragonBallName = L"Anim_P_R02_DecoBall_00_Idle-1";
        DragonBallName += to_wstring(iDragonBallIndex);
        auto DragonBall = CUR_SCENE->Get_GameObject(DragonBallName);

        if (DragonBall == nullptr)
            continue;

        DragonBall->Add_Component(make_shared<SphereCollider>(2.f)); //SphereCollider
        DragonBall->Get_Collider()->Set_CollisionGroup(MAPObject);
        DragonBall->Get_Collider()->Set_Activate(true);

        DragonBall->Set_DrawShadow(true);
        DragonBall->Set_ObjectGroup(OBJ_MAPOBJECT);
        DragonBall->Add_Component(make_shared<DragonBall_FSM>());
        DragonBall->Get_FSM()->Set_Target(m_pOwner.lock());

        iDragonBallIndex++;
    }
}

void Boss_Giant_Mir_FSM::Destroy_MapObject()
{
    if (CUR_SCENE->Get_GameObject(L"F01_P_BackGCircle_02_KEK"))
    {
        _float4 vPos = CUR_SCENE->Get_GameObject(L"F01_P_BackGCircle_02_KEK")->Get_Transform()->Get_State(Transform_State::POS);
        EVENTMGR.Delete_Object(CUR_SCENE->Get_GameObject(L"F01_P_BackGCircle_02_KEK"));
        
        shared_ptr<GameObject> DestroyBuilding = make_shared<GameObject>();
        DestroyBuilding->GetOrAddTransform()->Set_State(Transform_State::POS, vPos);
        DestroyBuilding->Get_Transform()->Scaled(_float3(3.f, 3.f,3.f));
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Anim_Mir_Intro_v02_BuildCenter");
            animator->Set_Model(model);
        }

        DestroyBuilding->Add_Component(animator);

        DestroyBuilding->Add_Component(make_shared<DestroyBuilding_FSM>());
        DestroyBuilding->Get_FSM()->Init();
        EVENTMGR.Create_Object(DestroyBuilding);
    }

    if (CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_MiddleHouse_01_AHJ"))
    {
        _float4 vPos = CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_MiddleHouse_01_AHJ")->Get_Transform()->Get_State(Transform_State::POS);
        EVENTMGR.Delete_Object(CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_MiddleHouse_01_AHJ"));

        shared_ptr<GameObject> DestroyBuilding = make_shared<GameObject>();
        DestroyBuilding->GetOrAddTransform()->Set_State(Transform_State::POS, vPos);
        DestroyBuilding->Get_Transform()->Scaled(_float3(3.f, 3.f, 3.f));
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Anim_Mir_Intro_v02_BuildCenter");
            animator->Set_Model(model);
        }

        DestroyBuilding->Add_Component(animator);

        DestroyBuilding->Add_Component(make_shared<DestroyBuilding_FSM>());
        DestroyBuilding->Get_FSM()->Init();
        EVENTMGR.Create_Object(DestroyBuilding);
    }

    if (CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_MiddleHouse_02_AHJ"))
    {
        _float4 vPos = CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_MiddleHouse_02_AHJ")->Get_Transform()->Get_State(Transform_State::POS);
        EVENTMGR.Delete_Object(CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_MiddleHouse_02_AHJ"));

        shared_ptr<GameObject> DestroyBuilding = make_shared<GameObject>();
        DestroyBuilding->GetOrAddTransform()->Set_State(Transform_State::POS, vPos);
        DestroyBuilding->Get_Transform()->Scaled(_float3(3.f, 3.f, 3.f));
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Anim_Mir_Intro_v02_BuildCenter");
            animator->Set_Model(model);
        }

        DestroyBuilding->Add_Component(animator);

        DestroyBuilding->Add_Component(make_shared<DestroyBuilding_FSM>());
        DestroyBuilding->Get_FSM()->Init();
        EVENTMGR.Create_Object(DestroyBuilding);
    }

    if (CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_MiddleHouse_03_AHJ"))
    {
        _float4 vPos = CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_MiddleHouse_03_AHJ")->Get_Transform()->Get_State(Transform_State::POS);
        EVENTMGR.Delete_Object(CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_MiddleHouse_03_AHJ"));

        shared_ptr<GameObject> DestroyBuilding = make_shared<GameObject>();
        DestroyBuilding->GetOrAddTransform()->Set_State(Transform_State::POS, vPos);
        DestroyBuilding->Get_Transform()->Scaled(_float3(3.f, 3.f, 3.f));
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Anim_Mir_Intro_v02_BuildCenter");
            animator->Set_Model(model);
        }

        DestroyBuilding->Add_Component(animator);

        DestroyBuilding->Add_Component(make_shared<DestroyBuilding_FSM>());
        DestroyBuilding->Get_FSM()->Init();
        EVENTMGR.Create_Object(DestroyBuilding);
    }

    if (CUR_SCENE->Get_GameObject(L"R02_Dragon_002_01"))
    {
        _float4 vPos = CUR_SCENE->Get_GameObject(L"R02_Dragon_002_01")->Get_Transform()->Get_State(Transform_State::POS);
        EVENTMGR.Delete_Object(CUR_SCENE->Get_GameObject(L"R02_Dragon_002_01"));

        shared_ptr<GameObject> DestroyBuilding = make_shared<GameObject>();
        DestroyBuilding->GetOrAddTransform()->Set_State(Transform_State::POS, vPos);
        DestroyBuilding->Get_Transform()->Scaled(_float3(3.f, 3.f, 3.f));
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Anim_Mir_Intro_v02_BuildCenter");
            animator->Set_Model(model);
        }

        DestroyBuilding->Add_Component(animator);

        DestroyBuilding->Add_Component(make_shared<DestroyBuilding_FSM>());
        DestroyBuilding->Get_FSM()->Init();
        EVENTMGR.Create_Object(DestroyBuilding);
    }

    if (CUR_SCENE->Get_GameObject(L"Mir_R02_Dragon_001_01"))
    {
        _float4 vPos = CUR_SCENE->Get_GameObject(L"Mir_R02_Dragon_001_01")->Get_Transform()->Get_State(Transform_State::POS);
        EVENTMGR.Delete_Object(CUR_SCENE->Get_GameObject(L"Mir_R02_Dragon_001_01"));

        shared_ptr<GameObject> DestroyBuilding = make_shared<GameObject>();
        DestroyBuilding->GetOrAddTransform()->Set_State(Transform_State::POS, vPos);
        DestroyBuilding->Get_Transform()->Scaled(_float3(3.f, 3.f, 3.f));
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Anim_Mir_Intro_v02_BuildCenter");
            animator->Set_Model(model);
        }

        DestroyBuilding->Add_Component(animator);

        DestroyBuilding->Add_Component(make_shared<DestroyBuilding_FSM>());
        DestroyBuilding->Get_FSM()->Init();
        EVENTMGR.Create_Object(DestroyBuilding);
    }

    if (CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_Circle_01_AHJ"))
    {
        _float4 vPos = CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_Circle_01_AHJ")->Get_Transform()->Get_State(Transform_State::POS);
        EVENTMGR.Delete_Object(CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_Circle_01_AHJ"));

        shared_ptr<GameObject> DestroyBuilding = make_shared<GameObject>();
        DestroyBuilding->GetOrAddTransform()->Set_State(Transform_State::POS, vPos);
        DestroyBuilding->Get_Transform()->Scaled(_float3(3.f, 3.f, 3.f));
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Anim_Mir_Intro_v02_BuildCenter");
            animator->Set_Model(model);
        }

        DestroyBuilding->Add_Component(animator);

        DestroyBuilding->Add_Component(make_shared<DestroyBuilding_FSM>());
        DestroyBuilding->Get_FSM()->Init();
        EVENTMGR.Create_Object(DestroyBuilding);
    }

    if (CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_Circle_02_AHJ"))
    {
         _float4 vPos = CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_Circle_02_AHJ")->Get_Transform()->Get_State(Transform_State::POS);
        EVENTMGR.Delete_Object(CUR_SCENE->Get_GameObject(L"Mir_Mirdragon_P_Circle_02_AHJ"));

        shared_ptr<GameObject> DestroyBuilding = make_shared<GameObject>();
        DestroyBuilding->GetOrAddTransform()->Set_State(Transform_State::POS, vPos);
        DestroyBuilding->Get_Transform()->Scaled(_float3(3.f, 3.f, 3.f));
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Anim_Mir_Intro_v02_BuildCenter");
            animator->Set_Model(model);
        }

        DestroyBuilding->Add_Component(animator);

        DestroyBuilding->Add_Component(make_shared<DestroyBuilding_FSM>());
        DestroyBuilding->Get_FSM()->Init();
        EVENTMGR.Create_Object(DestroyBuilding);
    }



}

void Boss_Giant_Mir_FSM::Calculate_IntroHeadCam()
{
    HeadBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iHeadBoneIndex) *
        _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

    m_vHeadBonePos = _float4{ HeadBoneMatrix.Translation().x, HeadBoneMatrix.Translation().y, HeadBoneMatrix.Translation().z , 1.f };
    m_vHeadCamPos = m_vHeadBonePos + 
                    (Get_Transform()->Get_State(Transform_State::RIGHT) * -20.f) +
                    (Get_Transform()->Get_State(Transform_State::UP) * 20.f) +
                    (Get_Transform()->Get_State(Transform_State::LOOK) * 30.f) ;

    //m_vHeadCamPos = m_vHeadBonePos + (Get_Transform()->Get_State(Transform_State::LOOK) * 30.f);

    m_vIntroCamPos =
        (Get_Transform()->Get_State(Transform_State::POS) +
            Get_Transform()->Get_State(Transform_State::LOOK) * 5.f +
                (Get_Transform()->Get_State(Transform_State::UP) * 15.f));
}

_float Boss_Giant_Mir_FSM::CamDistanceLerp(_float fStart, _float fEnd, _float fRatio)
{
    return fStart * (1.f - fRatio) + fEnd * fRatio;
}



