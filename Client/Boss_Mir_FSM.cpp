#include "pch.h"
#include "Boss_Mir_FSM.h"
#include "ModelAnimator.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"
#include "CharacterController.h"

HRESULT Boss_Mir_FSM::Init()
{
    auto animator = Get_Owner()->Get_Animator();
    if (animator)
    {
        animator->Set_CurrentAnim(L"3D_ui_start", false, 1.f);
        m_eCurState = STATE::First_Meet;
    }

    m_fDetectRange = 10.f;

    Add_Boss_Mir_Collider();

    return S_OK;
}

void Boss_Mir_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 3.f + _float3::Up);
    }

    if (!m_pTailCollider.expired())
    {
        //Head_Bone_Pos
        //(AnimationModel BonePos)
        TailBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iTailBoneIndex) *
            _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

        _float4 vBonePos = _float4{ TailBoneMatrix.Translation().x, TailBoneMatrix.Translation().y, TailBoneMatrix.Translation().z , 1.f };
        m_pTailCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, vBonePos);
    }

}

void Boss_Mir_FSM::State_Tick()
{
    State_Init();

    switch (m_eCurState)
    {
    case STATE::First_Meet:
        First_Meet();
        break;
    case STATE::sq_Intro:
        sq_Intro();
        break;
    case STATE::sq_Intro2:
        sq_Intro2();
        break;
    case STATE::b_idle:
        b_idle();
        break;
    case STATE::turn_l:
        turn_l();
        break;
    case STATE::turn_r:
        turn_r();
        break;
    case STATE::die:
        die();
        break;
    case STATE::groggy_start:
        groggy_start();
        break;
    case STATE::groggy_loop:
        groggy_loop();
        break;
    case STATE::groggy_end:
        groggy_end();
        break;
    case STATE::SQ_SBRin_Roar:
        SQ_SBRin_Roar();
        break;
    case STATE::skill_1100:
        skill_1100();
        break;
    case STATE::skill_2100:
        skill_2100();
        break;
    case STATE::skill_3100:
        skill_3100();
        break;
    case STATE::skill_4100:
        skill_4100();
        break;
    case STATE::skill_5100:
        skill_5100();
        break;
    case STATE::skill_9100:
        skill_9100();
        break;
    case STATE::skill_11100:
        skill_11100();
        break;
    case STATE::skill_12100:
        skill_12100();
        break;
    case STATE::skill_13100:
        skill_13100();
        break;
    case STATE::skill_14100:
        skill_14100();
        break;
    case STATE::skill_100000:
        skill_100000();
        break;
    case STATE::skill_100100:
        skill_100100();
        break;
    case STATE::skill_101100:
        skill_101100();
        break;
    case STATE::skill_100200:
        skill_100200();
        break;
    }
}

void Boss_Mir_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::First_Meet:
            First_Meet_Init();
            break;
        case STATE::sq_Intro:
            sq_Intro_Init();
            break;
        case STATE::sq_Intro2:
            sq_Intro2_Init();
            break;
        case STATE::b_idle:
            b_idle_Init();
            break;
        case STATE::turn_l:
            turn_l_Init();
            break;
        case STATE::turn_r:
            turn_r_Init();
            break;
        case STATE::die:
            die_Init();
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
        case STATE::SQ_SBRin_Roar:
            SQ_SBRin_Roar_Init();
            break;
        case STATE::skill_1100:
            skill_1100_Init();
            break;
        case STATE::skill_2100:
            skill_2100_Init();
            break;
        case STATE::skill_3100:
            skill_3100_Init();
            break;
        case STATE::skill_4100:
            skill_4100_Init();
            break;
        case STATE::skill_5100:
            skill_5100_Init();
            break;
        case STATE::skill_9100:
            skill_9100_Init();
            break;
        case STATE::skill_11100:
            skill_11100_Init();
            break;
        case STATE::skill_12100:
            skill_12100_Init();
            break;
        case STATE::skill_13100:
            skill_13100_Init();
            break;
        case STATE::skill_14100:
            skill_14100_Init();
            break;
        case STATE::skill_100000:
            skill_100000_Init();
            break;
        case STATE::skill_100100:
            skill_100100_Init();
            break;
        case STATE::skill_101100:
            skill_101100_Init();
            break;
        case STATE::skill_100200:
            skill_100200_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void Boss_Mir_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Boss_Mir_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
    if (pCollider->Get_Owner() == nullptr)
        return;

    wstring strSkillName = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_SkillName();

    if (!m_bInvincible)
    {
        Get_Hit(strSkillName, pCollider);
    }
}

void Boss_Mir_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Boss_Mir_FSM::Get_Hit(const wstring& skillname, shared_ptr<BaseCollider> pOppositeCollider)
{
    if (skillname == NORMAL_ATTACK || skillname == KNOCKBACK_ATTACK || skillname == KNOCKDOWN_ATTACK || skillname == AIRBORNE_ATTACK)
    {
        _float3 vAttackerPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
        m_vHitDir = vAttackerPos - Get_Transform()->Get_State(Transform_State::POS);
        m_vHitDir.y = 0.f;
        m_vHitDir.Normalize();
    }
    else if (skillname == NORMAL_SKILL || skillname == KNOCKBACK_SKILL || skillname == KNOCKDOWN_SKILL || skillname == AIRBORNE_SKILL)
    {
        m_vHitDir = pOppositeCollider->Get_CenterPos() - Get_Transform()->Get_State(Transform_State::POS);
        m_vHitDir.y = 0.f;
        m_vHitDir.Normalize();
    }

    if (skillname == NORMAL_ATTACK || skillname == NORMAL_SKILL)
    {
        if (!m_bSuperArmor)
        {

        }
    }
    else if (skillname == KNOCKBACK_ATTACK || skillname == KNOCKBACK_SKILL)
    {
        if (!m_bSuperArmor)
        {
           
        }
    }
    else if (skillname == KNOCKDOWN_ATTACK || skillname == KNOCKDOWN_SKILL)
    {
       
    }
    else if (skillname == AIRBORNE_ATTACK || skillname == AIRBORNE_SKILL)
    {
        if (!m_bSuperArmor)
        {
           
        }
    }

}

void Boss_Mir_FSM::AttackCollider_On(const wstring& skillname)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
    }
}

void Boss_Mir_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    }
}

void Boss_Mir_FSM::Set_State(_uint iIndex)
{
}

void Boss_Mir_FSM::First_Meet()
{
    if (Target_In_DetectRange())
        m_bDetected = true;

    if (!m_bDetected)
        Reset_Frame();

    if (Is_AnimFinished())
        m_eCurState = STATE::sq_Intro;
}

void Boss_Mir_FSM::First_Meet_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"3D_ui_start", 0.1f, false, 2.f);

    m_bInvincible = true;
}

void Boss_Mir_FSM::sq_Intro()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::sq_Intro2;
}

void Boss_Mir_FSM::sq_Intro_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"sq_Intro", 0.1f, false, 1.f);

    m_bInvincible = true;
}

void Boss_Mir_FSM::sq_Intro2()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::sq_Intro2_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"sq_Intro2", 0.1f, false, 1.f);

    m_bInvincible = true;
}

void Boss_Mir_FSM::b_idle()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);


    _uint iRan = rand() % 3;

    if (iRan == 0)
        m_eCurState = STATE::skill_1100;
    else if (iRan == 1)
        m_eCurState = STATE::skill_2100;
    else if (iRan == 2)
        m_eCurState = STATE::skill_3100;
    else if (iRan == 3)
        m_eCurState = STATE::skill_4100;
    else if (iRan == 4)
        m_eCurState = STATE::skill_11100;

}

void Boss_Mir_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    m_vTurnVector = _float3(0.f);

    m_bInvincible = false;
}

void Boss_Mir_FSM::turn_l()
{
}

void Boss_Mir_FSM::turn_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"turn_l", 0.1f, false, 1.f);
}

void Boss_Mir_FSM::turn_r()
{
}

void Boss_Mir_FSM::turn_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"turn_l", 0.1f, false, 1.f);
}

void Boss_Mir_FSM::die()
{
}

void Boss_Mir_FSM::die_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"SQ_Dead", 0.1f, false, 1.f);
}

void Boss_Mir_FSM::groggy_start()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::groggy_loop;
}

void Boss_Mir_FSM::groggy_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"groggy_start", 0.1f, false, 1.f);
}

void Boss_Mir_FSM::groggy_loop()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::groggy_end;
}

void Boss_Mir_FSM::groggy_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"groggy_loop", 0.1f, false, 1.f);
}

void Boss_Mir_FSM::groggy_end()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::groggy_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"groggy_end", 0.1f, false, 1.f);
}

void Boss_Mir_FSM::SQ_SBRin_Roar()
{
    if (Get_CurFrame() == 24 ||
        Get_CurFrame() == 34 ||
        Get_CurFrame() == 44 ||
        Get_CurFrame() == 54 ||
        Get_CurFrame() == 64 || 
        Get_CurFrame() == 74 )
    {
        if (!m_bSkillCreate)
        {
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

            if (Get_CurFrame() != 74)
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, NORMAL_ATTACK);
            else
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKBACK_ATTACK);

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::SQ_SBRin_Roar_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"SQ_SBRin_Roar", 0.1f, false, 1.f);
}

void Boss_Mir_FSM::skill_1100()
{
    if (Get_CurFrame() == 46 ||
        Get_CurFrame() == 56 ||
        Get_CurFrame() == 66 ||
        Get_CurFrame() == 76)
    {
        if (!m_bSkillCreate)
        {
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
            
            if (Get_CurFrame() != 76)
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, NORMAL_ATTACK);
            else
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKBACK_ATTACK);
            
            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
    
}

void Boss_Mir_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_2100()
{
    if (Get_CurFrame() == 70)
        AttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 93)
        AttackCollider_Off();

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::skill_2100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_2100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;

}

void Boss_Mir_FSM::skill_3100()
{
    if (Get_CurFrame() == 80)
        AttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 98)
        AttackCollider_Off();

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::skill_3100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_3100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_4100()
{
    if (Get_CurFrame() == 86)
        AttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 108)
        AttackCollider_Off();

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::skill_4100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_4100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_5100()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::skill_5100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_5100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_9100()
{
    if (Get_CurFrame() == 66 ||
        Get_CurFrame() == 76 ||
        Get_CurFrame() == 86 ||
        Get_CurFrame() == 96 ||
        Get_CurFrame() == 106)
    {
        if (!m_bSkillCreate)
        {
            MouseBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iMouseBoneIndex) *
                _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

            _float4 vBonePos = _float4{ MouseBoneMatrix.Translation().x, MouseBoneMatrix.Translation().y, MouseBoneMatrix.Translation().z , 1.f };

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::UP) * -1.f;
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 10.f;

            for (_uint i = 0; i < 4; i++)
            {
                _float fOffSetX = ((rand() * 2 / _float(RAND_MAX) - 1) * (rand() % 5 + 1));
                _float fOffSetZ = ((rand() * 2 / _float(RAND_MAX) - 1) * (rand() % 5 + 1));

                _float4 vSkillPos = vBonePos + _float4{ fOffSetX, 0.f, fOffSetZ, 1.f };

                Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, KNOCKBACK_ATTACK);
            }

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::skill_9100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_9100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_11100()
{
    if (Get_CurFrame() == 67)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 10.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

            Create_ForwardMovingSkillCollider(vSkillPos, 5.f, desc, KNOCKDOWN_ATTACK);

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::skill_11100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_11100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_12100()
{
    if (Get_CurFrame() == 28 ||
        Get_CurFrame() == 38 ||
        Get_CurFrame() == 48 ||
        Get_CurFrame() == 58)
    {
        if (!m_bSkillCreate)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

            FORWARDMOVINGSKILLDESC desc;
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.5f;
            desc.fLimitDistance = 20.f;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);

            wstring strAttackType = NORMAL_ATTACK;

            if (Get_CurFrame() == 58)
                strAttackType = KNOCKDOWN_ATTACK;

            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * 1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType);
            
            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::skill_12100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_12100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}


void Boss_Mir_FSM::skill_13100()
{
    if (Get_CurFrame() == 30)
    {
        if (!m_bSkillCreate)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
                _float3::Up;

            FORWARDMOVINGSKILLDESC desc;
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.5f;
            desc.fLimitDistance = 30.f;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) +
                Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;

            for (_uint i = 0; i < 3; i++)
            {
                Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKDOWN_ATTACK);

                desc.vSkillDir = desc.vSkillDir + Get_Transform()->Get_State(Transform_State::RIGHT);
            }

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::skill_13100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_13100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_14100()
{
    if (Get_CurFrame() == 25)
    {
        if (!m_bSkillCreate)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

            FORWARDMOVINGSKILLDESC desc;
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.5f;
            desc.fLimitDistance = 20.f;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);

            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_ATTACK);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_ATTACK);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_ATTACK);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * 1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_ATTACK);

            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 80)
    {
        if (!m_bSkillCreate)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

            FORWARDMOVINGSKILLDESC desc;
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.5f;
            desc.fLimitDistance = 20.f;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);

            wstring strAttackType = NORMAL_ATTACK;

            if (Get_CurFrame() == 58)
                strAttackType = KNOCKDOWN_ATTACK;

            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * 1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType);

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Mir_FSM::skill_14100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_14100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_100000()
{
}

void Boss_Mir_FSM::skill_100000_Init()
{
}

void Boss_Mir_FSM::skill_100100()
{
}

void Boss_Mir_FSM::skill_100100_Init()
{
}

void Boss_Mir_FSM::skill_101100()
{
}

void Boss_Mir_FSM::skill_101100_Init()
{
}

void Boss_Mir_FSM::skill_100200()
{
}

void Boss_Mir_FSM::skill_100200_Init()
{
}

void Boss_Mir_FSM::CalCulate_PlayerDir()
{
    //몬스터가 플레이어바라보는방향
    _float4 vDir = _float4(0.f);
    vDir = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS) - Get_Transform()->Get_State(Transform_State::POS);
    vDir.y = 0.f; //y값 0
    vDir.Normalize();
       
    //노멀라이즈
    _float4 vDot = _float4(0.f);
    _float4 vCross = _float4(0.f);

    vDot = XMVector3Dot(Get_Transform()->Get_State(Transform_State::LOOK), vDir);
    vCross = XMVector3Cross(Get_Transform()->Get_State(Transform_State::LOOK), vDir);

    if (XMVectorGetX(vDot) >= 0.f) //앞 내적 양수면 정면
    {
        if (XMVectorGetY(vCross) < 0.f) //왼쪽 외적이 음수면 왼쪽 
        {
            m_eAttackDir = DIR::FORWARD_LEFT;
        }
        else //오른쪽  정면오른쪽
        {
            m_eAttackDir = DIR::FORWARD_RIGHT;
        }
    }
    else //뒤 내적음수면 뒤
    {
        if (XMVectorGetY(vCross) < 0.f) // 외적 음수면 왼쪽
        {
            m_eAttackDir = DIR::BACKWARD_LEFT;
        }
        else //오른쪽  외적 양수면 오른쪽
        {
            m_eAttackDir = DIR::BACKWARD_RIGHT;
        }
    }
}

void Boss_Mir_FSM::Add_Boss_Mir_Collider()
{
    shared_ptr<GameObject> attackCollider = make_shared<GameObject>();
    attackCollider->GetOrAddTransform();
    attackCollider->Add_Component(make_shared<SphereCollider>(1.f));
    attackCollider->Get_Collider()->Set_CollisionGroup(Monster_Attack);

    m_pAttackCollider = attackCollider;

    CUR_SCENE->Add_GameObject(m_pAttackCollider.lock());
    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);

    m_pAttackCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
    m_pAttackCollider.lock()->Set_Name(L"Boss_Mir_AttackCollider");
    m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());

    shared_ptr<GameObject> tailCollider = make_shared<GameObject>();
    tailCollider->GetOrAddTransform();
    tailCollider->Add_Component(make_shared<SphereCollider>(3.f));
    tailCollider->Get_Collider()->Set_CollisionGroup(Monster_Attack);

    CUR_SCENE->Add_GameObject(tailCollider);
    tailCollider->Get_Collider()->Set_Activate(false);

    tailCollider->Add_Component(make_shared<AttackColliderInfoScript>());
    tailCollider->Set_Name(L"Boss_Mir_TailCollider");
    tailCollider->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());

    m_pTailCollider = tailCollider;

    m_iMouseBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Bone057");
    m_iTailBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Bone040");
}

void Boss_Mir_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType)
{
    shared_ptr<GameObject> SkillCollider = make_shared<GameObject>();

    SkillCollider->GetOrAddTransform();
    SkillCollider->Get_Transform()->Set_State(Transform_State::POS, vPos);
    SkillCollider->Add_Component(make_shared<SphereCollider>(fSkillRange));
    SkillCollider->Get_Collider()->Set_CollisionGroup(Player_Skill);

    SkillCollider->Add_Component(make_shared<AttackColliderInfoScript>());
    SkillCollider->Get_Collider()->Set_Activate(true);
    SkillCollider->Get_Script<AttackColliderInfoScript>()->Set_SkillName(SkillType);
    SkillCollider->Set_Name(L"Boss_Mir_SkillCollider");
    SkillCollider->Add_Component(make_shared<ForwardMovingSkillScript>(desc));
    SkillCollider->Get_Script<ForwardMovingSkillScript>()->Init();

    CUR_SCENE->Add_GameObject(SkillCollider);
}
