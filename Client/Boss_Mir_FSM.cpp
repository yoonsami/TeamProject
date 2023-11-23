#include "pch.h"
#include "Boss_Mir_FSM.h"
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

HRESULT Boss_Mir_FSM::Init()
{
    if (!m_bInitialize)
    {
        auto animator = Get_Owner()->Get_Animator();
        if (animator)
        {
            animator->Set_CurrentAnim(L"3D_ui_start", false, 1.f);
            m_eCurState = STATE::First_Meet;
        }

        m_fDetectRange = 10.f;

        Add_Boss_Mir_Collider();

        m_iHeadBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Head");
        m_iMouseBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Bone057");
        m_iTailBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Bone040");

        HeadBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iHeadBoneIndex) *
            _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

        m_vHeadCamPos = m_vHeadBonePos + (Get_Transform()->Get_State(Transform_State::LOOK) * 10.f);

        m_pCamera = CUR_SCENE->Get_MainCamera();

        m_fRunSpeed = 4.f;
        m_fKnockDownSpeed = 4.f;

        m_fDetectRange = 28.f;

        if (!m_pTarget.expired())
        {
            _float3 vLook = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
            vLook.y = 0.f;
            Get_Transform()->Set_LookDir(vLook);
        }
        
        Setting_DragonBall();



        m_bInitialize = true;
    }

    return S_OK;
}

void Boss_Mir_FSM::Tick()
{
    {
        HeadBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iHeadBoneIndex) *
            _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

        m_vHeadBonePos = _float4{ HeadBoneMatrix.Translation().x, HeadBoneMatrix.Translation().y, HeadBoneMatrix.Translation().z , 1.f };
        m_vHeadCamPos = m_vHeadBonePos + (Get_Transform()->Get_State(Transform_State::LOOK) * 10.f);
    }

    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 6.f + _float3::Up);
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

    m_iCurFrame = Get_CurFrame();

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
    case STATE::skill_Assault:
        skill_Assault();
        break;
    case STATE::skill_Return:
        skill_Return();
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
    case STATE::skill_200000:
        skill_200000();
        break;
    case STATE::skill_200100:
        skill_200100();
        break;
    }

    if (!m_pGroupEffect.expired())
        m_pGroupEffect.lock()->Get_Transform()->Set_WorldMat(Get_Transform()->Get_WorldMatrix());

    if (m_iPreFrame != m_iCurFrame)
        m_iPreFrame = m_iCurFrame;
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
        case STATE::skill_Assault:
            skill_Assault_Init();
            break;
        case STATE::skill_Return:
            skill_Return_Init();
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
        case STATE::skill_200000:
            skill_200000_Init();
            break;
        case STATE::skill_200100:
            skill_200100_Init();
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

void Boss_Mir_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Boss_Mir_FSM::Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget)
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
        if (m_bCounter)
        {
            if (CounterAttackCheck(90.f))
            {
                Create_CounterMotionTrail();
                m_eCurState = STATE::groggy_start;
            }
        }
    }
    else if (skillname == KNOCKBACK_ATTACK || skillname == KNOCKBACK_SKILL)
    {
        if (m_bCounter)
        {
            if (CounterAttackCheck(90.f))
            {
                Create_CounterMotionTrail();
                m_eCurState = STATE::groggy_start;
            }
        }
    }
    else if (skillname == KNOCKDOWN_ATTACK || skillname == KNOCKDOWN_SKILL)
    {
        if (m_bCounter)
        {
			if (CounterAttackCheck(90.f))
			{
				Create_CounterMotionTrail();
				m_eCurState = STATE::groggy_start;
			}
        }
    }
    else if (skillname == AIRBORNE_ATTACK || skillname == AIRBORNE_SKILL)
    {
        if (m_bCounter)
        {
			if (CounterAttackCheck(90.f))
			{
				Create_CounterMotionTrail();
				m_eCurState = STATE::groggy_start;
			}
        }
    }

}

void Boss_Mir_FSM::AttackCollider_On(const wstring& skillname, _float fAttackDamage)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(fAttackDamage);
    }
}

void Boss_Mir_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(0.f);
    }
}

void Boss_Mir_FSM::Set_State(_uint iIndex)
{
    m_eCurState = (STATE)iIndex;
}

void Boss_Mir_FSM::First_Meet()
{
    if (Target_In_DetectRange())
        m_bDetected = true;

    
    if (m_bDetected)
    {
        m_pOwner.lock()->Get_Animator()->Set_AnimState(false);
    
        if (Get_CurFrame() >= 1 && Get_CurFrame() < 90)
        {
            if (!m_pCamera.expired())
            {
                m_fIntroCamDistance = CamDistanceLerp(20.f, 10.f, m_fCamRatio);
                
                m_fCamRatio += fDT;

                if (m_fCamRatio >= 1.f)
                    m_fCamRatio = 1.f;

                m_vHeadCamDir = m_pCamera.lock()->Get_Transform()->Get_State(Transform_State::POS) - m_vHeadBonePos.xyz();
                m_vHeadCamDir.Normalize();

                m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(0.8f);
                m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vHeadBonePos.xyz());
                m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(1.f, m_vHeadCamDir, m_fIntroCamDistance);
                
            }
        }
        else if (Get_CurFrame() >= 90)
        {
            if (!m_pCamera.expired())
            {
                m_pCamera.lock()->Get_Transform()->Set_State(Transform_State::POS, m_vHeadCamPos);

                m_vHeadCamDir = m_vHeadBonePos.xyz() - m_vHeadCamPos.xyz();
                m_vHeadCamDir.Normalize();
                
                m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(0.5f);
                m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vHeadBonePos.xyz());
                m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(1.f, m_vHeadCamDir * -1.f, 10.f);
            }
        }
    }


    //if (!m_bDetected)
    //    Reset_Frame();

    if (Is_AnimFinished())
        m_eCurState = STATE::sq_Intro2;
}

void Boss_Mir_FSM::First_Meet_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"3D_ui_start", 0.1f, false, 1.5f);

    m_bInvincible = true;

    animator->Set_AnimState(true);

    m_fCamRatio = 0.f;
}

void Boss_Mir_FSM::sq_Intro()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::sq_Intro2;
}

void Boss_Mir_FSM::sq_Intro_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"sq_Intro", 0.1f, false, 1.5f);

    m_bInvincible = true;
}

void Boss_Mir_FSM::sq_Intro2()
{
    if (Get_CurFrame() > 5)
    {
        if (!m_pCamera.expired())
        {
            m_vHeadCamDir = m_vHeadBonePos.xyz() - m_vHeadCamPos.xyz();
            m_vHeadCamDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(0.5f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vHeadBonePos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(1.f, m_vHeadCamDir * -1.f, 10.f);
        }
    }

    if (Is_AnimFinished())
        //m_eCurState = STATE::b_idle;
        m_eCurState = STATE::First_Meet;
    
}

void Boss_Mir_FSM::sq_Intro2_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"sq_Intro2", 0.1f, false, 1.f);

    m_bInvincible = true;
}

void Boss_Mir_FSM::b_idle()
{
    m_tAttackCoolTime.fAccTime += fDT;
    
    if (m_eCurPhase == PHASE::PHASE1)
        Create_Meteor();

    if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
    {
        //Phase1 Assault and Return
        if (m_eCurPhase == PHASE::PHASE1)
        {
            CalCulate_PlayerDir();

            if (!m_bTurnMotion)
                m_eCurState = STATE::skill_Assault;
            else
            {
                if (m_iPhaseOne_TurnCnt == 0)
                {
                    m_iPhaseOne_TurnCnt++;

                    if (m_eAttackDir == DIR::FORWARD_LEFT)
                    {
                        m_eCurState = STATE::turn_l;
                    }
                    else if (m_eAttackDir == DIR::BACKWARD_LEFT)
                    {
                        m_eCurState = STATE::turn_l;
                        m_fTurnSpeed = XM_PI * 1.2f;
                    }
                    else if (m_eAttackDir == DIR::FORWARD_RIGHT)
                    {
                        m_eCurState = STATE::turn_r;
                    }
                    else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
                    {
                        m_eCurState = STATE::turn_r;
                        m_fTurnSpeed = XM_PI * 1.2f;
                    }
                }
                else
                {
                    m_eCurState = STATE::skill_Assault;
                }
            }
        }
        else
            Set_AttackPattern();
    }
}

void Boss_Mir_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.3f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    m_vTurnVector = _float3(0.f);

    m_bInvincible = false;

    m_tAttackCoolTime.fAccTime = 0.f;
    m_tBreathCoolTime.fAccTime = 0.f;

    AttackCollider_Off();
    TailAttackCollider_Off();

    if (m_eCurPhase == PHASE::PHASE1)
    {
        if (!m_bPhaseOneEmissive)
        {
            for (auto& material : Get_Owner()->Get_Model()->Get_Materials())
            {
                material->Get_MaterialDesc().emissive = Color(1.f, 0.1f, 0.1f, 1.f);
            }

            m_bPhaseOneEmissive = true;

            //Add_BossHp UI
            if (!m_pOwner.expired())
            {
                auto pScript = make_shared<UIBossHpBar>(BOSS::MIR);
                m_pOwner.lock()->Add_Component(pScript);
                pScript->Init();
            }
        }
    }
}

void Boss_Mir_FSM::turn_l()
{
    if (Get_CurFrame() > 9 && Get_CurFrame() < 28)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);
    }

    if (Is_AnimFinished())
    {
        m_fTurnSpeed = XM_PI * 0.5f;
        m_eCurState = STATE::b_idle;
    }
}

void Boss_Mir_FSM::turn_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"turn_l", 0.2f, false, 1.f);
    
    m_bTurnMotion = false;
}

void Boss_Mir_FSM::turn_r()
{
    if (Get_CurFrame() > 9 && Get_CurFrame() < 28)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);
    }

    if (Is_AnimFinished())
    {
        m_fTurnSpeed = XM_PI * 0.5f;
        m_eCurState = STATE::b_idle;
    }

}

void Boss_Mir_FSM::turn_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"turn_r", 0.2f, false, 1.f);

    m_bTurnMotion = false;
}

void Boss_Mir_FSM::die()
{
}

void Boss_Mir_FSM::die_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"SQ_Dead", 0.1f, false, 1.f);

    AttackCollider_Off();
    TailAttackCollider_Off();
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

    AttackCollider_Off();
    TailAttackCollider_Off();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_tBreathCoolTime.fAccTime = 0.f;
    m_bCounter = false;

    if (m_eCurPhase == PHASE::PHASE1)
    {
        m_iCrashCnt++;

        //In Phase1 Crash3 -> Phase 2 Start
        if (m_iCrashCnt == 3)
            m_eCurPhase = PHASE::PHASE2;

        if (m_eCurPhase == PHASE::PHASE2)
        {
            if (m_bPhaseOneEmissive)
            {
                for (auto& material : Get_Owner()->Get_Model()->Get_Materials())
                    material->Get_MaterialDesc().emissive = Color(0.f, 0.f, 0.f, 1.f);

                m_bPhaseOneEmissive = false;
            }

        }
    }   
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
    {
        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 1.f;
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
                m_fTurnSpeed = XM_PI * 1.f;
            }
        }
    }
}

void Boss_Mir_FSM::groggy_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"groggy_end", 0.1f, false, 1.f);
}

void Boss_Mir_FSM::skill_Assault()
{
    Create_Meteor();

    if (Get_CurFrame() <= 30)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 2.f);
    }

    if (Init_CurFrame(75))
    {
        AttackCollider_On(KNOCKBACK_ATTACK, 10.f);
    }
    else if (Init_CurFrame(92))
    {
        AttackCollider_Off();
    }

    if (Is_AnimFinished())
        m_eCurState = STATE::skill_Return;
}

void Boss_Mir_FSM::skill_Assault_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();
    
    animator->Set_NextTweenAnim(L"skill_2100", 0.1f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_iPhaseOne_TurnCnt = 0;
}

void Boss_Mir_FSM::skill_Return()
{
    Create_Meteor();

    if (Is_AnimFinished())
    {
        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 1.f;
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
                m_fTurnSpeed = XM_PI * 1.f;
            }
        }
    }
}

void Boss_Mir_FSM::skill_Return_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_5100", 0.1f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
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
        if (m_iPreFrame != m_iCurFrame)
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
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, NORMAL_ATTACK, 10.f);
            else
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKBACK_ATTACK, 10.f);
        }
    }

    if (Is_AnimFinished())
    {
        _uint iRan = rand() % 2;

        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_4100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_3100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
        }
    }
}

void Boss_Mir_FSM::SQ_SBRin_Roar_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"SQ_SBRin_Roar", 0.1f, false, 1.f);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_bTurnMotion = false;
}

void Boss_Mir_FSM::skill_1100()
{
    if (Get_CurFrame() == 46 ||
        Get_CurFrame() == 56 ||
        Get_CurFrame() == 66 ||
        Get_CurFrame() == 76)
    {
        if (m_iPreFrame != m_iCurFrame)
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
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, NORMAL_ATTACK, 10.f);
            else
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKBACK_ATTACK, 10.f);
        }
    }

    if (Is_AnimFinished())
    {
        _uint iRan = rand() % 2;
        
        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_4100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_3100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
        }
    }
    
}

void Boss_Mir_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;

}

void Boss_Mir_FSM::skill_2100()
{
    if (Get_CurFrame() == 55)
    {
        m_pOwner.lock()->Get_Animator()->Set_AnimationSpeed(m_fNormalAttack_AnimationSpeed / 4.f);

        for (auto& material : Get_Owner()->Get_Model()->Get_Materials())
        {
            material->Get_MaterialDesc().emissive = Color(0.05f, 0.2f, 1.f, 1.f);
        }
    }
    else if (Get_CurFrame() == 60)
    {
        m_bCounter = true;
    }
    else if (Get_CurFrame() == 68)
    {
        m_pOwner.lock()->Get_Animator()->Set_AnimationSpeed(m_fNormalAttack_AnimationSpeed);
        m_bCounter = false;
		for (auto& material : Get_Owner()->Get_Model()->Get_Materials())
		{
			material->Get_MaterialDesc().emissive = Color(0.f, 0.f, 0.f, 1.f);
		}
    }
    else if (Get_CurFrame() == 70)
        AttackCollider_On(KNOCKBACK_ATTACK, 10.f);
    else if (Get_CurFrame() == 93)
        AttackCollider_Off();

    if (Is_AnimFinished())
    {

        _float4 vPos = Get_Transform()->Get_State(Transform_State::POS);
        _uint iRan = rand() % 2;

        CalCulate_PlayerDir();

        if (iRan == 0)
        {
            if (!m_bTurnMotion)
                m_eCurState = STATE::b_idle;
            else
            {
                if (m_eAttackDir == DIR::FORWARD_LEFT)
                {
                    m_eCurState = STATE::turn_l;
                }
                else if (m_eAttackDir == DIR::BACKWARD_LEFT)
                {
                    m_eCurState = STATE::skill_4100;
                }
                else if (m_eAttackDir == DIR::FORWARD_RIGHT)
                {
                    m_eCurState = STATE::turn_r;
                }
                else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
                {
                    m_eCurState = STATE::skill_3100;
                }
            }
        }
        else
        {
            m_eCurState = STATE::skill_5100;
        }
    }
}

void Boss_Mir_FSM::skill_2100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_2100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_bCounter = false;
}

void Boss_Mir_FSM::skill_3100()
{
    if (Get_CurFrame() == 80)
        TailAttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 98)
        TailAttackCollider_Off();

    if (Is_AnimFinished())
    {
        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 0.7f;
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
                m_fTurnSpeed = XM_PI * 0.7f;
            }
        }
    }
   
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
        TailAttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 108)
        TailAttackCollider_Off();

    if (Is_AnimFinished())
    {
        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 0.7f;
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
                m_fTurnSpeed = XM_PI * 0.7f;
            }
        }
    }
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
    {
        CalCulate_PlayerDir();
        
        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 1.f;
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
                m_fTurnSpeed = XM_PI * 1.f;
            }
        }
    }
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
        Get_CurFrame() == 86 ||
        Get_CurFrame() == 106)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            if (!m_pTarget.expired())
            {
                _float4 vPlayerPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS);

                FORWARDMOVINGSKILLDESC desc;
                desc.vSkillDir = _float3{ 0.f,-1.f,0.f };
                desc.fMoveSpeed = 10.f;
                desc.fLifeTime = 1.f;
                desc.fLimitDistance = 20.f;

                for (_uint i = 0; i < 4; i++)
                {
                    _float fOffSetX = ((rand() * 2 / _float(RAND_MAX) - 1) * (rand() % 10 + 3));
                    _float fOffSetZ = ((rand() * 2 / _float(RAND_MAX) - 1) * (rand() % 10 + 3));

                    _float4 vSkillPos = vPlayerPos + _float4{ fOffSetX, 10.f, fOffSetZ, 0.f };

                    Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, AIRBORNE_ATTACK, 10.f);
                }
            }
        }
    }

    if (Is_AnimFinished())
    {
        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 0.7f;
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
                m_fTurnSpeed = XM_PI * 0.7f;
            }
        }
    }
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
        if (m_iPreFrame != m_iCurFrame)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 10.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

            Create_ForwardMovingSkillCollider(vSkillPos, 8.f, desc, KNOCKDOWN_ATTACK, 10.f);
        }
    }


    if (Is_AnimFinished())
    {
        _uint iRan = rand() % 2;

        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_4100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_3100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
        }
    }
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
        if (m_iPreFrame != m_iCurFrame)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

            FORWARDMOVINGSKILLDESC desc;
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.5f;
            desc.fLimitDistance = 20.f;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);

            wstring strAttackType = NORMAL_ATTACK;

            if (Get_CurFrame() == 58)
                strAttackType = KNOCKBACK_ATTACK;

            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType, 10.f);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType, 10.f);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType, 10.f);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * 1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType, 10.f);
        }
    }

    if (Is_AnimFinished())
    {
        _uint iRan = rand() % 2;

        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_4100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_3100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
        }
    }
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
        if (m_iPreFrame != m_iCurFrame)
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
                Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_ATTACK, 10.f);

                desc.vSkillDir = desc.vSkillDir + Get_Transform()->Get_State(Transform_State::RIGHT);
            }
        }
    }

    if (Is_AnimFinished())
    {
        _uint iRan = rand() % 2;

        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_4100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_3100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
        }
    }
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
        if (m_iPreFrame != m_iCurFrame)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

            FORWARDMOVINGSKILLDESC desc;
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.5f;
            desc.fLimitDistance = 20.f;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);

            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_ATTACK, 10.f);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_ATTACK, 10.f);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_ATTACK, 10.f);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * 1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_ATTACK, 10.f);
        }
    }
    else if (Get_CurFrame() == 80)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

            FORWARDMOVINGSKILLDESC desc;
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.5f;
            desc.fLimitDistance = 20.f;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);

            wstring strAttackType = NORMAL_ATTACK;

            if (Get_CurFrame() == 58)
                strAttackType = KNOCKBACK_ATTACK;

            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType, 10.f);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType, 10.f);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType, 10.f);

            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * 1.f;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, strAttackType, 10.f);
        }
    }

    if (Is_AnimFinished())
    {
        _uint iRan = rand() % 2;

        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_4100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                if (iRan == 0)
                    m_eCurState = STATE::skill_3100;
                else
                {
                    m_eCurState = STATE::turn_l;
                    m_fTurnSpeed = XM_PI * 0.7f;
                }
            }
        }
    }
}

void Boss_Mir_FSM::skill_14100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_14100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_100000()
{
    if (Get_CurFrame() > 90)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 2.5f);
    }

    if (Is_AnimFinished())
        m_eCurState = STATE::skill_100100;
}

void Boss_Mir_FSM::skill_100000_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100000", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_100100()
{
    if (Get_CurFrame() == 1)
    {
        m_pOwner.lock()->Get_Animator()->Set_AnimationSpeed(m_fNormalAttack_AnimationSpeed / 4.f);

        for (auto& material : Get_Owner()->Get_Model()->Get_Materials())
        {
            material->Get_MaterialDesc().emissive = Color(0.05f, 0.2f, 1.f, 1.f);
        }
    }
    else if (Get_CurFrame() == 4)
    {
        m_bCounter = true;
    }
    else if (Get_CurFrame() == 9)
    {
        m_pOwner.lock()->Get_Animator()->Set_AnimationSpeed(m_fNormalAttack_AnimationSpeed * 2.f);
        m_bCounter = false;
        
        for (auto& material : Get_Owner()->Get_Model()->Get_Materials())
        {
            material->Get_MaterialDesc().emissive = Color(0.f, 0.f, 0.f, 1.f);
        }
    }
    else if (Get_CurFrame() == 19 ||
             Get_CurFrame() == 29 ||
             Get_CurFrame() == 39 ||
             Get_CurFrame() == 49 ||
             Get_CurFrame() == 59 ||
             Get_CurFrame() == 69 ||
             Get_CurFrame() == 79 ||
             Get_CurFrame() == 84)
    {
        if (m_iPreFrame != m_iCurFrame)
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

            if (Get_CurFrame() != 84)
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, NORMAL_ATTACK, 10.f);
            else
                Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKDOWN_ATTACK, 10.f);
        }
    }
    else if (Get_CurFrame() == 90)
        m_pOwner.lock()->Get_Animator()->Set_AnimationSpeed(m_fNormalAttack_AnimationSpeed);
    


    if (Is_AnimFinished())
    {
        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 0.7f;
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 0.7f;   
            }
        }
    }
}

void Boss_Mir_FSM::skill_100100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_200000()
{
    if (Get_CurFrame() > 15)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 2.f);
    }

    if (Is_AnimFinished())
        m_eCurState = STATE::skill_200100;
}

void Boss_Mir_FSM::skill_200000_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200000", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

void Boss_Mir_FSM::skill_200100()
{
    m_tBreathCoolTime.fAccTime += fDT;

    if (Get_CurFrame() == 10)
    {
        m_pOwner.lock()->Get_Animator()->Set_AnimationSpeed(m_fNormalAttack_AnimationSpeed / 4.f);

        for (auto& material : Get_Owner()->Get_Model()->Get_Materials())
        {
            material->Get_MaterialDesc().emissive = Color(0.05f, 0.2f, 1.f, 1.f);
        }
    }
    else if (Get_CurFrame() == 15)
    {
        m_bCounter = true;
    }
    else if (Get_CurFrame() == 20)
    {
        m_pOwner.lock()->Get_Animator()->Set_AnimationSpeed(m_fNormalAttack_AnimationSpeed / 2.f);
        m_bCounter = false;

        for (auto& material : Get_Owner()->Get_Model()->Get_Materials())
        {
            material->Get_MaterialDesc().emissive = Color(0.f, 0.f, 0.f, 1.f);
        }
    }
    else if (Get_CurFrame() > 33 && Get_CurFrame() < 100)
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
        
            Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKBACK_ATTACK, 10.f);
        }
    }
    
    if (Is_AnimFinished())
    {
        CalCulate_PlayerDir();

        if (!m_bTurnMotion)
            m_eCurState = STATE::b_idle;
        else
        {
            if (m_eAttackDir == DIR::FORWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
            }
            else if (m_eAttackDir == DIR::BACKWARD_LEFT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 0.7f;   
            }
            else if (m_eAttackDir == DIR::FORWARD_RIGHT)
            {
                m_eCurState = STATE::turn_r;
            }
            else if (m_eAttackDir == DIR::BACKWARD_RIGHT)
            {
                m_eCurState = STATE::turn_l;
                m_fTurnSpeed = XM_PI * 0.7f;   
            }
        }
    }
}

void Boss_Mir_FSM::skill_200100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;
}

Boss_Mir_FSM::DIR Boss_Mir_FSM::CalCulate_PlayerDir()
{
    _float4 vDir = _float4(0.f);
    vDir = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS) - Get_Transform()->Get_State(Transform_State::POS);
    vDir.y = 0.f; 
    vDir.Normalize();

    _float4 vDot = _float4(0.f);
    _float4 vCross = _float4(0.f);

    vDot = XMVector3Dot(Get_Transform()->Get_State(Transform_State::LOOK), vDir);
    vCross = XMVector3Cross(Get_Transform()->Get_State(Transform_State::LOOK), vDir);

    _float fStandardTurnDegree = 0.f;
    
    if (m_eCurPhase == PHASE::PHASE1)
        fStandardTurnDegree = 20.f;
    else
        fStandardTurnDegree = 20.f;
    

    if (XMVectorGetX(vDot) >= 0.f)
    {
        if (XMVectorGetY(vCross) < 0.f)
        {
            m_eAttackDir = DIR::FORWARD_LEFT;

            if (XMVectorGetX(vDot) < cosf(XMConvertToRadians(fStandardTurnDegree)))
                m_bTurnMotion = true;
        }
        else
        {
            m_eAttackDir = DIR::FORWARD_RIGHT;

            if (XMVectorGetX(vDot) < cosf(XMConvertToRadians(fStandardTurnDegree)))
                m_bTurnMotion = true;
        }
    }
    else
    {
        m_bTurnMotion = true;

        if (XMVectorGetY(vCross) < 0.f)
        {
            m_eAttackDir = DIR::BACKWARD_LEFT;
        }
        else
        {
            m_eAttackDir = DIR::BACKWARD_RIGHT;
        }
    }
    

    return m_eAttackDir;
}


void Boss_Mir_FSM::Add_Boss_Mir_Collider()
{
    shared_ptr<GameObject> attackCollider = make_shared<GameObject>();
    attackCollider->GetOrAddTransform();
    attackCollider->Add_Component(make_shared<SphereCollider>(3.f));
    attackCollider->Get_Collider()->Set_CollisionGroup(Monster_Attack);

    m_pAttackCollider = attackCollider;

    CUR_SCENE->Add_GameObject(m_pAttackCollider.lock());
    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);

    m_pAttackCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
    m_pAttackCollider.lock()->Set_Name(L"Boss_Mir_AttackCollider");
    m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());

    shared_ptr<GameObject> tailCollider = make_shared<GameObject>();
    tailCollider->GetOrAddTransform();
    tailCollider->Add_Component(make_shared<SphereCollider>(6.f));
    tailCollider->Get_Collider()->Set_CollisionGroup(Monster_Attack);

    CUR_SCENE->Add_GameObject(tailCollider);
    tailCollider->Get_Collider()->Set_Activate(false);

    tailCollider->Add_Component(make_shared<AttackColliderInfoScript>());
    tailCollider->Set_Name(L"Boss_Mir_TailCollider");
    tailCollider->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());

    m_pTailCollider = tailCollider;
}

void Boss_Mir_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType, _float fAttackDamage)
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

    CUR_SCENE->Add_GameObject(SkillCollider);
}

void Boss_Mir_FSM::Create_CounterMotionTrail()
{
	for (auto& material : Get_Owner()->Get_Model()->Get_Materials())
	{
		material->Get_MaterialDesc().emissive = Color(0.f, 0.f, 0.f, 1.f);
	}
    m_pOwner.lock()->Get_Script<CounterMotionTrailScript>()->Init();
}

void Boss_Mir_FSM::Create_Meteor()
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

void Boss_Mir_FSM::Set_AttackPattern()
{
    _uint iRan = rand() % 10;

    while (true)
    {
        if (iRan == m_iPreAttack)
            iRan = rand() % 10;
        else
            break;
    }

    if (iRan == 0)
    {
        m_eCurState = STATE::SQ_SBRin_Roar;
        m_iPreAttack = 0;
    }
    else if (iRan == 1)
    {
        m_eCurState = STATE::skill_1100;
        m_iPreAttack = 1;
    }
    else if (iRan == 2)
    {
        m_eCurState = STATE::skill_2100;
        m_iPreAttack = 2;
    }
    else if (iRan == 3)
    {
        m_eCurState = STATE::skill_9100;
        m_iPreAttack = 3;
    }
    else if (iRan == 4)
    {
        m_eCurState = STATE::skill_11100;
        m_iPreAttack = 4;
    }
    else if (iRan == 5)
    {
        m_eCurState = STATE::skill_12100;
        m_iPreAttack = 5;
    }
    else if (iRan == 6)
    {
        m_eCurState = STATE::skill_13100;
        m_iPreAttack = 6;
    }
    else if (iRan == 7)
    {
        m_eCurState = STATE::skill_14100;
        m_iPreAttack = 7;
    }
    else if (iRan == 8)
    {
        m_eCurState = STATE::skill_100000;
        m_iPreAttack = 8;
    }
    else if (iRan == 9)
    {
        m_eCurState = STATE::skill_200000;
        m_iPreAttack = 9;
    }
}

void Boss_Mir_FSM::Setting_DragonBall()
{
    _uint iDragonBallIndex = 4;
    for (_uint i = 0; i < 3; i++)
    {
        wstring DragonBallName = L"Anim_P_R02_DecoBall_00_Idle-1"; // TODO 14,15,16
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

void Boss_Mir_FSM::TailAttackCollider_On(const wstring& skillname)
{
    m_pTailCollider.lock()->Get_Collider()->Set_Activate(true);
    m_pTailCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
}

void Boss_Mir_FSM::TailAttackCollider_Off()
{
    m_pTailCollider.lock()->Get_Collider()->Set_Activate(false);
    m_pTailCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
}

_float Boss_Mir_FSM::CamDistanceLerp(_float fStart, _float fEnd, _float fRatio)
{
    return fStart * (1.f - fRatio) + fEnd * fRatio;
}



