#include "pch.h"
#include "MainCameraScript.h"
#include "Player_FSM.h"
#include "ModelAnimator.h"
#include "MotionTrailRenderer.h"
#include "Debug_CreateMotionTrail.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"
#include "CoolTimeCheckScript.h"
#include "ModelRenderer.h"
#include "DistortionRenderer.h"
#include <Utils.h>

Player_FSM::Player_FSM()
{
}

Player_FSM::~Player_FSM()
{
}

HRESULT Player_FSM::Init()
{
    auto animator = Get_Owner()->Get_Animator();
    if (animator)
    {
        animator->Set_CurrentAnim(L"b_idle", true, 1.f);
        m_eCurState = STATE::b_idle;
    }
    shared_ptr<GameObject> attackCollider = make_shared<GameObject>();
    attackCollider->GetOrAddTransform();
    attackCollider->Add_Component(make_shared<SphereCollider>(1.f));
    attackCollider->Get_Collider()->Set_CollisionGroup(Player_Attack);

    m_pAttackCollider = attackCollider;

    CUR_SCENE->Add_GameObject(m_pAttackCollider.lock());
    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
    
    m_pAttackCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
    m_pAttackCollider.lock()->Set_Name(L"Player_AttackCollider");
    m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(Get_Owner());
 
    m_pWeapon = CUR_SCENE->Get_GameObject(L"Weapon_Player");

    m_iCenterBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Center");
    m_iCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Cam");
    m_iSkillCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_SkillCam");
    m_iSkillBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"B_nose");

    m_pCamera = CUR_SCENE->Get_MainCamera();

    m_fEffectYOffSet = 1.2f;

    return S_OK;
}

void Player_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up);
    }

    Calculate_CamBoneMatrix();
}

void Player_FSM::State_Tick()
{
    State_Init();

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
    case STATE::skill_100100:
        skill_100100();
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
    case STATE::skill_200200:
        skill_200200();
        break;
    case STATE::skill_300100:
        skill_300100();
        break;
	case STATE::skill_300200:
		skill_300200();
		break;
    }
}

void Player_FSM::State_Init()
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
        case STATE::skill_100100:
            skill_100100_Init();
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
        case STATE::skill_200200:
            skill_200200_Init();
            break;
        case STATE::skill_300100:
            skill_300100_Init();
            break;
		case STATE::skill_300200:
			skill_300200_Init();
			break;
        }
        m_ePreState = m_eCurState;
    }
}

void Player_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Player_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
    if (pCollider->Get_Owner() == nullptr)
        return;

	if (!pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>())
		return;

    wstring strSkillName = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_SkillName();

    if (!m_bInvincible)
	{
		shared_ptr<GameObject> targetToLook = nullptr;
		// skillName에 _Skill 포함이면
		if (strSkillName.find(L"_Skill") != wstring::npos)
			targetToLook = pCollider->Get_Owner(); // Collider owner를 넘겨준다
		else // 아니면
			targetToLook = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_ColliderOwner(); // Collider를 만든 객체를 넘겨준다

        if (targetToLook == nullptr)
            return;

        Get_Hit(strSkillName, targetToLook);
    }
}

void Player_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Player_FSM::Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget)
{
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

void Player_FSM::AttackCollider_On(const wstring& skillname)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
    }
}

void Player_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    }
}

void Player_FSM::Set_State(_uint iIndex)
{
}

void Player_FSM::b_idle()
{
	_float3 vInputVector = Get_InputDirVector();

	if (KEYPUSH(KEY_TYPE::W) || KEYPUSH(KEY_TYPE::S) ||
		KEYPUSH(KEY_TYPE::A) || KEYPUSH(KEY_TYPE::D))
		m_eCurState = STATE::b_run_start;

	if (KEYTAP(KEY_TYPE::LBUTTON))
		m_eCurState = STATE::skill_1100;

    Use_Skill();
}

void Player_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::b_run_start()
{
    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
            m_eCurState = STATE::b_idle;
    }
    else
    {
        if (Is_AnimFinished())
            m_eCurState = STATE::b_run;

        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);


        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1100;

        Use_Skill();
    }
}

void Player_FSM::b_run_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_start", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();
    
    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::b_run()
{
    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
        {
            if (Get_CurFrame() % 2 == 0)
                m_eCurState = STATE::b_run_end_r;
            else
                m_eCurState = STATE::b_run_end_l;
        }
    }
    else
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (KEYPUSH(KEY_TYPE::LSHIFT))
    {
        if (Get_CurFrame() == 1)
            m_eCurState = STATE::b_sprint;
    }

    if (KEYTAP(KEY_TYPE::LBUTTON))
        m_eCurState = STATE::skill_1100;

    Use_Skill();
}

void Player_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::b_run_end_r()
{
    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector != _float3(0.f))
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;


    if (KEYTAP(KEY_TYPE::LBUTTON))
        m_eCurState = STATE::skill_1100;

    Use_Skill();

}

void Player_FSM::b_run_end_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_r", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::b_run_end_l()
{
    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector != _float3(0.f))
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;


    if (KEYTAP(KEY_TYPE::LBUTTON))
        m_eCurState = STATE::skill_1100;

    Use_Skill();
}

void Player_FSM::b_run_end_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_l", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::b_sprint()
{
    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
        {
            if (Get_CurFrame() % 2 == 0)
                m_eCurState = STATE::b_run_end_r;
            else
                m_eCurState = STATE::b_run_end_l;
        }
    }
    else
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (!KEYPUSH(KEY_TYPE::LSHIFT))
    {
        if (Get_CurFrame() < 1 || Get_CurFrame() > 13)
            m_eCurState = STATE::b_run;
    }

    if (KEYTAP(KEY_TYPE::LBUTTON))
        m_eCurState = STATE::skill_1100;

    Use_Skill();
}

void Player_FSM::b_sprint_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_sprint", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fSprintSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::b_walk()
{
}

void Player_FSM::b_walk_Init()
{
}

void Player_FSM::die()
{
}

void Player_FSM::die_Init()
{
}

void Player_FSM::airborne_start()
{
     

    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_end;
}

void Player_FSM::airborne_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_start", 0.2f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Player_FSM::airborne_end()
{
     

    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_up;
}

void Player_FSM::airborne_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Player_FSM::airborne_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Player_FSM::airborne_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.1f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Player_FSM::hit()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Player_FSM::hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"hit", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::knock_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end;
}

void Player_FSM::knock_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_start", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.8f);
}

void Player_FSM::knock_end()
{
    if (Get_CurFrame() < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void Player_FSM::knock_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.5f);
}

void Player_FSM::knock_end_loop()
{
    m_tKnockDownEndCoolTime.fAccTime += fDT;
    
    if (Get_CurFrame() > Get_FinalFrame() / 2)
        m_eCurState = STATE::knock_up;
}

void Player_FSM::knock_end_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_loop", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::knock_end_hit()
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

void Player_FSM::knock_end_hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_hit", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::knock_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Player_FSM::knock_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    m_tKnockDownEndCoolTime.fAccTime = 0.f;

    Get_Transform()->Set_Speed(m_fRunSpeed);
}

void Player_FSM::knockdown_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knockdown_end;
}

void Player_FSM::knockdown_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knockdown_start", 0.1f, false, 2.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed);
}

void Player_FSM::knockdown_end()
{
    if (Get_CurFrame() < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_up;
}

void Player_FSM::knockdown_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_CurrentAnim(L"knockdown_end", false, 1.5f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed * 0.5f);
}

void Player_FSM::skill_1100()
{
    // Collider Control
    if (Get_CurFrame() == 9)
    {
        AttackCollider_On(NORMAL_ATTACK);

		/*  if (!m_bAttackEffectCreate)
		  {
			  Add_Effect(L"Teoi_Slash2");
			  m_bAttackEffectCreate = true;
		  }*/
    }
    else if (Get_CurFrame() == 13)
        AttackCollider_Off();
    else
        m_bAttackEffectCreate = false;

    // Init때 초기
    // m_vKeyInputTargetDir -> 이름 헷갈릴 여지 있음 TargetDir 등으로 회전하고자 하는 목표를 뜻하는 단어로
    if (m_vKeyInputTargetDir != _float3(0.f))
        Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

    // 특정 프레임 이하에서는 마우스 누르면 다음 공격을 예약함.
	if (Get_CurFrame() < 26)
	{
		if (KEYTAP(KEY_TYPE::LBUTTON))
			m_bCanCombo = true;
	}
    // 애니메이션이 자연스러워지는  타이밍에 다음 공격을 실행함
	if (m_bCanCombo)
	{
		if (Get_CurFrame() > 24)
			m_eCurState = STATE::skill_1200;
	}

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    //Using Skill
    Use_Skill();
}

void Player_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vKeyInputTargetDir = _float3(0.f);
    m_vKeyInputTargetDir = Get_InputDirVector();

    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();

    m_bInvincible = false;
    m_bSuperArmor = false;

    _float4x4 matSetting = Get_Owner()->Get_Animator()->Get_CurAnimTransform(m_iCenterBoneIndex) * Utils::m_matPivot * Get_Transform()->Get_WorldMatrix();
    //test
    {
        shared_ptr<GameObject> obj = make_shared<GameObject>();
        obj->GetOrAddTransform()->Set_WorldMat(matSetting);
        obj->GetOrAddTransform()->Scaled(_float3(1.f));
        {
            shared_ptr<DistortionRenderer> renderer = make_shared<DistortionRenderer>(RESOURCES.Get<Shader>(L"Shader_Distortion.fx"));
            renderer->Set_Model(RESOURCES.Get<Model>(L"fxmesh_PC_Teo_Trail_Skill_1300_00"));
            shared_ptr<Material> material = make_shared<Material>();
            material->Set_TextureMap(RESOURCES.GetOrAddTexture(L"FX_Decal_03.tga", L"../Resources/Textures/Universal/FX_Decal_03.tga"), TextureMapType::DISTORTION);
            renderer->Set_Material(material);
            obj->Add_Component(renderer);
        }
        CUR_SCENE->Add_GameObject(obj);
    }
}

void Player_FSM::skill_1200()
{
    if (Get_CurFrame() == 4)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() > 8)
        AttackCollider_Off();
    
    if (m_vKeyInputTargetDir != _float3(0.f))
        Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

    if (Get_CurFrame() < 13)
    {
        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_bCanCombo = true;
    }

    if (m_bCanCombo)
    {
        if(Get_CurFrame() > 13)
            m_eCurState = STATE::skill_1300;
    }

    if (Is_AnimFinished())
    {
        m_bCanCombo = false;
        m_eCurState = STATE::b_idle;
    }

    Use_Skill();
}

void Player_FSM::skill_1200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1200", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vKeyInputTargetDir = _float3(0.f);
    m_vKeyInputTargetDir = Get_InputDirVector();

    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::skill_1300()
{
    if (Get_CurFrame() == 12)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 14)
        AttackCollider_Off();

    if (m_vKeyInputTargetDir != _float3(0.f))
        Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

	if (Get_CurFrame() < 27)
	{
		if (KEYTAP(KEY_TYPE::LBUTTON))
			m_bCanCombo = true;
	}

	if (m_bCanCombo)
	{
		if (Get_CurFrame() > 27)
			m_eCurState = STATE::skill_1400;
	}

    if (Is_AnimFinished())
    {
        m_bCanCombo = false;
        m_eCurState = STATE::b_idle;
    }

    Use_Skill();
}

void Player_FSM::skill_1300_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1300", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();

    m_vKeyInputTargetDir = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Player_FSM::skill_1400()
{
	if (Get_CurFrame() == 16)
		AttackCollider_On(NORMAL_ATTACK);
	else if (Get_CurFrame() == 20)
		AttackCollider_Off();

	if (m_vKeyInputTargetDir != _float3(0.f))
		Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

	if (Is_AnimFinished())
	{
		m_bCanCombo = false;
		m_eCurState = STATE::b_idle;
	}

	Use_Skill();
}

void Player_FSM::skill_1400_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_1400", 0.15f, false, m_fNormalAttack_AnimationSpeed);

	m_bCanCombo = false;

    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();

	m_vKeyInputTargetDir = Get_InputDirVector();

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Player_FSM::skill_91100()
{
    _float3 vInputVector = Get_InputDirVector();

    if (m_vKeyInputTargetDir != _float3(0.f))
        Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Player_FSM::skill_91100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_91100", 0.15f, false, m_fEvade_AnimationSpeed);

    m_bCanCombo = false;

    m_vKeyInputTargetDir = _float3(0.f);
    m_vKeyInputTargetDir = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Player_FSM::skill_93100()
{
    _float3 vInputVector = Get_InputDirVector();

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Player_FSM::skill_93100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_93100", 0.15f, false, m_fEvade_AnimationSpeed);

    m_bCanCombo = false;
    
    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Player_FSM::skill_100100()
{
	if (Get_CurFrame() == 38)
	{
		if (!m_bSkillCreate)
		{
			FORWARDMOVINGSKILLDESC desc;
			desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
			desc.fMoveSpeed = 50.f;
			desc.fLifeTime = 0.5f;
			desc.fLimitDistance = 25.f;

			_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 1.f + _float3::Up;
			Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL);

			m_bSkillCreate = true;
		}
	}
    else
    {
        m_bSkillCreate = false;
    }

	if (Get_CurFrame() < 38)
	{
		if (KEYTAP(KEY_TYPE::KEY_1))
			m_bCanCombo = true;
	}

	if (m_bCanCombo)
	{
		if (Get_CurFrame() > 40)
			m_eCurState = STATE::skill_100200;
	}

    if (m_vKeyInputTargetDir != _float3(0.f))
        Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

    if (Is_AnimFinished())
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }

    Use_Dash();
}

void Player_FSM::skill_100100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vKeyInputTargetDir = _float3(0.f);
    m_vKeyInputTargetDir = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Player_FSM::skill_100200()
{
	if (Get_CurFrame() == 8)
	{
		if (!m_bSkillCreate)
		{
			FORWARDMOVINGSKILLDESC desc;
			desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
			desc.fMoveSpeed = 0.f;
			desc.fLifeTime = 0.5f;
			desc.fLimitDistance = 0.f;

			_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);
			Create_ForwardMovingSkillCollider(vSkillPos, 5.f, desc, KNOCKBACK_ATTACK);

			m_bSkillCreate = true;
		}
	}
    else if (Get_CurFrame() == 26)
    {
        vector<shared_ptr<GameObject>> targetMonster;
        for (auto& obj : CUR_SCENE->Get_Objects())
        {
            if(obj->Get_ObjectGroup()!= OBJ_MONSTER)
                continue;

			if (!obj->Get_FSM())
				continue;

            _float3 vObjPos = obj->Get_Transform()->Get_State(Transform_State::POS).xyz();
            _float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

            _float3 vDir = vObjPos - vMyPos;
            vDir.y = 0.f;

            if(vDir.LengthSquared() > PLAYER_SKILL1_DIST * PLAYER_SKILL1_DIST)
                continue;

            vDir.Normalize();

            _float3 vLook = Get_Transform()->Get_State(Transform_State::LOOK).xyz();
            vLook.Normalize();

            if (vDir.Dot(vLook) > cosf(PLAYER_SKILL1_ANGLE * 0.5f))
                obj->Get_FSM()->Get_Hit(KNOCKDOWN_ATTACK, Get_Owner());
        }
    }
	else
	{
		m_bSkillCreate = false;
	}

	if (Get_CurFrame() < 29)
	{
		if (KEYTAP(KEY_TYPE::KEY_1))
			m_bCanCombo = true;
	}

	if (m_bCanCombo)
	{
		if (Get_CurFrame() > 27)
			m_eCurState = STATE::skill_100300;
	}

	if (m_vKeyInputTargetDir != _float3(0.f))
		Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_eCurState = STATE::b_idle;
	}

	Use_Dash();
}

void Player_FSM::skill_100200_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_100200", 0.15f, false, m_fSkillAttack_AnimationSpeed);

	m_bCanCombo = false;

	m_vKeyInputTargetDir = _float3(0.f);
	m_vKeyInputTargetDir = Get_InputDirVector();

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Player_FSM::skill_100300()
{
    if (Get_CurFrame() >= 10 && Get_CurFrame() <= 23)
    {
        if (Get_CurFrame() == 10)
            m_vCamStopPos = m_pCamera.lock()->Get_Transform()->Get_State(Transform_State::POS);

        if (!m_pCamera.expired())
        {
            _float4 vDir = m_vCamStopPos - m_vCenterBonePos;
            vDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCenterBonePos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.35f, vDir.xyz(), 6.f);
        }
    }
    else if (Get_CurFrame() >= 24 && Get_CurFrame() < 31)
    {
        if (!m_pCamera.expired())
        {
            _float4 vDir = m_vCamStopPos - m_vCenterBonePos;
            vDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCenterBonePos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.35f, vDir.xyz(), 7.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_SmoothReturn();
        }
    }


	if (Get_CurFrame() == 29)
	{
		vector<shared_ptr<GameObject>> targetMonster;
		for (auto& obj : CUR_SCENE->Get_Objects())
		{
			if (obj->Get_ObjectGroup() != OBJ_MONSTER)
				continue;

			_float3 vObjPos = obj->Get_Transform()->Get_State(Transform_State::POS).xyz();
			_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

			_float3 vDir = vObjPos - vMyPos;
			vDir.y = 0.f;

			if (vDir.LengthSquared() > PLAYER_SKILL1_DIST * PLAYER_SKILL1_DIST)
				continue;

			vDir.Normalize();

			_float3 vLook = Get_Transform()->Get_State(Transform_State::LOOK).xyz();
			vLook.Normalize();

			if (vDir.Dot(vLook) > cosf(PLAYER_SKILL1_ANGLE * 0.5f))
				targetMonster.push_back(obj);
		}

		for (auto& obj : targetMonster)
		{
			if (!obj->Get_FSM())
				continue;

			obj->Get_FSM()->Get_Hit(KNOCKDOWN_ATTACK, Get_Owner());
		}
	}
	else
	{
		m_bSkillCreate = false;
	}

	if (m_vKeyInputTargetDir != _float3(0.f))
		Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_eCurState = STATE::b_idle;
	}
}

void Player_FSM::skill_100300_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_100300", 0.15f, false, m_fSkillAttack_AnimationSpeed);

	m_bCanCombo = false;

	m_vKeyInputTargetDir = _float3(0.f);
	m_vKeyInputTargetDir = Get_InputDirVector();

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Player_FSM::skill_200100()
{
    if (Get_CurFrame() >= 15)
    {
        if (Get_CurFrame() == 15)
            m_vCamStopPos = m_pCamera.lock()->Get_Transform()->Get_State(Transform_State::POS);

        if (!m_pCamera.expired())
        {
            _float4 vDir = m_vCamStopPos - m_vCenterBonePos;
            vDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCenterBonePos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.35f, vDir.xyz(), 10.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_SmoothReturn();
        }
    }


	if (Get_CurFrame() >= 30)
	{
        m_fSkillCreateTimer += fDT;
        if (m_fSkillCreateTimer >= 0.3f)
        {
            m_fSkillCreateTimer = 0.f;
			FORWARDMOVINGSKILLDESC desc;
			desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
			desc.fMoveSpeed = 0.f;
			desc.fLifeTime = 0.3f;
			desc.fLimitDistance = 0.f;

			_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);
			Create_ForwardMovingSkillCollider(vSkillPos, 5.f, desc, NORMAL_ATTACK);

			Get_Owner()->Get_Animator()->Set_RenderState(false);
			m_pWeapon.lock()->Get_ModelRenderer()->Set_RenderState(false);
        }
	}

	if (m_vKeyInputTargetDir != _float3(0.f))
		Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

    if (Is_AnimFinished())
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::skill_200200;
    }
}

void Player_FSM::skill_200100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vKeyInputTargetDir = _float3(0.f);
    m_vKeyInputTargetDir = Get_InputDirVector();

    AttackCollider_On(NORMAL_ATTACK);

    m_bInvincible = true;
    m_bSuperArmor = true;
}

void Player_FSM::skill_200200()
{
    if (Get_CurFrame() >= 5)
	{
		shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();
        animator->Set_RenderState(true);
        m_pWeapon.lock()->Get_ModelRenderer()->Set_RenderState(true);
     }

    if (Get_CurFrame() == 22)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 0.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKBACK_SKILL);

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    if (m_vKeyInputTargetDir != _float3(0.f))
        Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

    if (Is_AnimFinished())
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }
}

void Player_FSM::skill_200200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_CurrentAnim(L"skill_200200", false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vKeyInputTargetDir = _float3(0.f);
    m_vKeyInputTargetDir = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = true;
}

void Player_FSM::skill_300100()
{

    if (Get_CurFrame() == 14)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 0.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);
            Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKBACK_SKILL);

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

	if (Get_CurFrame() < 43)
	{
		if (KEYTAP(KEY_TYPE::KEY_3))
			m_bCanCombo = true;
	}

	if (m_bCanCombo)
	{
		if (Get_CurFrame() > 44)
			m_eCurState = STATE::skill_300200;
	}

    if (m_vKeyInputTargetDir != _float3(0.f))
        Soft_Turn_ToInputDir(m_vKeyInputTargetDir, XM_PI * 5.f);

    if (Is_AnimFinished())
    {
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
        m_eCurState = STATE::b_idle;
    }
}

void Player_FSM::skill_300100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_300100", 0.15f, false, 1.3f);

    m_bCanCombo = false;

    m_vKeyInputTargetDir = _float3(0.f);
    m_vKeyInputTargetDir = Get_InputDirVector();

    AttackCollider_Off();
    m_bInvincible = true;
    m_bSuperArmor = true;
}

void Player_FSM::skill_300200()
{
    if (Get_CurFrame() < 91)
    {
        if (!m_pCamera.expired())
        {
            _float4 vDir = m_vCamStopPos - m_vCenterBonePos;
            vDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCenterBonePos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.35f, vDir.xyz(), 12.f);
        }
    }
    else
    {
        if (!m_pCamera.expired())
        {
            _float4 vDir = m_vCamStopPos - m_vCenterBonePos;
            vDir.Normalize();
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCenterBonePos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.35f, vDir.xyz(), 4.5f);
        }
    }
    
    if (Get_CurFrame() >= 5 && Get_CurFrame() < 57)
    {
        m_fSkillCreateTimer += fDT;
        if (m_fSkillCreateTimer >= 0.3f)
        {
            m_fSkillCreateTimer = 0.f;
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 0.3f;
            desc.fLimitDistance = 0.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);
            Create_ForwardMovingSkillCollider(vSkillPos, 5.f, desc, NORMAL_ATTACK);

        }
    }
    else if (Get_CurFrame() >= 57 && Get_CurFrame() < 75)
    {
        m_fSkillCreateTimer += fDT;
        if (m_fSkillCreateTimer >= 0.1f)
        {
            m_fSkillCreateTimer = 0.f;
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 0.1f;
            desc.fLimitDistance = 0.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);
            Create_ForwardMovingSkillCollider(vSkillPos, 5.f, desc, NORMAL_ATTACK);

        }
    }
    else if (Get_CurFrame() == 101)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 0.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);
            Create_ForwardMovingSkillCollider(vSkillPos, 3.f, desc, KNOCKBACK_SKILL);

            m_bSkillCreate = true;
        }
    }
    else m_bSkillCreate = false;

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Player_FSM::skill_300200_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_300200", 0.05f, false, 1.3f);

	m_bCanCombo = false;

	m_vKeyInputTargetDir = _float3(0.f);
	m_vKeyInputTargetDir = Get_InputDirVector();

	AttackCollider_Off();

	m_bInvincible = true;
	m_bSuperArmor = true;

    m_vCamStopPos = m_pCamera.lock()->Get_Transform()->Get_State(Transform_State::POS);    
}

void Player_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType)
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
    m_pSkillCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());
    m_pSkillCollider.lock()->Set_Name(L"Player_SkillCollider");
    m_pSkillCollider.lock()->Add_Component(make_shared<ForwardMovingSkillScript>(desc));
    m_pSkillCollider.lock()->Get_Script<ForwardMovingSkillScript>()->Init();

    CUR_SCENE->Add_GameObject(m_pSkillCollider.lock());
}

void Player_FSM::Use_Skill()
{
    if (KEYTAP(KEY_TYPE::KEY_1) && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL1))
        m_eCurState = STATE::skill_100100;
    else if (KEYTAP(KEY_TYPE::KEY_2) && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL2))
        m_eCurState = STATE::skill_200100;
    else if (KEYTAP(KEY_TYPE::KEY_3) && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL3))
        m_eCurState = STATE::skill_300100;
    else
        Use_Dash();
}

void Player_FSM::Use_Dash()
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
