#include "pch.h"
#include "Kyle_FSM.h"
#include "ModelAnimator.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"
#include "MainCameraScript.h"
#include "Camera.h"
#include "CoolTimeCheckScript.h"
#include "GroupEffect.h"
#include "CharacterController.h"

HRESULT Kyle_FSM::Init()
{
	if (!m_bInitialize)
	{
		auto animator = Get_Owner()->Get_Animator();
		if (animator)
		{
		
			animator->Set_CurrentAnim(L"b_idle", true, 1.f);
			m_eCurState = STATE::b_idle;
		}

		m_iCenterBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Center");
		m_iCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Cam");
		m_iSkillCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_SkillCam");

		m_pCamera = CUR_SCENE->Get_MainCamera();

		m_fSkillAttack_AnimationSpeed =1.f;
		m_fDetectRange = 5.f;

		m_eElementType = GET_DATA(HERO::KYLE).Element;

		m_bEntryTeam = true;

		m_bInitialize = true;

		m_iLWeaponBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Chain_Bone016");
		m_iRWeaponBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Chain_Bone08");

	}

	m_fNormalAttack_AnimationSpeed = 1.2f;
	m_fSkillAttack_AnimationSpeed = 1.0f;
	m_fEvade_AnimationSpeed = 1.5f;

	m_fVoiceVolume = 0.5f;
	m_fSwingVolume = 0.5f;
	m_fFootStepVolume = 0.4f;
	m_fEffectVolume = 0.4f;

	m_fMySoundDistance = 100.f;

	m_WeaponMaterial[0] = Get_Owner()->Get_Model()->Get_MaterialByName(L"mi_Wp_Kyle_Chain");
	m_WeaponMaterial[1] = Get_Owner()->Get_Model()->Get_MaterialByName(L"mi_Wp_Kyle");


	if (!m_pAttackCollider.expired())
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackElementType(m_eElementType);
	
	return S_OK;
}

void Kyle_FSM::Tick()
{
	State_Tick();
	
	if (!m_pAttackCollider.expired())
	{
		//m_pAttack transform set forward
		m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 1.5f + _float3::Up);
	}

}

void Kyle_FSM::State_Tick()
{
	State_Init();
	Recovery_Color();
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
	case STATE::skill_100100:
		skill_100100();
		break;
	case STATE::skill_100200:
		skill_100200();
		break;
	case STATE::skill_200100:
		skill_200100();
		break;
	case STATE::skill_200200:
		skill_200200();
		break;
	case STATE::skill_200300:
		skill_200300();
		break;
	case STATE::skill_300100:
		skill_300100();
		break;
	case STATE::skill_502100:
		skill_502100();
		break;
	case STATE::skill_500100:
		skill_500100();
		break;
	}
	
	if (m_iPreFrame != m_iCurFrame)
		m_iPreFrame = m_iCurFrame;
}

void Kyle_FSM::State_Init()
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
		case STATE::skill_200100:
			skill_200100_Init();
			break;
		case STATE::skill_200200:
			skill_200200_Init();
			break;
		case STATE::skill_200300:
			skill_200300_Init();
			break;
		case STATE::skill_300100:
			skill_300100_Init();
			break;
		case STATE::skill_502100:
			skill_502100_Init();
			break;
		case STATE::skill_500100:
			skill_500100_Init();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void Kyle_FSM::Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget, _uint iElementType)
{
	// Random -20 Percent
	_float fHitDamage = Utils::Random_In_Range(fDamage * 0.8f, fDamage);

	//Calculate Damage 
	m_pOwner.lock()->Get_Hurt(fHitDamage);

	_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vOppositePos = pLookTarget->Get_Transform()->Get_State(Transform_State::POS).xyz();

	m_vHitDir = vOppositePos - vMyPos;
	m_vHitDir.y = 0.f;
	m_vHitDir.Normalize();

	Set_HitColor();

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

			wstring strSoundTag = L"vo_kyle_hit_0";
			strSoundTag = strSoundTag + to_wstring(rand() % 4 + 1);
			SOUND.Play_Sound(strSoundTag, CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

			CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>()->ShakeCamera(0.05f, 0.1f);
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

			wstring strSoundTag = L"vo_kyle_hit_0";
			strSoundTag = strSoundTag + to_wstring(rand() % 4 + 1);
			SOUND.Play_Sound(strSoundTag, CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

			CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>()->ShakeCamera(0.1f, 0.2f);
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

			wstring strSoundTag = L"vo_kyle_hit_0";
			strSoundTag = strSoundTag + to_wstring(rand() % 4 + 1);
			SOUND.Play_Sound(strSoundTag, CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

			CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>()->ShakeCamera(0.1f, 0.3f);
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

			wstring strSoundTag = L"vo_kyle_hit_0";
			strSoundTag = strSoundTag + to_wstring(rand() % 4 + 1);
			SOUND.Play_Sound(strSoundTag, CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

			CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>()->ShakeCamera(0.05f, 0.3f);
		}
	}
	else
		CUR_SCENE->Get_MainCamera()->Get_Script<MainCameraScript>()->ShakeCamera(0.05f, 0.03f);

}

void Kyle_FSM::Set_State(_uint iIndex)
{
}

void Kyle_FSM::b_idle()
{
	_float3 vInputVector = Get_InputDirVector();

	if (!g_bIsCanMouseMove && !g_bCutScene)
	{
		if (KEYPUSH(KEY_TYPE::W) || KEYPUSH(KEY_TYPE::S) ||
			KEYPUSH(KEY_TYPE::A) || KEYPUSH(KEY_TYPE::D))
			m_eCurState = STATE::b_run_start;

		if (KEYTAP(KEY_TYPE::LBUTTON))
			m_eCurState = STATE::skill_1100;

		Use_Skill();
	}
}

void Kyle_FSM::b_idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);

	Get_Transform()->Set_Speed(m_fRunSpeed);
	m_tRunEndDelay.fAccTime = 0.f;

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::b_run_start()
{
	if (Init_CurFrame(10))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Right", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(20))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Left", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}

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

void Kyle_FSM::b_run_start_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run_start", 0.1f, false, 1.5f);

	Get_Transform()->Set_Speed(m_fRunSpeed);
	m_tRunEndDelay.fAccTime = 0.f;

	AttackCollider_Off();
	
	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::b_run()
{
	if (Init_CurFrame(10))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Right", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(20))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Left", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}

	Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

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
			if (m_iCurFrame == 1)
				m_eCurState = STATE::b_sprint;
		}

		if (KEYTAP(KEY_TYPE::LBUTTON))
			m_eCurState = STATE::skill_1100;

		Use_Skill();
	}
}

void Kyle_FSM::b_run_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

	Get_Transform()->Set_Speed(m_fRunSpeed);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::b_run_end_r()
{
	if (Init_CurFrame(3))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Right", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(6))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Left", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}

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

void Kyle_FSM::b_run_end_r_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run_end_r", 0.1f, false, 1.5f);

	Get_Transform()->Set_Speed(m_fRunSpeed);
	m_tRunEndDelay.fAccTime = 0.f;

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::b_run_end_l()
{
	if (Init_CurFrame(4))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Left", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(7))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Right", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}

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

void Kyle_FSM::b_run_end_l_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_run_end_l", 0.1f, false, 1.5f);

	Get_Transform()->Set_Speed(m_fRunSpeed);
	m_tRunEndDelay.fAccTime = 0.f;

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::b_sprint()
{
	m_fRunEffectTimer += fDT;
	if (m_fRunEffectTimer > 0.2f)
	{
		Add_Effect(L"Sprint_Wind");
		m_fRunEffectTimer = 0.f;
	}

	if (Init_CurFrame(7))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Right", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(14))
	{
		Create_Foot_Dust();
		SOUND.Play_Sound(L"footstep_Left", CHANNELID::SOUND_EFFECT, m_fFootStepVolume * g_fEnvironmentRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}

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

void Kyle_FSM::b_sprint_Init()
{
	Add_Effect(L"All_DashStart");
		
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_sprint", 0.1f, true, 1.f);

	Get_Transform()->Set_Speed(m_fSprintSpeed);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::b_walk()
{
}

void Kyle_FSM::b_walk_Init()
{
}

void Kyle_FSM::die()
{
}

void Kyle_FSM::die_Init()
{
}

void Kyle_FSM::airborne_start()
{
	m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();

	Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

	if (Is_AnimFinished())
		m_eCurState = STATE::airborne_end;
}

void Kyle_FSM::airborne_start_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"airborne_start", 0.2f, false, 1.f);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;

	Get_CharacterController()->Add_Velocity(6.f);
}

void Kyle_FSM::airborne_end()
{
	if (Is_AnimFinished())
		m_eCurState = STATE::airborne_up;
}

void Kyle_FSM::airborne_end_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"airborne_end", 0.2f, false, 1.f);

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Kyle_FSM::airborne_up()
{
	if (Is_AnimFinished())
		m_eCurState = STATE::b_idle;
}

void Kyle_FSM::airborne_up_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"knock_Up", 0.1f, false, 1.f);

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Kyle_FSM::hit()
{
	m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();

	Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

	if (Is_AnimFinished())
		m_eCurState = STATE::b_idle;
}

void Kyle_FSM::hit_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"hit", 0.1f, false, 1.f);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::knock_start()
{
	m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();

	Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

	Get_Transform()->Go_Backward();

	if (Is_AnimFinished())
		m_eCurState = STATE::knock_end;
}

void Kyle_FSM::knock_start_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"knock_start", 0.1f, false, 1.f);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;

	Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.8f);
}

void Kyle_FSM::knock_end()
{
	if (m_iCurFrame < 16)
		Get_Transform()->Go_Backward();

	if (Is_AnimFinished())
		m_eCurState = STATE::knock_end_loop;
}

void Kyle_FSM::knock_end_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"knock_end", 0.2f, false, 1.f);

	m_bInvincible = false;
	m_bSuperArmor = true;

	Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.5f);
}

void Kyle_FSM::knock_end_loop()
{
	m_tKnockDownEndCoolTime.fAccTime += fDT;

	if (m_iCurFrame > Get_FinalFrame() / 2)
		m_eCurState = STATE::knock_up;
}

void Kyle_FSM::knock_end_loop_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"knock_end_loop", 0.2f, false, 1.f);

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::knock_end_hit()
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

void Kyle_FSM::knock_end_hit_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"knock_end_hit", 0.2f, false, 1.f);

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::knock_up()
{
	if (Is_AnimFinished())
		m_eCurState = STATE::b_idle;
}

void Kyle_FSM::knock_up_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"knock_Up", 0.2f, false, 1.f);

	m_bInvincible = false;
	m_bSuperArmor = true;

	m_tKnockDownEndCoolTime.fAccTime = 0.f;

	Get_Transform()->Set_Speed(m_fRunSpeed);
}

void Kyle_FSM::knockdown_start()
{
	m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();

	Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

	Get_Transform()->Go_Backward();

	if (Is_AnimFinished())
		m_eCurState = STATE::knockdown_end;
}

void Kyle_FSM::knockdown_start_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"knockdown_start", 0.1f, false, 2.f);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;

	Get_Transform()->Set_Speed(m_fKnockDownSpeed);
}

void Kyle_FSM::knockdown_end()
{
	if (m_iCurFrame < 16)
		Get_Transform()->Go_Backward();

	if (Is_AnimFinished())
		m_eCurState = STATE::knock_up;
}

void Kyle_FSM::knockdown_end_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_CurrentAnim(L"knockdown_end", false, 1.5f);

	m_bInvincible = false;
	m_bSuperArmor = true;

	Get_Transform()->Set_Speed(m_fKnockDownSpeed * 0.5f);
}

void Kyle_FSM::skill_1100()
{
	if (Init_CurFrame(9))
		Add_And_Set_Effect(L"Kyle_1100");

	Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

	Look_DirToTarget();

	if (Init_CurFrame(9))
	{
		SOUND.Play_Sound(L"swing_axe_01", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		Set_ColliderOption(FIRE, L"Hit_Slash_Orange");
		AttackCollider_On(NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 0) * 0.25f);
	}
	else if (Init_CurFrame(19))
		AttackCollider_Off();

	if (!g_bIsCanMouseMove && !g_bCutScene)
	{
		if (Check_Combo(22, KEY_TYPE::LBUTTON))
			m_eCurState = STATE::skill_1200;

		//Using Skill
		Use_Skill();
	}
	
	if (Is_AnimFinished())
	{
		if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
			m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		
		m_eCurState = STATE::b_idle;
	}

}

void Kyle_FSM::skill_1100_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

	if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();

	m_bCanCombo = false;

	SOUND.Play_Sound(L"vo_kyle_att_02", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);


	Set_DirToTargetOrInput(OBJ_MONSTER);

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::skill_1200()
{
	if (Init_CurFrame(9))
		Add_And_Set_Effect(L"Klye_1200");

	Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

	Look_DirToTarget();

	if (Init_CurFrame(9))
	{
		SOUND.Play_Sound(L"swing_axe_02", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_1", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		Set_ColliderOption(FIRE, L"Hit_Slash_Orange");
		AttackCollider_On(NORMAL_ATTACK ,
			GET_DAMAGE(HERO::KYLE, 0) * 0.25f);
	}
	else if (Init_CurFrame(20))
		AttackCollider_Off();

	if (!g_bIsCanMouseMove && !g_bCutScene)
	{
		if (Check_Combo(20, KEY_TYPE::LBUTTON))
			m_eCurState = STATE::skill_1300;

		//Using Skill
		Use_Skill();
	}

	if (Is_AnimFinished())
	{
		if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
			m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		
		m_bCanCombo = false;
		m_eCurState = STATE::b_idle;
	}
}

void Kyle_FSM::skill_1200_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_1200", 0.15f, false, m_fNormalAttack_AnimationSpeed);

	if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Start_Attack_Button_Effect();
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(DEFAULT);
	}

	SOUND.Play_Sound(L"vo_kyle_att_03", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);


	m_bCanCombo = false;

	Set_DirToTargetOrInput(OBJ_MONSTER);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::skill_1300()
{
	if (Init_CurFrame(9))
		Add_And_Set_Effect(L"Klye_1300");

	Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

	Look_DirToTarget();

	if (Init_CurFrame(8))
	{
		SOUND.Play_Sound(L"swing_axe_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_2", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		Set_ColliderOption(FIRE, L"Hit_Slash_Orange");
		AttackCollider_On(NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 0) * 0.25f);
	}
	else if (Init_CurFrame(18))
		AttackCollider_Off();

	if (!g_bIsCanMouseMove && !g_bCutScene)
	{
		if (Check_Combo(24, KEY_TYPE::LBUTTON))
			m_eCurState = STATE::skill_1400;

		//Using Skill
		Use_Skill();
	}

	if (Is_AnimFinished())
	{
		if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
			m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		
		m_bCanCombo = false;
		m_eCurState = STATE::b_idle;
	}

}

void Kyle_FSM::skill_1300_Init()
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

	SOUND.Play_Sound(L"vo_kyle_att_03", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::skill_1400()
{
	if (Init_CurFrame(11))
		Add_Effect(L"Kyle_1400");

	Look_DirToTarget();
	if (m_iCurFrame >= 7 && m_iCurFrame < 50)
		Set_WeaponLight(true);
	else
		Set_WeaponLight(false);

	if (Init_CurFrame(10))
	{
		SOUND.Play_Sound(L"swing_axe_04", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(11))
	{
		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::LOOK) * 4.f;
		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 2.5f, desc, KNOCKBACK_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 0) * 0.25f
			, L"Hit_Slash_Orange");
	}

	if (Is_AnimFinished())
	{
		if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
			m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		
		m_bCanCombo = false;
		m_eCurState = STATE::b_idle;
	}

	if (!g_bIsCanMouseMove && !g_bCutScene)
	{
		//Using Skill
		Use_Skill();
	}
}

void Kyle_FSM::skill_1400_Init()
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

	SOUND.Play_Sound(L"vo_kyle_att_08", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	m_bInvincible = false;
	m_bSuperArmor = false;
}

void Kyle_FSM::skill_91100()
{
	Look_DirToTarget();

	_float3 vInputVector = Get_InputDirVector();

	if (Is_AnimFinished())
		m_eCurState = STATE::b_idle;

	if (m_iCurFrame >= 22)
	{
		if (vInputVector != _float3(0.f))
			m_eCurState = STATE::b_run;
	}
}

void Kyle_FSM::skill_91100_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_91100", 0.15f, false, m_fEvade_AnimationSpeed);

	SOUND.Play_Sound(L"7129_shout_01", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	m_bCanCombo = false;

	m_vDirToTarget = _float3(0.f);
	m_vDirToTarget = Get_InputDirVector();

	AttackCollider_Off();

	m_bInvincible = true;
	m_bSuperArmor = false;
}

void Kyle_FSM::skill_93100()
{
	_float3 vInputVector = Get_InputDirVector();

	if (Is_AnimFinished())
		m_eCurState = STATE::b_idle;

	if (m_iCurFrame >= 22)
	{
		if (vInputVector != _float3(0.f))
			m_eCurState = STATE::b_run;
	}
}

void Kyle_FSM::skill_93100_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_93100", 0.15f, false, m_fEvade_AnimationSpeed);

	SOUND.Play_Sound(L"7129_shout_01", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	m_bCanCombo = false;

	AttackCollider_Off();

	m_bInvincible = true;
	m_bSuperArmor = false;
}

void Kyle_FSM::skill_100100()
{
	if (Init_CurFrame(10))
		Add_Effect(L"Kyle_100100_Slash");

	Look_DirToTarget();

	if (m_iCurFrame >= 5 && m_iCurFrame < 32)
		Set_WeaponLight(true);
	else
		Set_WeaponLight(false);

	if (Init_CurFrame(9))
	{
		SOUND.Play_Sound(L"vo_kyle_att_long_03", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_axe_01", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(19))
	{
		SOUND.Play_Sound(L"swing_axe_02", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_1", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}


	if (m_iCurFrame >= 17)
	{
		if (m_iCurFrame == 17)
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

	if (Init_CurFrame(5))
	{
		Set_WeaponLight(true);
		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 1.5f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::UP);
		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 2.f, desc, NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 1) * 0.5f, L"Hit_Slash_Orange");
	}

	if (!g_bIsCanMouseMove && !g_bCutScene)
	{
		if (Check_Combo(25, KEY_TYPE::KEY_1))
			m_eCurState = STATE::skill_100200;

		Use_Dash();
	}

	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_eCurState = STATE::b_idle;
	}

	Calculate_CamBoneMatrix();

}

void Kyle_FSM::skill_100100_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_100100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

	m_bCanCombo = false;

	Set_DirToTargetOrInput(OBJ_MONSTER);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Kyle_FSM::skill_100200()
{
	if (Init_CurFrame(12))
		Add_GroupEffectOwner(L"Kyle_100100_Install", _float3(0.f, 0.f, 1.f), false, nullptr, false);

	Look_DirToTarget();
	
	if (m_iCurFrame >= 1 && m_iCurFrame < 25)
		Set_WeaponLight(true);
	else
		Set_WeaponLight(false);

	if (Init_CurFrame(4))
		SOUND.Play_Sound(L"swing_chain_2", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	else if (Init_CurFrame(12))
	{
		SOUND.Play_Sound(L"swing_axe_02", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"hit_explosive_bomb_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fCharacterEffectRatio * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(26))
	{
		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}


	if (m_iCurFrame >= 4)
	{
		if (!m_pCamera.expired())
		{
			_float4 vDir = m_vCamStopPos - m_vCenterBonePos;
			vDir.Normalize();

			m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(2.f);
			m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCenterBonePos.xyz());
			m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.5f, vDir.xyz(), 5.f);
		}
	}

	if (Init_CurFrame(12))
	{
		SOUND.Play_Sound(L"vo_kyle_sk_06", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 2.f, desc, AIRBORNE_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 1) * 0.5f
			, L"Hit_Slash_Orange");
	}

	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_eCurState = STATE::b_idle;
		
		if (!m_pCamera.expired())
			m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedTime(0.f);
	}

	Calculate_CamBoneMatrix();

	if (!g_bIsCanMouseMove && !g_bCutScene)
		Use_Dash();
}

void Kyle_FSM::skill_100200_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_100200", 0.15f, false, m_fSkillAttack_AnimationSpeed);
	m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(SKILL1);

	m_bCanCombo = false;

	Set_DirToTargetOrInput(OBJ_MONSTER);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;

	Calculate_CamBoneMatrix();
}

void Kyle_FSM::skill_200100()
{
	if (Init_CurFrame(10))
		Add_And_Set_Effect(L"Kyle_200100_Slash");
	else if (Init_CurFrame(12))
		Add_And_Set_Effect(L"Kyle_200_Boost");
	else if (Init_CurFrame(19))
		Add_GroupEffectOwner(L"Kyle_200100_Install", _float3(0.f, 0.f, 3.5f), false, nullptr, false);

	Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

	Look_DirToTarget();
	if (m_iCurFrame >= 1 && m_iCurFrame < 15)
		Set_WeaponLight(true);
	else
		Set_WeaponLight(false);

	if (Init_CurFrame(11))
	{
		SOUND.Play_Sound(L"vo_kyle_att_01", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_axe_01", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		Set_ColliderOption(FIRE, L"Hit_Slash_Orange");
		AttackCollider_On(NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 2) * 0.2f);
	}
	else if (Init_CurFrame(23))
		AttackCollider_Off();
	
	if (!g_bIsCanMouseMove && !g_bCutScene)
	{
		if (Check_Combo(16, KEY_TYPE::KEY_2))
			m_eCurState = STATE::skill_200200;

		Use_Dash();
	}

	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_eCurState = STATE::b_idle;
	}


}

void Kyle_FSM::skill_200100_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_200100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

	m_bCanCombo = false;

	Set_DirToTargetOrInput(OBJ_MONSTER);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Kyle_FSM::skill_200200()
{
	if (Init_CurFrame(10))
		Add_And_Set_Effect(L"Kyle_200200_Slash");
	else if (Init_CurFrame(12))
		Add_And_Set_Effect(L"Kyle_200_Boost");
	else if (Init_CurFrame(19))
		Add_GroupEffectOwner(L"Kyle_200200_Install", _float3(0.f, 0.f, 3.5f), false, nullptr, false);

	Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

	if (m_iCurFrame >= 1 && m_iCurFrame < 29)
		Set_WeaponLight(true);
	else
		Set_WeaponLight(false);
	
	if (Init_CurFrame(13))
	{
		SOUND.Play_Sound(L"swing_axe_02", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_1", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		Set_ColliderOption(FIRE, L"Hit_Slash_Orange");

		AttackCollider_On(NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 2) * 0.2f);
	}
	else if (Init_CurFrame(18))
	{
		Set_ColliderOption(FIRE, L"Hit_Slash_Orange");
		AttackCollider_Off();
	}
	else if (Init_CurFrame(21))
	{
		SOUND.Play_Sound(L"vo_kyle_att_02", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_axe_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_2", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		Set_ColliderOption(FIRE, L"Hit_Slash_Orange");
		AttackCollider_On(NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 2) * 0.2f);
	}
	else if (Init_CurFrame(28))
		AttackCollider_Off();

	Look_DirToTarget();

	if (!g_bIsCanMouseMove && !g_bCutScene)
	{
		if (Check_Combo(28, KEY_TYPE::KEY_2))
			m_eCurState = STATE::skill_200300;

		Use_Dash();
	}

	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_eCurState = STATE::b_idle;
	}

}

void Kyle_FSM::skill_200200_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_200200", 0.05f, false, m_fSkillAttack_AnimationSpeed);
	m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(SKILL2);

	m_bCanCombo = false;

	Set_DirToTargetOrInput(OBJ_MONSTER);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Kyle_FSM::skill_200300()
{
	if (Init_CurFrame(9))
		Add_GroupEffectOwner(L"Kyle_200300_Install", _float3(0.f, 0.f, 2.f), false, nullptr, false);

	if (m_iCurFrame >= 1 && m_iCurFrame < 62)
		Set_WeaponLight(true);
	else
		Set_WeaponLight(false);
	
	if (Init_CurFrame(8))
	{
		SOUND.Play_Sound(L"vo_kyle_sk_09", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_axe_04", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_01", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(9))
	{
		SOUND.Play_Sound(L"hit_explosive_bomb_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fCharacterEffectRatio * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;
		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::LOOK) * 3.f;
		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 2.5f, desc, KNOCKBACK_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 2) * 0.4f
			, L"Hit_Slash_Orange");
	}
	else if (Init_CurFrame(32))
		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	
	Look_DirToTarget();

	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_eCurState = STATE::b_idle;
	}

	if (!g_bIsCanMouseMove && !g_bCutScene)
		Use_Dash();
}

void Kyle_FSM::skill_200300_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_200300", 0.05f, false, m_fSkillAttack_AnimationSpeed);
	m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Next_Combo(SKILL2);

	m_bCanCombo = false;

	Set_DirToTargetOrInput(OBJ_MONSTER);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Kyle_FSM::skill_300100()
{
	Cal_WeaponBonePos();
	m_fWeaponEffectTimer += fDT;
	if (m_fWeaponEffectTimer > 0.04f)
	{
		Add_GroupEffectOwner(L"Kyle_500100_WeaponFollow1", _float3(m_vLWeaponPos), true, nullptr, false);
		Add_GroupEffectOwner(L"Kyle_500100_WeaponFollow1", _float3(m_vRWeaponPos), true, nullptr, false);
		m_fWeaponEffectTimer = 0.f;
	}

    if (Init_CurFrame(5))
		Add_And_Set_Effect(L"Klye_1300");
	if (Init_CurFrame(6))
		Add_Effect(L"Kyle_300100_Boost ");
	else if (Init_CurFrame(10))
		Add_GroupEffectOwner(L"Kyle_300100_Install1", _float3(0.f, 0.f, -0.5f), false, nullptr, false);
	else if (Init_CurFrame(28))
		Add_GroupEffectOwner(L"Kyle_300100_Install2", _float3(0.f, 0.f, 3.5f), false, nullptr, false);

	Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

	Look_DirToTarget();
	
	if (m_iCurFrame >= 5 && m_iCurFrame < 49)
		Set_WeaponLight(true);
	else
		Set_WeaponLight(false);

	if (Init_CurFrame(2))
	{
		Set_ColliderOption(FIRE, L"Hit_Slash_Orange");
		AttackCollider_On(NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 3) * 0.2f);
	}
	else if (Init_CurFrame(5))
		AttackCollider_Off();
	else if (Init_CurFrame(9))
	{
		SOUND.Play_Sound(L"vo_kyle_sk_11", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_axe_01", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_1", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(27))
	{
		SOUND.Play_Sound(L"swing_axe_02", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_2", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(35))
	{
		SOUND.Play_Sound(L"swing_axe_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_1", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}
	else if (Init_CurFrame(38))
		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	

	if (Init_CurFrame(10))
	{
		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::UP);

		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 2.5f, desc, NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 3) * 0.2f
			, L"Hit_Slash_Orange");
	}
	else if (Init_CurFrame(28))
	{
		SOUND.Play_Sound(L"hit_explosive_bomb_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fCharacterEffectRatio * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::LOOK) * 4.f;

		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 3.f, desc, AIRBORNE_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 3) * 0.3f
			, L"Hit_Slash_Orange");

	}
	else if (Init_CurFrame(36))
	{
		SOUND.Play_Sound(L"hit_explosive_bomb_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fCharacterEffectRatio * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::LOOK) * 3.f;

		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 3.f, desc, AIRBORNE_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 3) * 0.3f
			, L"Hit_Slash_Orange");

	}


	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_eCurState = STATE::b_idle;
	}

	if (!g_bIsCanMouseMove && !g_bCutScene)
		Use_Dash();
}

void Kyle_FSM::skill_300100_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_300100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

	m_bCanCombo = false;

	Set_DirToTargetOrInput(OBJ_MONSTER);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Kyle_FSM::skill_502100()
{
	Cal_WeaponBonePos();
	m_fWeaponEffectTimer += fDT;
	if (m_fWeaponEffectTimer > 0.04f)
	{
		Add_GroupEffectOwner(L"Kyle_500100_WeaponFollow1", _float3(m_vLWeaponPos), true, nullptr, false);
		Add_GroupEffectOwner(L"Kyle_500100_WeaponFollow1", _float3(m_vRWeaponPos), true, nullptr, false);
		m_fWeaponEffectTimer = 0.f;
	}

	if (Init_CurFrame(50))
		Add_GroupEffectOwner(L"Kyle_502100_Install2", _float3(0.f, 0.f, 2.f), false, nullptr, false);
	else if (Init_CurFrame(108))
		Add_Effect(L"Kyle_502100_Install1");
	else if (Init_CurFrame(109))
		Add_GroupEffectOwner(L"Kyle_502100_Wind", _float3(0.f, 0.5f, -3.8f), false, nullptr, false);

	if (m_iCurFrame >= 21 && m_iCurFrame < 143)
		Set_WeaponLight(true);
	else
		Set_WeaponLight(false);

	if (m_iCurFrame == 17)
	{
		if (!m_pCamera.expired())
		{
			_float4 vDestinationPos = (Get_Transform()->Get_State(Transform_State::POS)) + (Get_Transform()->Get_State(Transform_State::LOOK) * -5.f) + _float3::Up * 4.f;
			_float4 vDir = vDestinationPos - (Get_Transform()->Get_State(Transform_State::POS));
			vDir.Normalize();

			m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
			m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCenterBonePos.xyz());
			m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(4.5f, vDir.xyz(), 10.f);
		}
	}
	else if (m_iCurFrame == 90)
	{
		m_vCamStopPos = m_vCenterBonePos;
	}
	else if (m_iCurFrame > 90 && m_iCurFrame <= 152)
	{
		m_vCamStopPos.y += fDT * 2.f;
		m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCamStopPos.xyz());
	}
	else if (m_iCurFrame > 152)
	{
		if (m_vCamStopPos.y >= 0.f)
			m_vCamStopPos.y -= fDT * 3.f;
		else
			m_vCamStopPos.y = 0.f;

		m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCamStopPos.xyz());
	}
	
	Calculate_CamBoneMatrix();

	if (Init_CurFrame(10))
		SOUND.Play_Sound(L"vo_kyle_sk_15", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	else if (Init_CurFrame(30))
		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);


	if (Init_CurFrame(55))
	{
		SOUND.Play_Sound(L"swing_chain_1", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"hit_explosive_bomb_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fCharacterEffectRatio * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 1.f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::LOOK) * 2.f +
			Get_Transform()->Get_State(Transform_State::UP);
		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 1.f, desc, NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 4) * 0.2f
			, L"Hit_Slash_Orange");

	}
	else if (Init_CurFrame(78))
	{
		SOUND.Play_Sound(L"swing_chain_1", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"hit_explosive_bomb_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fCharacterEffectRatio * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 1.f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::LOOK) * 5.f +
			Get_Transform()->Get_State(Transform_State::UP);
		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 2.f, desc, NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 4) * 0.4f
			, L"Hit_Slash_Orange");
	}
	else if (Init_CurFrame(106))
	{
		SOUND.Play_Sound(L"hit_explosive_bomb_02", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fCharacterEffectRatio * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::LOOK) * 5.f +
			Get_Transform()->Get_State(Transform_State::UP);

		_float4 vSkillDir = (Get_Transform()->Get_State(Transform_State::POS) - vSkillPos);
		vSkillDir.y = 0.f;

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = vSkillDir.Normalize();
		desc.fMoveSpeed = 20.f;
		desc.fLifeTime = 1.f;
		desc.fLimitDistance = 4.f;

		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 2.f, desc, NORMAL_SKILL, 
			GET_DAMAGE(HERO::KYLE, 4) * 0.4f
			, L"Hit_Slash_Orange");
	}
	else if (Init_CurFrame(108))
	{
		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);
	}

	Look_DirToTarget();

	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_eCurState = STATE::b_idle;
	}

}

void Kyle_FSM::skill_502100_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_502100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

	m_bCanCombo = false;

	Set_DirToTargetOrInput(OBJ_MONSTER);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;
}

void Kyle_FSM::skill_500100()
{
	Cal_WeaponBonePos();
	m_fWeaponEffectTimer += fDT;
	if (m_fWeaponEffectTimer > 0.04f)
	{
		Add_GroupEffectOwner(L"Kyle_500100_WeaponFollow1", _float3(m_vLWeaponPos), true, nullptr, false);
		Add_GroupEffectOwner(L"Kyle_500100_WeaponFollow1", _float3(m_vRWeaponPos), true, nullptr, false);
		m_fWeaponEffectTimer = 0.f;
	}


	if (Init_CurFrame(10))
		Add_And_Set_Effect(L"Kyle_500100_Swing");
	else if (Init_CurFrame(83))
		Add_Effect(L"Kyle_500100_Slash");
	else if (Init_CurFrame(94))
		Add_GroupEffectOwner(L"Kyle_500100_Install", _float3(0.f, 0.f, 3.f), false, nullptr, false);

	Update_GroupEffectWorldPos(Get_Owner()->Get_Transform()->Get_WorldMatrix());

	if (m_iCurFrame >= 1 && m_iCurFrame < 106)
		Set_WeaponLight(true);
	else
		Set_WeaponLight(false);
	if (m_iCurFrame >= 1)
	{
		if (!m_pCamera.expired())
		{
			_float4 vDestinationPos = (Get_Transform()->Get_State(Transform_State::POS)) + 
									   m_vSkillCamRight +
									  (Get_Transform()->Get_State(Transform_State::LOOK) * -3.f) 
										+ _float3::Up * 6.f;
			_float4 vDir = vDestinationPos - m_vCenterBonePos; 

			vDir.Normalize();
			
			m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
			m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vCenterBonePos.xyz());
			m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.5f, vDir.xyz(), 8.f);
		}
	}
	
	Calculate_CamBoneMatrix();

	if (Init_CurFrame(11))
	{
		SOUND.Play_Sound(L"vo_kyle_sk_02", CHANNELID::SOUND_EFFECT, m_fVoiceVolume * g_fCharacterVoiceRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_axe_01", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::UP);
		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 4.f, desc, NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 5) * 0.22f
			, L"Hit_Slash_Orange");

	}
	else if (Init_CurFrame(31))
	{
		SOUND.Play_Sound(L"swing_axe_02", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_1", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::UP);
		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 4.f, desc, NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 5) * 0.22f
			, L"Hit_Slash_Orange");

	}
	else if (Init_CurFrame(47))
	{
		SOUND.Play_Sound(L"swing_axe_03", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_2", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;	

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::UP);
		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 4.f, desc, NORMAL_ATTACK, 
			GET_DAMAGE(HERO::KYLE, 5) * 0.22f
			, L"Hit_Slash_Orange");
	}
	else if (Init_CurFrame(85))
	{
		SOUND.Play_Sound(L"swing_axe_02", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		SOUND.Play_Sound(L"swing_chain_2", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	}
	else if (Init_CurFrame(87))
	{
		SOUND.Play_Sound(L"hit_explosive_bomb_01", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fCharacterEffectRatio * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
		desc.fMoveSpeed = 0.f;
		desc.fLifeTime = 0.5f;
		desc.fLimitDistance = 0.f;

		_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
			Get_Transform()->Get_State(Transform_State::LOOK) * 5.f;
		Create_ForwardMovingSkillCollider(Player_Skill, L"Player_SkillCollider", vSkillPos, 4.f, desc, AIRBORNE_SKILL, 
			GET_DAMAGE(HERO::KYLE, 5) * 0.34f
			, L"Hit_Slash_Orange");

	}
	else if (Init_CurFrame(98))
		SOUND.Play_Sound(L"swing_chain_0", CHANNELID::SOUND_EFFECT, m_fSwingVolume * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);

	

	if (m_iCurFrame > 11)
		m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(Get_Transform()->Get_State(Transform_State::POS).xyz());

	if (m_iCurFrame == 164)
	{
		m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(0.2f);
	}

	Look_DirToTarget();

	if (Is_AnimFinished())
	{
		m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Skill_End();
		m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.f);
		m_eCurState = STATE::b_idle;
	}

	if (!g_bIsCanMouseMove && !g_bCutScene)
		Use_Dash();
}

void Kyle_FSM::skill_500100_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"skill_500100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

	m_bCanCombo = false;

	Set_DirToTargetOrInput(OBJ_MONSTER);

	AttackCollider_Off();

	m_bInvincible = false;
	m_bSuperArmor = true;

	Calculate_CamBoneMatrix();

	//For.Skill_CutScene
	Cal_SkillCamDirection(3.f);
}

void Kyle_FSM::Use_Skill()
{
	if (KEYTAP(KEY_TYPE::KEY_1)) // && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL1))
		m_eCurState = STATE::skill_100100;
	else if (KEYTAP(KEY_TYPE::KEY_2))// && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL2))
		m_eCurState = STATE::skill_200100;
	else if (KEYTAP(KEY_TYPE::KEY_3)) //&& m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL3))
		m_eCurState = STATE::skill_300100;
	else if (KEYTAP(KEY_TYPE::KEY_4)) //&& m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL4))
		m_eCurState = STATE::skill_502100;
	else if (KEYTAP(KEY_TYPE::KEY_5))// && m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->IsAvailable(SKILL5))
		m_eCurState = STATE::skill_500100;
	else 
		Use_Dash();
}

void Kyle_FSM::Use_Dash()
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

void Kyle_FSM::AttackCollider_On(const wstring& skillname, _float fAttackDamage)
{
	if (!m_pAttackCollider.expired())
	{
		m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(fAttackDamage);
	}
	Set_WeaponLight(true);
}

void Kyle_FSM::AttackCollider_Off()
{
	if (!m_pAttackCollider.expired())
	{
		m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(0.f);
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_HitEffectTag(L"");
	}
	Set_WeaponLight(false);
}

void Kyle_FSM::Set_WeaponLight(_bool bOn)
{

	if(bOn)
	{
		for (auto& material : m_WeaponMaterial)
		{
			material->Get_MaterialDesc().emissive = Color(214.f / 255.f, 55.f / 255.f, 0.0f, 1.f);
		}
	}
	else
	{
		for (auto& material : m_WeaponMaterial)
		{
			material->Get_MaterialDesc().emissive = Color(_float3(0.f), 1.f);
		}
	}
}

void Kyle_FSM::Cal_WeaponBonePos()
{
	_float4x4 mLFinger = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iLWeaponBoneIndex) *
		_float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

	m_vLWeaponPos = _float4(mLFinger.Translation(), 1.f);

	_float4x4 mRFinger = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iRWeaponBoneIndex) *
		_float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

	m_vRWeaponPos = _float4(mRFinger.Translation(), 1.f);

}
