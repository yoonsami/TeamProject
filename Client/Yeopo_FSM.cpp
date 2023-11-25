#include "pch.h"
#include "MainCameraScript.h"
#include "Yeopo_FSM.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"
#include "MotionTrailRenderer.h"
#include "Debug_CreateMotionTrail.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"
#include "YeopoHorse_FSM.h"
#include "CoolTimeCheckScript.h"
#include "CharacterController.h"

Yeopo_FSM::Yeopo_FSM()
{
}

Yeopo_FSM::~Yeopo_FSM()
{
}

HRESULT Yeopo_FSM::Init()
{
    if (!m_bInitialize)
    {
        auto animator = Get_Owner()->Get_Animator();
        if (animator)
        {
            animator->Set_CurrentAnim(L"b_idle", true, 1.f);
            m_eCurState = STATE::b_idle;
        }

        m_pCamera = CUR_SCENE->Get_MainCamera();

        m_iCenterBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Center");
        m_iCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Cam");
        m_iSkillCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_SkillCam");
        m_iSkillBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"D_Eye_Target");

        m_fNormalAttack_AnimationSpeed = 1.2f;
        m_fDetectRange = 5.f;

        m_bInitialize = true;
    }

   
    return S_OK;
}

void Yeopo_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
		m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 1.5f);
    }

}

void Yeopo_FSM::State_Tick()
{
    State_Init();

    m_iCurFrame = Get_CurFrame();

    switch (m_eCurState)
    {
    case STATE::b_idle:
        b_idle();
        break;
    case STATE::b_run_start:
        b_run_start();
        break;
    case STATE::b_run:
        b_run();
        break;
    case STATE::b_run_end_r:
        b_run_end_r();
        break;
    case STATE::b_run_end_l:
        b_run_end_l();
        break;
    case STATE::b_sprint:
        b_sprint();
        break;
    case STATE::b_walk:
        b_walk();
        break;
    case STATE::die:
        die();
        break;
    case STATE::airborne_start:
        airborne_start();
        break;
    case STATE::airborne_end:
        airborne_end();
        break;
    case STATE::airborne_up:
        airborne_up();
        break;
    case STATE::hit:
        hit();
        break;
    case STATE::knock_start:
        knock_start();
        break;
    case STATE::knock_end:
        knock_end();
        break;
    case STATE::knock_end_loop:
        knock_end_loop();
        break;
    case STATE::knock_end_hit:
        knock_end_hit();
        break;
    case STATE::knock_up:
        knock_up();
        break;
    case STATE::knockdown_start:
        knockdown_start();
        break;
    case STATE::knockdown_end:
        knockdown_end();
        break;
    case STATE::skill_1100:
        skill_1100();
        break;
    case STATE::skill_1200:
        skill_1200();
        break;
    case STATE::skill_1300:
        skill_1300();
        break;
    case STATE::skill_1400:
        skill_1400();
        break;
    case STATE::skill_91100:
        skill_91100();
        break;
    case STATE::skill_93100:
        skill_93100();
        break;
    case STATE::skill_100200:
        skill_100200();
        break;
    case STATE::skill_100300:
        skill_100300();
        break;
    case STATE::skill_200100:
        skill_200100();
        break;
    case STATE::skill_300100:
        skill_300100();
        break;
    case STATE::skill_300200:
        skill_300200();
        break;
    case STATE::skill_300300:
        skill_300300();
        break;
    case STATE::skill_300400:
        skill_300400();
        break;
    case STATE::skill_400100:
        skill_400100();
        break;
    case STATE::skill_501100:
        skill_501100();
        break;
    case STATE::SQ_RideHorse_Idle:
        SQ_RideHorse_Idle();
        break;
    case STATE::SQ_RideHorse_Run:
        SQ_RideHorse_Run();
        break;
    case STATE::SQ_RideHorse_Stop:
        SQ_RideHorse_Stop();
        break;
    case STATE::SQ_RideHorse_End:
        SQ_RideHorse_End();
        break;
    }

    if (!m_pGroupEffect.expired())
        m_pGroupEffect.lock()->Get_Transform()->Set_WorldMat(Get_Transform()->Get_WorldMatrix());


    if (m_iPreFrame != m_iCurFrame)
        m_iPreFrame = m_iCurFrame;
}

void Yeopo_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::b_idle:
            b_idle_Init();
            break;
        case STATE::b_run_start:
            b_run_start_Init();
            break;
        case STATE::b_run:
            b_run_Init();
            break;
        case STATE::b_run_end_r:
            b_run_end_r_Init();
            break;
        case STATE::b_run_end_l:
            b_run_end_l_Init();
            break;
        case STATE::b_sprint:
            b_sprint_Init();
            break;
        case STATE::b_walk:
            b_walk_Init();
            break;
        case STATE::die:
            die_Init();
            break;
        case STATE::airborne_start:
            airborne_start_Init();
            break;
        case STATE::airborne_end:
            airborne_end_Init();
            break;
        case STATE::airborne_up:
            airborne_up_Init();
            break;
        case STATE::hit:
            hit_Init();
            break;
        case STATE::knock_start:
            knock_start_Init();
            break;
        case STATE::knock_end:
            knock_end_Init();
            break;
        case STATE::knock_end_loop:
            knock_end_loop_Init();
            break;
        case STATE::knock_end_hit:
            knock_end_hit_Init();
            break;
        case STATE::knock_up:
            knock_up_Init();
            break;
        case STATE::knockdown_start:
            knockdown_start_Init();
            break;
        case STATE::knockdown_end:
            knockdown_end_Init();
            break;
        case STATE::skill_1100:
            skill_1100_Init();
            break;
        case STATE::skill_1200:
            skill_1200_Init();
            break;
        case STATE::skill_1300:
            skill_1300_Init();
            break;
        case STATE::skill_1400:
            skill_1400_Init();
            break;
        case STATE::skill_91100:
            skill_91100_Init();
            break;
        case STATE::skill_93100:
            skill_93100_Init();
            break;
        case STATE::skill_100200:
            skill_100200_Init();
            break;
        case STATE::skill_100300:
            skill_100300_Init();
            break;
        case STATE::skill_200100:
            skill_200100_Init();
            break;
        case STATE::skill_300100:
            skill_300100_Init();
            break;
        case STATE::skill_300200:
            skill_300200_Init();
            break;
        case STATE::skill_300300:
            skill_300300_Init();
            break;
        case STATE::skill_300400:
            skill_300400_Init();
            break;
        case STATE::skill_400100:
            skill_400100_Init();
            break;
        case STATE::skill_501100:
            skill_501100_Init();
            break;
        case STATE::SQ_RideHorse_Idle:
            SQ_RideHorse_Idle_Init();
            break;
        case STATE::SQ_RideHorse_Run:
            SQ_RideHorse_Run_Init();
            break;
        case STATE::SQ_RideHorse_Stop:
            SQ_RideHorse_Stop_Init();
            break;
        case STATE::SQ_RideHorse_End:
            SQ_RideHorse_End_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void Yeopo_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Yeopo_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
    if (pCollider->Get_Owner() == nullptr)
        return;

	if (!pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>())
		return;


    if (!m_bInvincible)
    {
        wstring strSkillName = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_SkillName();
        _float fAttackDamage = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_AttackDamage();

        
        shared_ptr<GameObject> targetToLook = nullptr;
		// skillName에 _Skill 포함이면
		if (strSkillName.find(L"_Skill") != wstring::npos)
			targetToLook = pCollider->Get_Owner(); // Collider owner를 넘겨준다
		else // 아니면
			targetToLook = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_ColliderOwner(); // Collider를 만든 객체를 넘겨준다

        if (targetToLook == nullptr)
            return;

		Get_Hit(strSkillName, fAttackDamage, targetToLook);
    }
}

void Yeopo_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Yeopo_FSM::Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget)
{
    //Calculate Damage 
    m_pOwner.lock()->Get_Hurt(fDamage);


	_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vOppositePos = pLookTarget->Get_Transform()->Get_State(Transform_State::POS).xyz();

	m_vHitDir = vOppositePos - vMyPos;
	m_vHitDir.y = 0.f;
	m_vHitDir.Normalize();

    if (skillname == NORMAL_ATTACK || skillname == NORMAL_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (m_eCurState == STATE::hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_loop)
                m_eCurState = STATE::knock_end_hit;
            else
                m_eCurState = STATE::hit;
        }
    }
    else if (skillname == KNOCKBACK_ATTACK || skillname == KNOCKBACK_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (m_eCurState == STATE::knock_end_hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_loop)
                m_eCurState = STATE::knock_end_hit;
            else
                m_eCurState = STATE::knock_start;
        }
    }
    else if (skillname == KNOCKDOWN_ATTACK || skillname == KNOCKDOWN_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (m_eCurState == STATE::knock_end_hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_loop)
                m_eCurState = STATE::knock_end_hit;
            else
                m_eCurState = STATE::knockdown_start;
        }
    }
    else if (skillname == AIRBORNE_ATTACK || skillname == AIRBORNE_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (m_eCurState == STATE::knock_end_hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_loop)
                m_eCurState = STATE::knock_end_hit;
            else
                m_eCurState = STATE::airborne_start;
        }
    }
}

void Yeopo_FSM::AttackCollider_On(const wstring& skillname, _float fAttackDamage)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(fAttackDamage);
    }
}

void Yeopo_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(0.f);
    }
}

void Yeopo_FSM::Set_State(_uint iIndex)
{
}

void Yeopo_FSM::b_idle()
{
    RidingCoolCheck();

    _float3 vInputVector = Get_InputDirVector();

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (KEYPUSH(KEY_TYPE::W) || KEYPUSH(KEY_TYPE::S) ||
            KEYPUSH(KEY_TYPE::A) || KEYPUSH(KEY_TYPE::D))
            m_eCurState = STATE::b_run_start;

        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1100;

        Use_Skill();

        if (KEYPUSH(KEY_TYPE::V))
        {
            if (!m_bRidingCoolCheck)
            {
                Create_Vehicle();
                m_eCurState = STATE::SQ_RideHorse_Idle;
            }
        }
    }
}

void Yeopo_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    if (m_ePreState == STATE::skill_400100)
        animator->Set_NextTweenAnim(L"b_idle", 0.3f, true, 1.f);
    else
        animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;

    if (m_ePreState == STATE::SQ_RideHorse_End)
        Get_Transform()->Set_State(Transform_State::POS, m_vRidingEndPos);
}

void Yeopo_FSM::b_run_start()
{
    RidingCoolCheck();

    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
            m_eCurState = STATE::b_run_end_l;
    }
    else
    {
        if (Is_AnimFinished())
            m_eCurState = STATE::b_run;

        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

        if (!g_bIsCanMouseMove && !g_bCutScene)
        {
            if (KEYTAP(KEY_TYPE::LBUTTON))
                m_eCurState = STATE::skill_1100;

            Use_Skill();
        }
    }
}

void Yeopo_FSM::b_run_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_start", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::b_run()
{
    RidingCoolCheck();

    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
        {
            if (m_iCurFrame % 2 == 0)
                m_eCurState = STATE::b_run_end_r;
            else
                m_eCurState = STATE::b_run_end_l;
        }
    }
    else
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (g_bIsCanMouseMove || g_bCutScene)
    {
        if (m_iCurFrame % 2 == 0)
            m_eCurState = STATE::b_run_end_r;
        else
            m_eCurState = STATE::b_run_end_l;
    }

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (KEYPUSH(KEY_TYPE::LSHIFT))
        {
            if ((m_iCurFrame == 1))
                m_eCurState = STATE::b_sprint;
        }

        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1100;

        Use_Skill();
    }
}

void Yeopo_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::b_run_end_r()
{
    RidingCoolCheck();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector != _float3(0.f))
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1100;

        Use_Skill();
    }
}

void Yeopo_FSM::b_run_end_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_r", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::b_run_end_l()
{
    RidingCoolCheck();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector != _float3(0.f))
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1100;

        Use_Skill();
    }
}

void Yeopo_FSM::b_run_end_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_l", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::b_sprint()
{
    RidingCoolCheck();

    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
        {
            if (m_iCurFrame % 2 == 0)
                m_eCurState = STATE::b_run_end_r;
            else
                m_eCurState = STATE::b_run_end_l;
        }
    }
    else
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (g_bIsCanMouseMove || g_bCutScene)
    {
        if (m_iCurFrame % 2 == 0)
            m_eCurState = STATE::b_run_end_r;
        else
            m_eCurState = STATE::b_run_end_l;
    }

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (!KEYPUSH(KEY_TYPE::LSHIFT))
        {
            if (m_iCurFrame < 1 || m_iCurFrame > 13)
                m_eCurState = STATE::b_run;
        }

        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1100;

        Use_Skill();
    }
}

void Yeopo_FSM::b_sprint_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_sprint", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fSprintSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::b_walk()
{
}

void Yeopo_FSM::b_walk_Init()
{
}

void Yeopo_FSM::die()
{
}

void Yeopo_FSM::die_Init()
{
}

void Yeopo_FSM::airborne_start()
{
    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();

    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_end;
}

void Yeopo_FSM::airborne_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_start", 0.2f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_CharacterController()->Add_Velocity(6.f);
}

void Yeopo_FSM::airborne_end()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_up;
}

void Yeopo_FSM::airborne_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::airborne_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Yeopo_FSM::airborne_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.1f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::hit()
{
    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();

    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Yeopo_FSM::hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"hit", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::knock_start()
{
    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();

    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end;
}

void Yeopo_FSM::knock_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_start", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.8f);
}

void Yeopo_FSM::knock_end()
{
    if (m_iCurFrame < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void Yeopo_FSM::knock_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.5f);
}

void Yeopo_FSM::knock_end_loop()
{
    m_tKnockDownEndCoolTime.fAccTime += fDT;

    if (m_iCurFrame > Get_FinalFrame() / 2)
        m_eCurState = STATE::knock_up;
}

void Yeopo_FSM::knock_end_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_loop", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::knock_end_hit()
{
    m_tKnockDownEndCoolTime.fAccTime += fDT;

    if (Is_AnimFinished())
    {
        if (m_tKnockDownEndCoolTime.fAccTime >= m_tKnockDownEndCoolTime.fCoolTime)
            m_eCurState = STATE::knock_up;
        else
            m_eCurState = STATE::knock_end_loop;
    }
}

void Yeopo_FSM::knock_end_hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_hit", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::knock_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Yeopo_FSM::knock_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    m_tKnockDownEndCoolTime.fAccTime = 0.f;

    Get_Transform()->Set_Speed(m_fRunSpeed);
}

void Yeopo_FSM::knockdown_start()
{
    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();

    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knockdown_end;
}

void Yeopo_FSM::knockdown_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knockdown_start", 0.1f, false, 2.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed);
}

void Yeopo_FSM::knockdown_end()
{
    if (m_iCurFrame < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_up;
}

void Yeopo_FSM::knockdown_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_CurrentAnim(L"knockdown_end", false, 1.5f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed * 0.5f);
}

void Yeopo_FSM::skill_1100()
{
    Look_DirToTarget();

    if (m_iCurFrame == 9)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 19)
        AttackCollider_Off();

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (Check_Combo(15, KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1200;

        Use_Skill();
    }

    if (Get_FinalFrame() - m_iCurFrame < 6)
    {
        if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
            m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        
        m_eCurState = STATE::b_idle;
    }


}

void Yeopo_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::skill_1200()
{
    Look_DirToTarget();

    if (m_iCurFrame == 10)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 17)
        AttackCollider_Off();

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (Check_Combo(15, KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1300;

        Use_Skill();
    }

    if (Get_FinalFrame() - m_iCurFrame < 7)
    {
        if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
            m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        
        m_bCanCombo = false;
        m_eCurState = STATE::b_idle;
    }


}

void Yeopo_FSM::skill_1200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1200", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(DEFAULT);
    }

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::skill_1300()
{
    if (m_iCurFrame == 8)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 13)
        AttackCollider_Off();
    else if (m_iCurFrame == 16)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 23)
        AttackCollider_Off();
    else if (m_iCurFrame == 28)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 36)
        AttackCollider_Off();
    else if (m_iCurFrame == 41)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 46)
        AttackCollider_Off();

    Look_DirToTarget();

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (Check_Combo(45, KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1400;

        Use_Skill();
    }

    if (Get_FinalFrame() - m_iCurFrame < 6)
    {
        if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
            m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        
        m_bCanCombo = false;
        m_eCurState = STATE::b_idle;
    }


}

void Yeopo_FSM::skill_1300_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1300", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(DEFAULT);
    }

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Yeopo_FSM::skill_1400()
{
    if (m_iCurFrame == 7)
        AttackCollider_On(KNOCKBACK_ATTACK, 10.f);
    else if (m_iCurFrame == 14)
        AttackCollider_Off();
    else if (m_iCurFrame == 19)
        AttackCollider_On(KNOCKDOWN_ATTACK, 10.f);
    else if (m_iCurFrame == 36)
        AttackCollider_Off();


    Look_DirToTarget();

    if (Get_FinalFrame() - m_iCurFrame < 6)
    {
        if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
            m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        
        m_bCanCombo = false;
        m_eCurState = STATE::b_idle;
    }

    if (!g_bIsCanMouseMove && !g_bCutScene)
        Use_Skill();
}

void Yeopo_FSM::skill_1400_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1400", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(DEFAULT);
    }

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::skill_91100()
{
    _float3 vInputVector = Get_InputDirVector();

    Look_DirToTarget();

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (m_iCurFrame >= 23)
    {
        if (vInputVector != _float3(0.f))
            m_eCurState = STATE::b_run;
    }
}

void Yeopo_FSM::skill_91100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_91100", 0.15f, false, m_fEvade_AnimationSpeed);

    m_bCanCombo = false;

    m_vDirToTarget = _float3(0.f);
    m_vDirToTarget = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Yeopo_FSM::skill_93100()
{
    _float3 vInputVector = Get_InputDirVector();

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (m_iCurFrame >= 23)
    {
        if (vInputVector != _float3(0.f))
            m_eCurState = STATE::b_run;
    }
}

void Yeopo_FSM::skill_93100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_93100", 0.15f, false, m_fEvade_AnimationSpeed);

    m_bCanCombo = false;

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Yeopo_FSM::skill_100200()
{
    if (Init_CurFrame(18))
    {
		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 35.f;
		desc.fLifeTime = 1.f;
		desc.fLimitDistance = 10.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
		Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL, 10.f);
    }
    else if (Init_CurFrame(30))
    {
		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::LOOK) * 15.f +
			_float3::Up;

		_float4 vSkillDir = (Get_Transform()->Get_State(Transform_State::POS) - vSkillPos);
		vSkillDir.y = 0.f;

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = vSkillDir.Normalize();
		desc.fMoveSpeed = 28.f;
		desc.fLifeTime = 1.f;
		desc.fLimitDistance = 13.f;

		Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL, 10.f);
    }

    Look_DirToTarget();

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (Check_Combo(56, KEY_TYPE::KEY_1))
            m_eCurState = STATE::skill_100300;
    }

    if (Get_FinalFrame() - m_iCurFrame < 8)
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }

}

void Yeopo_FSM::skill_100200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100200", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::skill_100300()
{
    if (Init_CurFrame(24))
    {
		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 1.f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK);
		Create_ForwardMovingSkillCollider(vSkillPos, 2.5f, desc, AIRBORNE_ATTACK, 10.f);
    }
    
    Look_DirToTarget();

    if (Is_AnimFinished())
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }

    if (!g_bIsCanMouseMove && !g_bCutScene)
        Use_Dash();
}

void Yeopo_FSM::skill_100300_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100300", 0.15f, false, m_fSkillAttack_AnimationSpeed);
    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(SKILL1);

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::skill_200100()
{
    if (Init_CurFrame(10))
	{
		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 1.f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);
		Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKDOWN_ATTACK, 10.f);
    }
    

    Look_DirToTarget();

    if (Get_FinalFrame() - m_iCurFrame < 10)
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }

    if (!g_bIsCanMouseMove && !g_bCutScene)
        Use_Dash();
}

void Yeopo_FSM::skill_200100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::skill_300100()
{
    if (m_iCurFrame == 25)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 30)
        AttackCollider_Off();
    else if (m_iCurFrame == 40)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 46)
        AttackCollider_Off();
    else if (m_iCurFrame == 55)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 64)
        AttackCollider_Off();

    Look_DirToTarget();

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (Check_Combo(65, KEY_TYPE::KEY_3))
            m_eCurState = STATE::skill_300200;

        Use_Dash();
    }

    if (Get_FinalFrame() - m_iCurFrame <15)
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }


}

void Yeopo_FSM::skill_300100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_300100", 0.15f, false, 1.3f);

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::skill_300200()
{
    if (m_iCurFrame == 9)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 12)
        AttackCollider_Off();
    else if (m_iCurFrame == 15)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 20)
        AttackCollider_Off();

    Look_DirToTarget();

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (Check_Combo(25, KEY_TYPE::KEY_3))
            m_eCurState = STATE::skill_300300;

        Use_Dash();
    }

    if (Get_FinalFrame() - m_iCurFrame < 15)
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }
}

void Yeopo_FSM::skill_300200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_300200", 0.15f, false, 1.3f);
    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(SKILL3);

    m_bCanCombo = false;
    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::skill_300300()
{
    if (m_iCurFrame == 13)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 18)
        AttackCollider_Off();
    else if (m_iCurFrame == 21)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 30)
        AttackCollider_Off();

    Look_DirToTarget();

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (Check_Combo(30, KEY_TYPE::KEY_3))
            m_eCurState = STATE::skill_300400;

        Use_Dash();
    }

    if (Get_FinalFrame() - m_iCurFrame < 15)
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }


}

void Yeopo_FSM::skill_300300_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_300300", 0.15f, false, 1.3f);
    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(SKILL3);

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::skill_300400()
{
    if (m_iCurFrame == 31)
        AttackCollider_On(KNOCKBACK_ATTACK, 10.f);
    else if (m_iCurFrame == 36)
        AttackCollider_Off();
    
    Look_DirToTarget();

    if (Get_FinalFrame() - m_iCurFrame < 15)
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }

    if (!g_bIsCanMouseMove && !g_bCutScene)
        Use_Dash();
}

void Yeopo_FSM::skill_300400_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_300400", 0.15f, false, 1.3f);
    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(SKILL3);

    m_bCanCombo = false;
    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Yeopo_FSM::skill_400100()
{
    Calculate_CamBoneMatrix();

    if (m_iCurFrame < 84)
    {
        if (!m_pCamera.expired())
        {
            _float4 vSkillCamPos = m_vSkillCamBonePos;
            vSkillCamPos.y += 2.f;
            
            _float4 vDir = m_vCamBonePos - vSkillCamPos;
            vDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(vSkillCamPos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.5f, vDir.xyz() * -1.f, 10.f);
        }
    }
    else
    {
        if (!m_pCamera.expired())
        {
            _float4 vSkillCamPos = m_vSkillCamBonePos;
            vSkillCamPos.y += 2.f;
            _float4 vDir = m_vCamBonePos - vSkillCamPos;
            vDir.Normalize();

            //m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(vSkillCamPos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.5f, vDir.xyz() * -1.f, 12.f);
        }
    }

    //Look_DirToTarget();
    //Skill End
    if (m_iCurFrame == 135)
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }

}

void Yeopo_FSM::skill_400100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_400100", 0.15f, false, 1.f);

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = true;

    Calculate_CamBoneMatrix();

    Create_Vehicle();
 
    Set_VehicleState((_uint)YeopoHorse_FSM::STATE::skill_400100_fx);
}

void Yeopo_FSM::skill_501100()
{
    if (m_iCurFrame == 12)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 16)
        AttackCollider_Off();
    else if (m_iCurFrame == 17)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 19)
        AttackCollider_Off();
    else if (m_iCurFrame == 21)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 23)
        AttackCollider_Off();
    else if (m_iCurFrame == 25)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 28)
        AttackCollider_Off();
    else if (m_iCurFrame == 30)
        AttackCollider_On(NORMAL_ATTACK, 10.f);
    else if (m_iCurFrame == 35)
        AttackCollider_Off();
    else if (m_iCurFrame == 52)
        AttackCollider_On(KNOCKDOWN_ATTACK, 10.f);
    else if (m_iCurFrame == 55)
        AttackCollider_Off();
    

    Look_DirToTarget();

    if (Is_AnimFinished())
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }

    if (!g_bIsCanMouseMove && !g_bCutScene)
        Use_Dash();
}

void Yeopo_FSM::skill_501100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_501100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    Set_DirToTargetOrInput(OBJ_MONSTER);

    AttackCollider_Off();


    m_bInvincible = false;
    m_bSuperArmor = true;
}


void Yeopo_FSM::SQ_RideHorse_Idle()
{
    _float3 vInputVector = Get_InputDirVector();

    if (!g_bIsCanMouseMove && !g_bCutScene)
    {
        if (KEYPUSH(KEY_TYPE::W) || KEYPUSH(KEY_TYPE::S) ||
            KEYPUSH(KEY_TYPE::A) || KEYPUSH(KEY_TYPE::D))
            m_eCurState = STATE::SQ_RideHorse_Run;

        if (KEYTAP(KEY_TYPE::V))
            m_eCurState = STATE::SQ_RideHorse_End;
    }
}

void Yeopo_FSM::SQ_RideHorse_Idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    if (m_ePreState == STATE::b_idle)
        animator->Set_CurrentAnim(L"SQ_RideHorse_Idle", true, 1.f);
    else
        animator->Set_NextTweenAnim(L"SQ_RideHorse_Idle", 0.15f, true, 1.f);

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;

    Get_Transform()->Set_Speed(m_fRidingSpeed);

    Set_VehicleState((_uint)YeopoHorse_FSM::STATE::SQ_RideHorse_Idle);
}

void Yeopo_FSM::SQ_RideHorse_Run()
{
    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRidingEndDelay.fAccTime += fDT;

        if (m_tRidingEndDelay.fAccTime >= m_tRidingEndDelay.fCoolTime)
            m_eCurState = STATE::SQ_RideHorse_Stop;
    }
    else
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);
}

void Yeopo_FSM::SQ_RideHorse_Run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"SQ_RideHorse_Run", 0.1f, true, 1.f);

    m_bInvincible = true;
    m_bSuperArmor = false;

    Get_Transform()->Set_Speed(m_fRidingSpeed);

    Set_VehicleState((_uint)YeopoHorse_FSM::STATE::SQ_RideHorse_Run);
}

void Yeopo_FSM::SQ_RideHorse_Stop()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::SQ_RideHorse_Idle;
}

void Yeopo_FSM::SQ_RideHorse_Stop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"SQ_RideHorse_Stop", 0.1f, false, 1.f);

    m_vDirToTarget = _float3(0.f);
    m_vDirToTarget = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;

    Get_Transform()->Set_Speed(m_fRidingSpeed * 0.8f);

    m_tRidingEndDelay.fAccTime = 0.f;

    Set_VehicleState((_uint)YeopoHorse_FSM::STATE::SQ_RideHorse_Stop);
}

void Yeopo_FSM::SQ_RideHorse_End()
{
    if (Is_AnimFinished())
    {
        matBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iSkillBoneIndex) *
            _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

        m_vRidingEndPos = _float4{ matBoneMatrix.Translation().x, matBoneMatrix.Translation().y, matBoneMatrix.Translation().z , 1.f };
        m_vRidingEndPos.y = Get_Transform()->Get_State(Transform_State::POS).y;
        Get_Transform()->Set_State(Transform_State::POS, m_vRidingEndPos);
        m_eCurState = STATE::b_idle;
    }
}

void Yeopo_FSM::SQ_RideHorse_End_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"SQ_RideHorse_End", 0.1f, false, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    Set_VehicleState((_uint)YeopoHorse_FSM::STATE::SQ_RideHorse_End);
}

void Yeopo_FSM::RidingCoolCheck()
{
    if (m_bRidingCoolCheck)
    {
        m_tRidingDelay.fAccTime += fDT;

        if (m_tRidingDelay.fAccTime >= m_tRidingDelay.fCoolTime)
        {
            m_bRidingCoolCheck = false;
            m_tRidingDelay.fAccTime = 0.f;
        }
    }
}

void Yeopo_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType, _float fAttackDamage)
{
    shared_ptr<GameObject> SkillCollider = make_shared<GameObject>();

    m_pSkillCollider = SkillCollider;

    m_pSkillCollider.lock()->GetOrAddTransform();
    m_pSkillCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, vPos);
    
    auto pSphereCollider = make_shared<SphereCollider>(fSkillRange);
    pSphereCollider->Set_CenterPos(_float3{ vPos.x,vPos.y, vPos.z });
    m_pSkillCollider.lock()->Add_Component(pSphereCollider);

    m_pSkillCollider.lock()->Get_Collider()->Set_CollisionGroup(Player_Skill);

    m_pSkillCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
    m_pSkillCollider.lock()->Get_Collider()->Set_Activate(true);
    m_pSkillCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(SkillType);
    m_pSkillCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(fAttackDamage);
    m_pSkillCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());
    m_pSkillCollider.lock()->Set_Name(L"Player_SkillCollider");
    m_pSkillCollider.lock()->Add_Component(make_shared<ForwardMovingSkillScript>(desc));
    m_pSkillCollider.lock()->Get_Script<ForwardMovingSkillScript>()->Init();

    CUR_SCENE->Add_GameObject(m_pSkillCollider.lock());
}

void Yeopo_FSM::Create_Vehicle()
{
    //Add. Player's Vehicle
    shared_ptr<GameObject> ObjVehicle = make_shared<GameObject>();

    ObjVehicle->Add_Component(make_shared<Transform>());
    {
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> renderer = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Yeopo_Horse");
            renderer->Set_Model(model);
        }

        ObjVehicle->Add_Component(renderer);

    }
    ObjVehicle->Add_Component(make_shared<YeopoHorse_FSM>());
    ObjVehicle->Get_FSM()->Set_Target(m_pOwner.lock());
    ObjVehicle->Get_FSM()->Init();
    ObjVehicle->Set_Name(L"Yeopo_RedHorse");
    
    CUR_SCENE->Add_GameObject(ObjVehicle);

    m_pVehicle = ObjVehicle;
}

void Yeopo_FSM::Set_VehicleState(_uint iAnimindex)
{
    if (!m_pVehicle.expired())
        m_pVehicle.lock()->Get_FSM()->Set_State(iAnimindex);

}

void Yeopo_FSM::Use_Skill()
{
    if (KEYTAP(KEY_TYPE::KEY_1) && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL1))
        m_eCurState = STATE::skill_100200;
    else if (KEYTAP(KEY_TYPE::KEY_2) && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL2))
        m_eCurState = STATE::skill_200100;
    else if (KEYTAP(KEY_TYPE::KEY_3) && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL3))
        m_eCurState = STATE::skill_300100;
    else if (KEYTAP(KEY_TYPE::KEY_4) && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL4))
        m_eCurState = STATE::skill_400100;
    else if (KEYTAP(KEY_TYPE::KEY_5) && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL5))
        m_eCurState = STATE::skill_501100;
    else
        Use_Dash();
}

void Yeopo_FSM::Use_Dash()
{
    if (KEYTAP(KEY_TYPE::SPACE))
    {
        if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(EVADE))
        {
            m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();

            _float3 vInputVector = Get_InputDirVector();
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

