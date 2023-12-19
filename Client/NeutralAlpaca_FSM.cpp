#include "pch.h"
#include "NeutralAlpaca_FSM.h"
#include "ModelAnimator.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "MainCameraScript.h"
#include "UiDamageCreate.h"
#include "ObjectDissolve.h"
#include "UiMonsterHp.h"
#include "CharacterController.h"
#include <MathUtils.h>
#include "Model.h"
#include "MotionTrailDisappear.h"
#include "MotionTrailRenderer.h"

HRESULT NeutralAlpaca_FSM::Init()
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

        m_fRunSpeed = 7.f;
        m_fKnockBackSpeed = 4.f;
        m_fKnockDownSpeed = 4.f;

        m_fNormalAttack_AnimationSpeed = 1.f;
        m_fSkillAttack_AnimationSpeed = 1.f;
        m_fDetectRange = 6.f;

        m_bInitialize = true;
    }

    m_fMySoundDistance = 4.f;
    m_fVoiceVolume = 0.5f;

	shared_ptr<GameObject> attackCollider = make_shared<GameObject>();
	attackCollider->GetOrAddTransform();
	attackCollider->Add_Component(make_shared<SphereCollider>(1.f));
	attackCollider->Get_Collider()->Set_CollisionGroup(Monster_Attack);

	m_pAttackCollider = attackCollider;

	EVENTMGR.Create_Object(m_pAttackCollider.lock());
	m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);

	m_pAttackCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
	m_pAttackCollider.lock()->Set_Name(L"Alpaca_AttackCollider");
	m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());

    m_pTarget = GET_PLAYER;

    return S_OK;
}

void NeutralAlpaca_FSM::Tick()
{
    DeadCheck();

    State_Tick();

	if (!m_pAttackCollider.expired())
	{
		//m_pAttack transform set forward
		m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up);
	}
}

void NeutralAlpaca_FSM::State_Tick()
{
    State_Init();

    m_iCurFrame = Get_CurFrame();
    Recovery_Color();
    switch (m_eCurState)
    {
    case STATE::b_idle:
        b_idle();
        break;
    case STATE::b_run:
        b_run();
        break;
    case STATE::n_run:
        n_run();
        break;
    case STATE::die_01:
        die_01();
        break;
    case STATE::die_02:
        die_02();
        break;
    case STATE::gaze_b:
        gaze_b();
        break;
    case STATE::gaze_f:
        gaze_f();
        break;
    case STATE::gaze_l:
        gaze_l();
        break;
    case STATE::gaze_r:
        gaze_r();
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
    case STATE::skill_2100:
        skill_2100();
        break;
    case STATE::skill_3100:
        skill_3100();
        break;
    case STATE::attack_run:
        attack_run();
        break;
    }

    Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

    if (m_iPreFrame != m_iCurFrame)
        m_iPreFrame = m_iCurFrame;
}

void NeutralAlpaca_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::b_idle:
            b_idle_Init();
            break;
        case STATE::b_run:
            b_run_Init();
            break;
        case STATE::n_run:
            n_run_Init();
            break;
        case STATE::die_01:
            die_01_Init();
            break;
        case STATE::die_02:
            die_02_Init();
            break;
        case STATE::gaze_b:
            gaze_b_Init();
            break;
        case STATE::gaze_f:
            gaze_f_Init();
            break;
        case STATE::gaze_l:
            gaze_l_Init();
            break;
        case STATE::gaze_r:
            gaze_r_Init();
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
        case STATE::skill_2100:
            skill_2100_Init();
            break;
        case STATE::skill_3100:
            skill_3100_Init();
            break;
		case STATE::attack_run:
			attack_run_Init();
			break;
        }
        m_ePreState = m_eCurState;
    }
}

void NeutralAlpaca_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void NeutralAlpaca_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
    if (pCollider->Get_Owner() == nullptr)
        return;

	

    if (pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>())
	{
		wstring hitEffectTag = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_HitEffectTag();
		if (!hitEffectTag.empty())
		{
			_float3 random = MathUtils::Get_RandomVector(_float3(0, 0, XM_PI / 6.f), _float3(0, 0, -XM_PI / 6.f));

			Quaternion q = Quaternion::CreateFromYawPitchRoll(random.x, random.y, random.z);

			Add_Effect(hitEffectTag, nullptr, _float4x4::CreateFromQuaternion(q));
		}
         
		if (!m_bInvincible)
		{
			wstring strSkillName = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_SkillName();
			_float fAttackDamage = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_AttackDamage();
            ElementType eType = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_AttackElementType();
			shared_ptr<GameObject> targetToLook = nullptr;

			if (strSkillName.find(L"_Skill") != wstring::npos)
				targetToLook = pCollider->Get_Owner();
			else
				targetToLook = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_ColliderOwner();

			if (targetToLook == nullptr)
				return;

			Get_Hit(strSkillName, fAttackDamage, targetToLook,eType);
		}
	}

    if (pCollider->Get_Owner()->Get_Name() == L"Alpaca_Exit")
    {
        if (!m_pAttackCollider.expired())
            EVENTMGR.Delete_Object(m_pAttackCollider.lock());
	
        auto script = make_shared<ObjectDissolve>(1.f);
		Get_Owner()->Add_Component(script);
		script->Init();
        m_bIsDead = true;
    }
}

void NeutralAlpaca_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void NeutralAlpaca_FSM::Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget, _uint iElementType)
{
	CUR_SCENE->Get_UI(L"UI_Damage_Controller")->Get_Script<UiDamageCreate>()->Create_Damage_Font(Get_Owner(), fDamage, ElementType(iElementType));

    m_pOwner.lock()->Get_Hurt(fDamage);

    auto pScript = m_pOwner.lock()->Get_Script<UiMonsterHp>();
    if (nullptr == pScript)
    {
        pScript = make_shared<UiMonsterHp>();
        m_pOwner.lock()->Add_Component(pScript);
        pScript->Init();
    }

    m_bDetected = true;
    m_pCamera.lock()->Get_Script<MainCameraScript>()->ShakeCamera(0.1f, 0.05f);
    _float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
    _float3 vOppositePos = pLookTarget->Get_Transform()->Get_State(Transform_State::POS).xyz();

    m_vHitDir = vOppositePos - vMyPos;
    m_vHitDir.y = 0.f;
    m_vHitDir.Normalize();
	Set_HitColor();
    if(!m_bCanHitCounter)
	{
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

                SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
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

                SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
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

                SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
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

                SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
			}
		}
	}
    else
    {
        m_bAngry = false;
		Get_Owner()->Set_Instancing(true);
		Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f);
		Get_Owner()->Get_Animator()->Get_RenderParamDesc().vec4Params[1] = Color(0.f);
		m_bCanHitCounter = false;

		shared_ptr<GameObject> motionTrail = make_shared<GameObject>();
		motionTrail->GetOrAddTransform()->Set_WorldMat(Get_Transform()->Get_WorldMatrix());
		motionTrail->Add_Component(make_shared<MotionTrailRenderer>(RESOURCES.Get<Shader>(L"Shader_Model.fx"), Get_Owner()->Get_Animator()->Get_TweenDesc(), Get_Owner()->Get_Animator()->Get_Model()));
		motionTrail->Add_Component(make_shared<MotionTrailDisappear>(2.f, Color(0.2f, 0.2f, 2.f, 1.f)));
		motionTrail->Get_Script<MotionTrailDisappear>()->Init();
		EVENTMGR.Create_Object(motionTrail);
        m_bRealEducated = true;

        m_eCurState = STATE::knockdown_start;
    }
}

void NeutralAlpaca_FSM::Set_State(_uint iIndex)
{
}

void NeutralAlpaca_FSM::b_idle()
{
    //if (Init_CurFrame(40))
    //    SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);


    if (!m_bDetected)
	{
		CalCulate_PatrolTime();

		if (m_bPatrolMove)
		{
			m_eCurState = STATE::n_run;
		}
		if (Target_In_DetectRange())
			m_bDetected = true;
	}

    if (m_bDetected)
        m_eCurState = STATE::b_run;


}

void NeutralAlpaca_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    m_tPatrolMoveCool.fAccTime = 0.f;
    m_vTurnVector = _float3(0.f);

    m_bSuperArmor = false;
}

void NeutralAlpaca_FSM::b_run()
{
    if (Init_CurFrame(8))
        SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    _float3 vTargetPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
    _float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

    vTargetPos = 2.f * vMyPos - vTargetPos;

    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(vTargetPos, XM_PI * 5.f);

    Get_Transform()->Go_Straight();

	_bool bFlag = false;

	_float fGap = (Get_Transform()->Get_State(Transform_State::POS).xyz() -
		m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz()).LengthSquared();

	if (fGap <= pow(m_fDetectRange + 2.f,2.f))
		bFlag = true;

	m_bDetected = bFlag;

	if (!m_bDetected)
		m_eCurState = STATE::b_idle;

    if(!m_bAngry && CUR_SCENE->g_sceneFlag >= 16 && !m_bRealEducated)
	{
		_float randFloat = MathUtils::Get_RandomFloat(0.f, 100.f);
		if (randFloat < 0.5f)
		{
            m_pTarget = CUR_SCENE->Get_Player();
            Set_Gaze();
			m_bAngry = true;
		}
	}
}

void NeutralAlpaca_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    m_bSuperArmor = false;
    m_bInvincible = false;
}

void NeutralAlpaca_FSM::n_run()
{
    //if (Init_CurFrame(16))
    //    SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);


    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, XM_PI * 5.f);

    _float4 vPrePos = Get_Transform()->Get_State(Transform_State::POS);

    if (Get_Transform()->Go_Straight())
    {
        m_vTurnVector = (Get_Transform()->Get_State(Transform_State::LOOK) * -1.f).xyz();
        Get_Transform()->Set_LookDir(m_vTurnVector);
    }

    m_fPatrolDistanceCnt += (Get_Transform()->Get_State(Transform_State::POS) - vPrePos).Length();

    if (m_fPatrolDistanceCnt >= m_fPatrolDistance)
    {
        m_fPatrolDistanceCnt = 0.f;
        m_bPatrolMove = false;
        m_eCurState = STATE::b_idle;
    }

    m_bDetected = Target_In_DetectRange();
    
    if (m_bDetected)
    {
        m_eCurState = STATE::b_run;
    }

}

void NeutralAlpaca_FSM::n_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"n_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed * 0.2f);

    m_vTurnVector = _float3{ (rand() * 2 / _float(RAND_MAX) - 1), 0.f, (rand() * 2 / _float(RAND_MAX) - 1) };
    m_vTurnVector.Normalize();

    m_bSuperArmor = false;
}

void NeutralAlpaca_FSM::attack_run()
{
    if (Init_CurFrame(8))
        SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	if (!m_pTarget.expired())
		Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

	Get_Transform()->Go_Straight();

	if (Target_In_AttackRange())
		Execute_AttackSkill();
}

void NeutralAlpaca_FSM::attack_run_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

	Get_Transform()->Set_Speed(m_fRunSpeed);

	m_bSuperArmor = false;
	m_bInvincible = false;
}

void NeutralAlpaca_FSM::die_01()
{
    if (Is_AnimFinished())
    {
        auto script = make_shared<ObjectDissolve>(1.f);
        Get_Owner()->Add_Component(script);
        script->Init();

        if (!m_pAttackCollider.expired())
            EVENTMGR.Delete_Object(m_pAttackCollider.lock());
    }
}

void NeutralAlpaca_FSM::die_01_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"die_01", 0.2f, false, 1.f);
    
    SOUND.Play_Sound(L"vo_alpaca_die", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    m_bSuperArmor = false;
    m_bInvincible = true;
}

void NeutralAlpaca_FSM::die_02()
{
    if (Is_AnimFinished())
    {
        auto script = make_shared<ObjectDissolve>(1.f);
        Get_Owner()->Add_Component(script);
        script->Init();

        if (!m_pAttackCollider.expired())
            EVENTMGR.Delete_Object(m_pAttackCollider.lock());
    }
}

void NeutralAlpaca_FSM::die_02_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"die_02", 0.2f, false, 1.f);

    SOUND.Play_Sound(L"vo_alpaca_die", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    m_bSuperArmor = false;
    m_bInvincible = true;
}

void NeutralAlpaca_FSM::gaze_b()
{
    //if (Init_CurFrame(16))
    //    SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	_float3 vTargetPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

	vTargetPos = 2.f * vMyPos - vTargetPos;

	if (!m_pTarget.expired())
		Soft_Turn_ToTarget(vTargetPos, XM_PI * 5.f);
    m_tAttackCoolTime.fAccTime += fDT;

    Get_Transform()->Go_Backward();

    if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
    {

         m_eCurState = STATE::attack_run;
    }
}

void NeutralAlpaca_FSM::gaze_b_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_b", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);
    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSuperArmor = false;
}

void NeutralAlpaca_FSM::gaze_f()
{
    //if (Init_CurFrame(16))
    //    SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	_float3 vTargetPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

	vTargetPos = 2.f * vMyPos - vTargetPos;

	if (!m_pTarget.expired())
		Soft_Turn_ToTarget(vTargetPos, XM_PI * 5.f);
    m_tAttackCoolTime.fAccTime += fDT;

    Get_Transform()->Go_Straight();

    if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
    {

            m_eCurState = STATE::attack_run;
    }
}

void NeutralAlpaca_FSM::gaze_f_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_f", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_bSuperArmor = false;
    m_tAttackCoolTime.fAccTime = 0.f;
}

void NeutralAlpaca_FSM::gaze_l()
{
    //if (Init_CurFrame(16))
    //    SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	_float3 vTargetPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

	vTargetPos = 2.f * vMyPos - vTargetPos;

	if (!m_pTarget.expired())
		Soft_Turn_ToTarget(vTargetPos, XM_PI * 5.f);
    m_tAttackCoolTime.fAccTime += fDT;

    Get_Transform()->Go_Left();

    if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
    {

            m_eCurState = STATE::attack_run;
    }
}

void NeutralAlpaca_FSM::gaze_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_l", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);
    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSuperArmor = false;
}

void NeutralAlpaca_FSM::gaze_r()
{
    //if (Init_CurFrame(16))
    //    SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	_float3 vTargetPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

	vTargetPos = 2.f * vMyPos - vTargetPos;

	if (!m_pTarget.expired())
		Soft_Turn_ToTarget(vTargetPos, XM_PI * 5.f);

    m_tAttackCoolTime.fAccTime += fDT;

    Get_Transform()->Go_Right();

    if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
    {

            m_eCurState = STATE::attack_run;
    }
}

void NeutralAlpaca_FSM::gaze_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_r", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);
    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSuperArmor = false;
}

void NeutralAlpaca_FSM::airborne_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 10.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_end;
}

void NeutralAlpaca_FSM::airborne_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_start", 0.2f, false, 1.f);

    AttackCollider_Off();

    m_bSuperArmor = true;

    Get_CharacterController()->Add_Velocity(5.f);
}

void NeutralAlpaca_FSM::airborne_end()
{
    if (Is_AnimFinished())
    {
        if (!m_bIsDead)
            m_eCurState = STATE::airborne_up;
        else
        {
            m_bInvincible = true;

            Get_Owner()->Get_Animator()->Set_AnimState(true);

            auto script = make_shared<ObjectDissolve>(1.f);
            Get_Owner()->Add_Component(script);
            script->Init();

            if (!m_pAttackCollider.expired())
                EVENTMGR.Delete_Object(m_pAttackCollider.lock());
        }
    }
}

void NeutralAlpaca_FSM::airborne_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_end", 0.2f, false, 1.f);

    m_bSuperArmor = true;
}

void NeutralAlpaca_FSM::airborne_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void NeutralAlpaca_FSM::airborne_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.1f, false, 1.f);

    m_bSuperArmor = true;
}

void NeutralAlpaca_FSM::hit()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 10.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;


    Dead_Setting();
}

void NeutralAlpaca_FSM::hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"hit", 0.1f, false, 1.f);

    AttackCollider_Off();
}

void NeutralAlpaca_FSM::knock_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 10.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end;
}

void NeutralAlpaca_FSM::knock_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_start", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fRunSpeed * 0.5f);
}

void NeutralAlpaca_FSM::knock_end()
{
    if (m_iCurFrame < 13)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void NeutralAlpaca_FSM::knock_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end", 0.2f, false, 1.f);

    m_bSuperArmor = true;
}

void NeutralAlpaca_FSM::knock_end_loop()
{
    if (m_iCurFrame > Get_FinalFrame() / 2)
        m_eCurState = STATE::knock_up;

    if (m_bIsDead)
    {
        m_bInvincible = true;

        Get_Owner()->Get_Animator()->Set_AnimState(true);

        auto script = make_shared<ObjectDissolve>(1.f);
        Get_Owner()->Add_Component(script);
        script->Init();

        if (!m_pAttackCollider.expired())
            EVENTMGR.Delete_Object(m_pAttackCollider.lock());
    }
}

void NeutralAlpaca_FSM::knock_end_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_loop", 0.2f, false, 1.f);

    m_bSuperArmor = false;
}

void NeutralAlpaca_FSM::knock_end_hit()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;

    if (m_bIsDead)
        m_bInvincible = true;
}

void NeutralAlpaca_FSM::knock_end_hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_hit", 0.2f, false, 1.f);

    m_bSuperArmor = false;
}

void NeutralAlpaca_FSM::knock_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void NeutralAlpaca_FSM::knock_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.2f, false, 1.f);

    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fRunSpeed);
}

void NeutralAlpaca_FSM::knockdown_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knockdown_end;
}

void NeutralAlpaca_FSM::knockdown_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knockdown_start", 0.2f, false, 2.f);

    AttackCollider_Off();

    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed);
}

void NeutralAlpaca_FSM::knockdown_end()
{
    if (m_iCurFrame < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
    {
        if (!m_bIsDead)
            m_eCurState = STATE::knock_up;
        else
        {
            m_bInvincible = true;

            Get_Owner()->Get_Animator()->Set_AnimState(true);

            auto script = make_shared<ObjectDissolve>(1.f);
            Get_Owner()->Add_Component(script);
            script->Init();

            if (!m_pAttackCollider.expired())
                EVENTMGR.Delete_Object(m_pAttackCollider.lock());
        }
    }
}

void NeutralAlpaca_FSM::knockdown_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knockdown_end", 0.2f, false, 1.5f);

    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed * 0.5f);
}

void NeutralAlpaca_FSM::skill_1100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (m_iCurFrame <= 15)
    {
        m_fStTimer += fDT;
        Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f / (1.f + m_fStTimer));
        Get_Owner()->Get_Animator()->Get_RenderParamDesc().vec4Params[1] = Color(0.05f, 0.2f, 1.f, 1.f);
        Get_Owner()->Set_Instancing(false);

        m_bCanHitCounter = true;
    }
    else if(m_iCurFrame > 15)
		Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f);


    if (m_iCurFrame == 26)
	{
        Get_Owner()->Set_Instancing(true);
		Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f);
		Get_Owner()->Get_Animator()->Get_RenderParamDesc().vec4Params[1] = Color(0.f);

        m_bCanHitCounter = false;
        AttackCollider_On(STUN_HIT, 0.f);
    }
    else if (m_iCurFrame == 30)
        AttackCollider_Off();

    if (Is_AnimFinished())
    {
        m_eCurState = STATE::b_idle;
        m_bAngry = false;
    }
}

void NeutralAlpaca_FSM::skill_1100_Init()
{
    SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, 1.f);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_vTurnVector = Calculate_TargetTurnVector();

    m_bSuperArmor = true;
    m_bInvincible = true;
    m_fStTimer = 0.f;
}

void NeutralAlpaca_FSM::skill_2100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

	if (m_iCurFrame <= 15)
	{
		m_fStTimer += fDT;
		Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f / (1.f +m_fStTimer));
		Get_Owner()->Get_Animator()->Get_RenderParamDesc().vec4Params[1] = Color(0.05f, 0.2f, 1.f, 1.f);
		Get_Owner()->Set_Instancing(false);

		m_bCanHitCounter = true;
	}
	else if (m_iCurFrame > 15)
		Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f);

    //NORMAL ATTACK
    if (m_iCurFrame == 29)
    {
		Get_Owner()->Set_Instancing(true);
		Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f);
		Get_Owner()->Get_Animator()->Get_RenderParamDesc().vec4Params[1] = Color(0.f);
		m_bCanHitCounter = false;
        AttackCollider_On(STUN_HIT, 0.f);
    }
    else if (m_iCurFrame == 38)
        AttackCollider_Off();
    
	if (Is_AnimFinished())
	{
		m_eCurState = STATE::b_idle;
		m_bAngry = false;
	}
}

void NeutralAlpaca_FSM::skill_2100_Init()
{
    SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_2100", 0.15f, false, 1.f);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_vTurnVector = Calculate_TargetTurnVector();

    m_bSuperArmor = false;
}

void NeutralAlpaca_FSM::skill_3100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

	if (m_iCurFrame <= 15)
	{
		m_fStTimer += fDT;
		Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f / (1.f + m_fStTimer));
		Get_Owner()->Get_Animator()->Get_RenderParamDesc().vec4Params[1] = Color(0.05f, 0.2f, 1.f, 1.f);
		Get_Owner()->Set_Instancing(false);

		m_bCanHitCounter = true;
	}
	else if (m_iCurFrame > 15)
		Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f);


    if (m_iCurFrame == 33)
    {
		Get_Owner()->Set_Instancing(true);
		Get_Owner()->Get_Animator()->Set_AnimationSpeed(1.f);
		Get_Owner()->Get_Animator()->Get_RenderParamDesc().vec4Params[1] = Color(0.f);
		m_bCanHitCounter = false;
        AttackCollider_On(STUN_HIT, 0.f);
    }
    else if (m_iCurFrame == 34)
        AttackCollider_Off();

	if (Is_AnimFinished())
	{
		m_eCurState = STATE::b_idle;
		m_bAngry = false;
	}
}

void NeutralAlpaca_FSM::skill_3100_Init()
{
    SOUND.Play_Sound(L"vo_alpaca_att", CHANNELID::SOUND_EFFECT, m_fVoiceVolume, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_3100", 0.15f, false, 1.f);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_vTurnVector = Calculate_TargetTurnVector();

    m_bSuperArmor = false;
}

void NeutralAlpaca_FSM::CalCulate_PatrolTime()
{
    m_tPatrolMoveCool.fAccTime += fDT;

    if (m_tPatrolMoveCool.fAccTime >= m_tPatrolMoveCool.fCoolTime)
    {
        m_tPatrolMoveCool.fAccTime = 0.f;
        m_tPatrolMoveCool.fCoolTime = _float(rand() % 3) + 2.f;
        m_fPatrolDistance = _float(rand() % 4) + 6.f;
        m_vPatrolFirstPos = Get_Transform()->Get_State(Transform_State::POS);

        m_bPatrolMove = true;
    }
}

void NeutralAlpaca_FSM::Execute_AttackSkill()
{
    _uint iRan = rand() % 3;

    while (true)
    {
        if (iRan == m_iPreAttack)
            iRan = rand() % 3;
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
        m_eCurState = STATE::skill_2100;
        m_iPreAttack = 1;
    }
    else if (iRan == 2)
    {
        m_eCurState = STATE::skill_3100;
        m_iPreAttack = 2;
    }
}

void NeutralAlpaca_FSM::Set_Gaze()
{

        _uint iRan = 0;

        if (Target_In_AttackRange())
            iRan = rand() % 3;
        else
            iRan = rand() % 4;

        if (iRan == 0)
            m_eCurState = STATE::gaze_b;
        else if (iRan == 1)
            m_eCurState = STATE::gaze_l;
        else if (iRan == 2)
            m_eCurState = STATE::gaze_r;
        else if (iRan == 3)
            m_eCurState = STATE::gaze_f;
    
}

void NeutralAlpaca_FSM::Dead_Setting()
{
    if (m_bIsDead)
    {
        m_bInvincible = true;

        if (rand() % 2 == 0)
            m_eCurState = STATE::die_01;
        else
            m_eCurState = STATE::die_02;
    }
}

void NeutralAlpaca_FSM::GroundCheck()
{
    Ray ray;
    ray.position = Get_Transform()->Get_State(Transform_State::POS).xyz();

    Get_Transform()->Set_State(Transform_State::POS, _float4(ray.position.x, 0.f, ray.position.z, 1.f));
}

_float3 NeutralAlpaca_FSM::Calculate_TargetTurnVector()
{
    if (m_pTarget.expired())
        return _float3(0.f);
    else
        return m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz() - m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
}

_bool NeutralAlpaca_FSM::m_bAngry = false;
