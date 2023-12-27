#include "pch.h"
#include "DragonBallMonster_FSM.h"
#include "Boss_Giant_Mir_FSM.h"
#include "ModelAnimator.h"
#include "MotionTrailRenderer.h"
#include "MotionTrailDisappear.h"
#include "SphereCollider.h"
#include "RigidBody.h"
#include "Model.h"
#include "AttackColliderInfoScript.h"
#include "UiDamageCreate.h"
#include "UiMonsterHp.h"
#include "ObjectDissolve.h"
#include "OBBBoxCollider.h"
#include "CreateEffect_Script.h"
#include "DragonBallLightning_Script.h"
#include "TimerScript.h"
#include "MainCameraScript.h"

HRESULT DragonBallMonster_FSM::Init()
{
	if (!m_pOwner.expired())
		m_pOwner.lock()->Set_MaxHp(200.f);

	shared_ptr<GameObject> rigidBodyObj = make_shared<GameObject>();
	rigidBodyObj->GetOrAddTransform()->Set_State(Transform_State::POS, { 0.f,0.f,0.f,1.f });
	{
		_float3 vObjPos = rigidBodyObj->Get_Transform()->Get_State(Transform_State::POS).xyz();
		auto rigidBody = make_shared<RigidBody>();
		rigidBodyObj->Add_Component(rigidBody);
		rigidBody->Create_CapsuleRigidBody(vObjPos, 2.f, 6.f);
	}
	EVENTMGR.Create_Object(rigidBodyObj);
	m_pRigidBody = rigidBodyObj;

	m_iCenterBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy001");

	m_fEffectVolume = 1.f;

	m_fMySoundDistance = 100.f;

	return S_OK;
}

void DragonBallMonster_FSM::Tick()
{
	DeadCheck();

	Update_UI_Pos();

	State_Tick();
}

void DragonBallMonster_FSM::Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget, _uint iElementType)
{
	auto pScript = m_pOwner.lock()->Get_Script<UiMonsterHp>();
	if (nullptr == pScript)
	{
		pScript = make_shared<UiMonsterHp>(true);
		m_pOwner.lock()->Add_Component(pScript);
		pScript->Init();
	}

	Set_HitColor();

	//Calculate Damage 
	m_pOwner.lock()->Get_Hurt(fDamage);


	CUR_SCENE->Get_UI(L"UI_Damage_Controller")->Get_Script<UiDamageCreate>()->Create_Damage_Font(Get_Owner(), fDamage, ElementType(iElementType));

}

void DragonBallMonster_FSM::State_Tick()
{
	m_fTimer_CreateFloorSkillEffect += fDT;

	State_Init();

	m_iCurFrame = Get_CurFrame();

	if (m_bSummonMeteor)
		Create_Meteor();

	Create_FloorSkillEffect();

	Recovery_Color();

	switch (m_eCurState)
	{
	case STATE::Idle:
		Idle();
		break;
	case STATE::Crash:
		Crash();
		break;
	case STATE::NONE:
		break;
	}

	if (m_iPreFrame != m_iCurFrame)
		m_iPreFrame = m_iCurFrame;
}

void DragonBallMonster_FSM::State_Init()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case STATE::Idle:
			Idle_Init();
			break;
		case STATE::Crash:
			Crash_Init();
			break;
		case STATE::NONE:
			break;
		}


		m_ePreState = m_eCurState;
	}

}

void DragonBallMonster_FSM::Idle()
{
	m_tPatternCoolTime.fAccTime += fDT;

	if (m_tPatternCoolTime.fAccTime >= m_tPatternCoolTime.fCoolTime)
	{
		Set_AttackPattern();
	}


	if (m_bIsDead)
	{
		m_tPatternCoolTime.fAccTime = 0.f;
		m_bInvincible = true;
		m_eCurState = STATE::Crash;
	}
}

void DragonBallMonster_FSM::Idle_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_CurrentAnim(L"anim_R02_DecoBall_00_Anim", true, 2.f);

	m_bInvincible = false;
}

void DragonBallMonster_FSM::Crash()
{
	if (Is_AnimFinished())
	{
		auto script = make_shared<ObjectDissolve>(1.f);
		Get_Owner()->Add_Component(script);
		script->Init();
	}
}

void DragonBallMonster_FSM::Crash_Init()
{
	shared_ptr<Model> model = RESOURCES.Get<Model>(L"Anim_P_R02_DecoBall_01_Break");

	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();
	animator->Set_Model(model);

	animator->Set_CurrentAnim(L"P_R02_DecoBall_01_Fire", false, 1.f);

	{
		Ray ray;
		ray.position = Get_Transform()->Get_State(Transform_State::POS).xyz();

		Get_Transform()->Set_State(Transform_State::POS, _float4(ray.position.x, 0.f, ray.position.z, 1.f));
	}
	{
		if (!m_pRigidBody.expired())
			EVENTMGR.Delete_Object(m_pRigidBody.lock());
	}

	SOUND.Play_Sound(L"RockFalling", CHANNELID::SOUND_EFFECT, 0.3f, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance);


	if (!m_pTarget.expired())
		m_pTarget.lock()->Get_FSM()->Set_State((_uint)Boss_Giant_Mir_FSM::STATE::groggy_start);
}

void DragonBallMonster_FSM::Summon_CrossFloor()
{	
	_float4x4 matWorldPos = Get_Transform()->Get_WorldMatrix();
	matWorldPos._42 = 0.f;
	Add_GroupEffectOwner(L"DragonBall_Cross", _float3(matWorldPos._41, 0.f, matWorldPos._43), true);

	m_tPatternCoolTime.fAccTime = 0.f;

	_float3 vSkillScale = _float3{ 3.f,1.f, 12.f };

	FLOORSKILLDESC desc;
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
	
	desc.fAttackStartGap = 2.5f;

	desc.fAttackTickTime = 0.3f;
	desc.strAttackType = NORMAL_SKILL;
	desc.strLastAttackType = NORMAL_SKILL;
	desc.fAttackDamage = 5.f;
	desc.fLastAttackDamage = 5.f;
	desc.iLimitAttackCnt = 3;
	

	_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
						Get_Transform()->Get_State(Transform_State::LOOK) * 15.f;
	
	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider",vSkillPos, vSkillScale, desc);


	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f;

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::LOOK) * -15.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::RIGHT) * -15.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT);

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::RIGHT) * 15.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	shared_ptr<GameObject> obj = make_shared<GameObject>();
	auto script = make_shared<TimerScript>(2.5f);
	script->Set_Function([&]() { SOUND.Play_Sound(L"burst_stone_04", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance); });
	obj->Add_Component(script);
	EVENTMGR.Create_Object(obj);
}

void DragonBallMonster_FSM::Summon_X_Floor()
{
	_float4x4 matWorldPos = Get_Transform()->Get_WorldMatrix();
	matWorldPos._42 = 0.f;
	Add_GroupEffectOwner(L"DragonBall_X", _float3(matWorldPos._41, 0.f, matWorldPos._43), true);

	m_tPatternCoolTime.fAccTime = 0.f;

	_float3 vSkillScale = _float3{ 3.f,1.f, 12.f };

	FLOORSKILLDESC desc;
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) + 
					 Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;
	
	desc.fAttackStartGap = 2.5f;
	
	desc.fAttackTickTime = 0.3f;
	desc.iLimitAttackCnt = 3;

	desc.strAttackType = NORMAL_SKILL;
	desc.strLastAttackType = NORMAL_SKILL;
	desc.fAttackDamage = 5.f;
	desc.fLastAttackDamage = 5.f;

	//Forward_Left
	_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
						Get_Transform()->Get_State(Transform_State::LOOK) * 11.f +
						Get_Transform()->Get_State(Transform_State::RIGHT) * -11.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	//Forward_Right
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) +
					 Get_Transform()->Get_State(Transform_State::RIGHT);

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
				Get_Transform()->Get_State(Transform_State::LOOK) * 11.f +
				Get_Transform()->Get_State(Transform_State::RIGHT) * 11.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	//BackWard_Left
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f + 
					 Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
				Get_Transform()->Get_State(Transform_State::LOOK) * -11.f +
				Get_Transform()->Get_State(Transform_State::RIGHT) * -11.f;
		
	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	//BackWard_Right
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f +
					 Get_Transform()->Get_State(Transform_State::RIGHT);

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
				Get_Transform()->Get_State(Transform_State::LOOK) * -11.f +
				Get_Transform()->Get_State(Transform_State::RIGHT) * 11.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	shared_ptr<GameObject> obj = make_shared<GameObject>();
	auto script = make_shared<TimerScript>(2.5f);
	script->Set_Function([&]() { SOUND.Play_Sound(L"burst_stone_04", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance); });
	obj->Add_Component(script);
	EVENTMGR.Create_Object(obj);
}

void DragonBallMonster_FSM::Summon_Hash_Floor()
{
	_float4x4 matWorldPos = Get_Transform()->Get_WorldMatrix();
	matWorldPos._42 = 0.f;
	Add_GroupEffectOwner(L"Dragonall_Hash", _float3(matWorldPos._41, 0.f, matWorldPos._43), true);

	m_tPatternCoolTime.fAccTime = 0.f;

	_float3 vSkillScale = _float3{ 4.f,1.f, 25.f };

	FLOORSKILLDESC desc;
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);

	desc.fAttackStartGap = 3.2f;

	desc.fAttackTickTime = 0.3f;
	desc.strAttackType = NORMAL_SKILL;
	desc.strLastAttackType = NORMAL_SKILL;
	desc.fAttackDamage = 5.f;
	desc.fLastAttackDamage = 5.f;
	desc.iLimitAttackCnt = 3;

	//Left
	_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
						Get_Transform()->Get_State(Transform_State::RIGHT) * -12.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	//Right
	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + 
				Get_Transform()->Get_State(Transform_State::RIGHT) * 12.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	//Forward
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT);
	
	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::LOOK) * 12.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	//BackWard
	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::LOOK) * -12.f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	shared_ptr<GameObject> obj = make_shared<GameObject>();
	auto script = make_shared<TimerScript>(3.f);
	script->Set_Function([&]() { SOUND.Play_Sound(L"magic_dragon_Breath_fire_00", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance); });
	obj->Add_Component(script);
	EVENTMGR.Create_Object(obj);
}

void DragonBallMonster_FSM::Summon_Web_Floor()
{
	_float4x4 matWorldPos = Get_Transform()->Get_WorldMatrix();
	matWorldPos._42 = 0.f;
	Add_GroupEffectOwner(L"Dragonall_Web", _float3(matWorldPos._41, 0.f, matWorldPos._43), true);

	m_tPatternCoolTime.fAccTime = 0.f;

	_float3 vSkillScale = _float3{ 4.f,1.f, 30.f };

	FLOORSKILLDESC desc;
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) + 
					 Get_Transform()->Get_State(Transform_State::RIGHT);

	desc.fAttackStartGap = 3.2f;

	desc.fAttackTickTime = 0.3f;
	desc.strAttackType = NORMAL_SKILL;
	desc.strLastAttackType = NORMAL_SKILL;
	desc.fAttackDamage = 5.f;
	desc.fLastAttackDamage = 5.f;
	desc.iLimitAttackCnt = 3;

	_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
						Get_Transform()->Get_State(Transform_State::LOOK) * 8.5f +
						Get_Transform()->Get_State(Transform_State::RIGHT) * -8.5f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
				Get_Transform()->Get_State(Transform_State::LOOK) * -8.5f +
				Get_Transform()->Get_State(Transform_State::RIGHT) * 8.5f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) +
					 Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;
	
	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
				Get_Transform()->Get_State(Transform_State::LOOK) * 8.5f +
				Get_Transform()->Get_State(Transform_State::RIGHT) * 8.5f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	
	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
				Get_Transform()->Get_State(Transform_State::LOOK) * -8.5f +
				Get_Transform()->Get_State(Transform_State::RIGHT) * -8.5f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	shared_ptr<GameObject> obj = make_shared<GameObject>();
	auto script = make_shared<TimerScript>(3.f);
	script->Set_Function([&]() { SOUND.Play_Sound(L"magic_dragon_Breath_fire_00", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance); });
	obj->Add_Component(script);
	EVENTMGR.Create_Object(obj);
}

void DragonBallMonster_FSM::Summon_HalfCircle_Floor()
{
	_uint iRan = rand() % 2;
	m_eHalfCircleDirection = (HalfCircleDirection)iRan;
	_float fPos = 0.f;
	if (LEFT == m_eHalfCircleDirection)
		fPos = -1.f;
	else if(RIGHT == m_eHalfCircleDirection)
		fPos = 1.f;

	_float3 vEffectPos = Get_Transform()->Get_WorldMatrix().Translation();
	vEffectPos.y = 0.f;
	_float4x4 matEffectWorldMatrix = _float4x4::CreateTranslation(vEffectPos);
	if (m_eHalfCircleDirection == RIGHT)
		Add_Effect(L"DragonBall_Half2", nullptr, matEffectWorldMatrix, true);
	else if(m_eHalfCircleDirection == LEFT)
		Add_Effect(L"Dragonall_Half", nullptr, matEffectWorldMatrix, true);


	m_tPatternCoolTime.fAccTime = 0.f;

	_float3 vSkillScale = _float3{ 14.f, 1.f, 30.f };

	FLOORSKILLDESC desc;
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
	
	desc.fAttackStartGap = 2.5f;

	desc.fAttackTickTime = 0.3f;
	desc.strAttackType = NORMAL_SKILL;
	desc.strLastAttackType = NORMAL_SKILL;
	desc.fAttackDamage = 5.f;
	desc.fLastAttackDamage = 5.f;
	desc.iLimitAttackCnt = 3;

	_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
						Get_Transform()->Get_State(Transform_State::RIGHT) * 14.f * fPos;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	shared_ptr<GameObject> obj = make_shared<GameObject>();
	auto script = make_shared<TimerScript>(2.5f);
	script->Set_Function([&]() { SOUND.Play_Sound(L"Thunder", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance); });
	obj->Add_Component(script);
	EVENTMGR.Create_Object(obj);
}

void DragonBallMonster_FSM::Summon_Star_Floor()
{
	_float4x4 matWorldPos = Get_Transform()->Get_WorldMatrix();
	matWorldPos._42 = 0.f;
	Add_GroupEffectOwner(L"Dragonall_Star", _float3(matWorldPos._41, 0.f, matWorldPos._43), true);

	m_tPatternCoolTime.fAccTime = 0.f;

	_float3 vSkillScale = _float3{ 3.f,1.f, 13.f };

	FLOORSKILLDESC desc;
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) +
		Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;

	desc.fAttackStartGap = 2.5f;

	desc.fAttackTickTime = 0.3f;
	desc.strAttackType = NORMAL_SKILL;
	desc.strLastAttackType = NORMAL_SKILL;
	desc.fAttackDamage = 5.f;
	desc.fLastAttackDamage = 5.f;
	desc.iLimitAttackCnt = 3;

	//Forward_Left
	_float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::LOOK) * 12.7f +
		Get_Transform()->Get_State(Transform_State::RIGHT) * -12.7f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	//Forward_Right
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) +
		Get_Transform()->Get_State(Transform_State::RIGHT);

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::LOOK) * 12.7f +
		Get_Transform()->Get_State(Transform_State::RIGHT) * 12.7f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	//BackWard_Left
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f +
		Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::LOOK) * -12.7f +
		Get_Transform()->Get_State(Transform_State::RIGHT) * -12.7f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	//BackWard_Right
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f +
		Get_Transform()->Get_State(Transform_State::RIGHT);

	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::LOOK) * -12.7f +
		Get_Transform()->Get_State(Transform_State::RIGHT) * 12.7f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	//Forward
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::LOOK) * 17.5f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	//Backward
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK) * -1.f;
	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::LOOK) * -17.5f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	//Left
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f;
	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::RIGHT) * -17.5f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);

	//Right
	desc.vSkillDir = Get_Transform()->Get_State(Transform_State::RIGHT);
	vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
		Get_Transform()->Get_State(Transform_State::RIGHT) * 17.5f;

	Create_FloorSkillCollider(Monster_Skill, L"DragonBall_FloorSkillCollider", vSkillPos, vSkillScale, desc);


	shared_ptr<GameObject> obj = make_shared<GameObject>();
	auto script = make_shared<TimerScript>(2.5f);
	script->Set_Function([&]() { SOUND.Play_Sound(L"burst_stone_04", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance); });
	obj->Add_Component(script);
	EVENTMGR.Create_Object(obj);
}

void DragonBallMonster_FSM::Create_Meteor()
{
	m_tMeteorCoolTime.fAccTime += fDT;

	if (m_tMeteorCoolTime.fAccTime >= m_tMeteorCoolTime.fCoolTime)
	{
		_float4 vMyPos = Get_Transform()->Get_State(Transform_State::POS);
		auto pPlayer = CUR_SCENE->Get_Player();
		_float4 vPlayerPos = pPlayer->Get_Transform()->Get_State(Transform_State::POS);
		vMyPos.y = vPlayerPos.y;

		FORWARDMOVINGSKILLDESC desc;
		desc.vSkillDir = _float3{ 0.f,-1.f,0.f };
		desc.fMoveSpeed = 10.f;
		desc.fLifeTime = 1.5f;
		desc.fLimitDistance = 20.f;

		for (_uint i = 0; i < 3; i++)
		{
			_float fOffSetX = ((rand() * 2 / _float(RAND_MAX) - 1) * (rand() % 10 + 5));
			_float fOffSetZ = ((rand() * 2 / _float(RAND_MAX) - 1) * (rand() % 10 + 5));

			_float4 vSkillPos = vMyPos + _float4{ fOffSetX, 13.5f, fOffSetZ, 0.f };
			
			Add_GroupEffectOwner(L"Mir_Meteor_Meteor", _float3(vSkillPos.x, vPlayerPos.y, vSkillPos.z), true);
			Add_GroupEffectOwner(L"Mir_Meteor_Floor", _float3(vSkillPos.x, vPlayerPos.y, vSkillPos.z), true);
			Create_ForwardMovingSkillCollider(Monster_Skill, L"DragonBallMonster_SkillCollider", vSkillPos, 1.f, desc, KNOCKDOWN_SKILL, 10.f);
		}

		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			auto script = make_shared<TimerScript>(1.35f);
			script->Set_Function([]() {CAMERA_SHAKE(0.2f, 0.3f); });
			obj->Add_Component(script);
			EVENTMGR.Create_Object(obj);
		}

		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			auto script = make_shared<TimerScript>(1.35f);
			script->Set_Function([&]() { SOUND.Play_Sound(L"burst_stone_03", CHANNELID::SOUND_EFFECT, m_fEffectVolume * g_fMonsterEffectRatio * g_fMonsterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), m_fMySoundDistance); });
			obj->Add_Component(script);
			EVENTMGR.Create_Object(obj);
		}


		m_tMeteorCoolTime.fAccTime = 0.f;
		m_iCurMeteorCnt++;	
	}

	if (m_iCurMeteorCnt >= m_iLimitMeteorCnt)
	{
		m_iCurMeteorCnt = 0;
		m_bSummonMeteor = false;
	}
}

void DragonBallMonster_FSM::Update_UI_Pos()
{
	if (m_eCurState == STATE::Idle)
	{
		if (m_pOwner.lock()->Get_Script<UiMonsterHp>())
		{
			m_CenterBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iCenterBoneIndex) *
				_float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

			m_vCenterBonePos = _float4{ m_CenterBoneMatrix.Translation().x, m_CenterBoneMatrix.Translation().y, m_CenterBoneMatrix.Translation().z , 1.f };

			m_pOwner.lock()->Get_Script<UiMonsterHp>()->Change_Pos(m_vCenterBonePos);
		}
	}
}

void DragonBallMonster_FSM::Create_FloorSkillEffect()
{
	if (m_fTimer_CreateFloorSkillEffect > 2.5f && !m_bIsCreateFloorSkillEffectDone)
	{
		shared_ptr<CreateEffect_Script> pScript = make_shared<CreateEffect_Script>();
		pScript->Set_CoolTime(0.05f);
		pScript->Set_SkillTag(L"DragonBall_floorattack");
		pScript->Set_UseOwnerPosOnly(true);
		_float4x4 matOffset = _float4x4::CreateScale(1.9f) 
			* _float4x4::CreateFromQuaternion(Quaternion(0.f, 0.f, 0.f, 1.f)) 
			* _float4x4::CreateTranslation(_float3(0.f, -2.2f, 0.f));
		pScript->Set_OffsetMatrix(matOffset);

		switch (m_iPreAttack)
		{
		case 0: // Cross
		{
			_float3 vRight, vLook;
			_float3 vUp = { 0.f, 1.f, 0.f };
			_float4 vPos = { 0.f, 0.f, 0.f, 1.f };
			_float4x4 matPivot = _float4x4::Identity;
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 90 degree
			vLook = _float3(1.f, 0.f, 0.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 180 degree
			vLook = _float3(0.f, 0.f, -1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 270 degree
			vLook = _float3(-1.f, 0.f, 0.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			break;
		}
		case 1:	// X
		{
			_float3 vRight, vLook;
			_float3 vUp = { 0.f, 1.f, 0.f };
			_float4 vPos = { 0.f, 0.f, 0.f, 1.f };
			_float4x4 matPivot = _float4x4::Identity;

			// 45 degree
			vLook = _float3(1.f, 0.f, 1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);

			// 135 degree
			vLook = _float3(1.f, 0.f, -1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);

			// 225 degree
			vLook = _float3(-1.f, 0.f, -1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);

			// 315 degree
			vLook = _float3(-1.f, 0.f, 1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			break;
		}
		case 2:	// Hash
		{
			_float3 vRight = { 1.f, 0.f, 0.f };
			_float3 vUp = { 0.f, 1.f, 0.f };
			_float3 vLook = {0.f, 0.f, 1.f};
			_float4 vPos = { 0.f, 0.f, 12.f, 1.f };
			_float4x4 matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_HashAttack", nullptr, matPivot, true);
			
			vLook = _float3(0.f, 0.f, 1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			vPos = { 0.f, 0.f, -12.f, 1.f };
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_HashAttack", nullptr, matPivot, true);
			
			vLook = _float3(1.f, 0.f, 0.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			vPos = { 12.f, 0.f, 0.f, 1.f };
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_HashAttack", nullptr, matPivot, true);
			
			vLook = _float3(-1.f, 0.f, 0.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			vPos = { -12.f, 0.f, 0.f, 1.f };
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_HashAttack", nullptr, matPivot, true);
			break;
		}
		case 3: // Web
		{
			Add_Effect(L"DragonBall_WebAttack", nullptr, _float4x4::Identity, true);
			break;
		}
		case 4: // Half
		{
			shared_ptr<GameObject> pEffectOwnerObj = make_shared<GameObject>();
			shared_ptr<DragonBallLightning_Script> pEffectScript = make_shared<DragonBallLightning_Script>();
			pEffectOwnerObj->Add_Component(pEffectScript);
			pEffectOwnerObj->Get_Script<DragonBallLightning_Script>()->Set_Direction(DragonBallLightning_Script::DIR((_int)m_eHalfCircleDirection));
			EVENTMGR.Create_Object(pEffectOwnerObj);
			break;
		}
		case 5: // Star
		{
			// 0 degree
			_float3 vRight, vLook;
			_float3 vUp = { 0.f, 1.f, 0.f };
			_float4 vPos = { 0.f, 0.f, 0.f, 1.f };
			_float4x4 matPivot = _float4x4::Identity;
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 45 degree
			vLook = _float3(1.f, 0.f, 1.f); 
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 90 degree
			vLook = _float3(1.f, 0.f, 0.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 135 degree
			vLook = _float3(1.f, 0.f, -1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 180 degree
			vLook = _float3(0.f, 0.f, -1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 225 degree
			vLook = _float3(-1.f, 0.f, -1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 270 degree
			vLook = _float3(-1.f, 0.f, 0.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			// 315 degree
			vLook = _float3(-1.f, 0.f, 1.f);
			vLook.Normalize();
			vRight = vLook.Cross(vUp);
			matPivot = _float4x4(_float4(vRight, 0.f), _float4(vUp, 0.f), _float4(vLook, 0.f), vPos);
			Add_Effect(L"DragonBall_StarRock", nullptr, matPivot, true);
			break;
		}
		default:
			break;
		}

		m_bIsCreateFloorSkillEffectDone = true;
	}
}

void DragonBallMonster_FSM::Set_AttackPattern()
{
	_uint iRan = rand() % 6;

	m_fTimer_CreateFloorSkillEffect = 0.f;
	m_bIsCreateFloorSkillEffectDone = false;


	while (true)
	{
		if (iRan == m_iPreAttack)
			iRan = rand() % 6;
		else
			break;
	}

	if (iRan == 0)
	{
		Summon_CrossFloor();
		m_tPatternCoolTime.fCoolTime = 4.f;
		m_iPreAttack = 0;
	}
	else if (iRan == 1)
	{
		Summon_X_Floor();
		m_tPatternCoolTime.fCoolTime = 4.f;
		m_iPreAttack = 1;
	}
	else if (iRan == 2)
	{
		Summon_Hash_Floor();
		m_tPatternCoolTime.fCoolTime = 5.f;
		m_iPreAttack = 2;
	}
	else if (iRan == 3)
	{
		Summon_Web_Floor();
		m_tPatternCoolTime.fCoolTime = 5.f;
		m_iPreAttack = 3;
	}
	else if (iRan == 4)
	{
		Summon_HalfCircle_Floor();
		m_tPatternCoolTime.fCoolTime = 7.f;
		m_iPreAttack = 4;
	}
	else if (iRan == 5)
	{
		Summon_Star_Floor();
		m_tPatternCoolTime.fCoolTime = 4.f;
		m_iPreAttack = 5;
	}
	
	if (!m_bSummonMeteor)
	{
		_uint iRan = rand() % 3;

		if (iRan == 0)
		{
			m_iLimitMeteorCnt = rand() % 3 + 1;
			m_bSummonMeteor = true;
		}
	}
	
}