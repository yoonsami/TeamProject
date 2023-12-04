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

HRESULT DragonBallMonster_FSM::Init()
{
	if (!m_pOwner.expired())
		m_pOwner.lock()->Set_MaxHp(200.f);

	return S_OK;
}

void DragonBallMonster_FSM::Tick()
{
	DeadCheck();

	State_Tick();
}

void DragonBallMonster_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
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

void DragonBallMonster_FSM::Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget)
{
	auto pScript = m_pOwner.lock()->Get_Script<UiMonsterHp>();
	if (nullptr == pScript)
	{
		pScript = make_shared<UiMonsterHp>();
		m_pOwner.lock()->Add_Component(pScript);
		pScript->Init();
	}

	Set_HitColor();

	//Calculate Damage 
	m_pOwner.lock()->Get_Hurt(fDamage);

	CUR_SCENE->Get_UI(L"UI_Damage_Controller")->Get_Script<UiDamageCreate>()->Create_Damage_Font(Get_Owner(), fDamage);

}

void DragonBallMonster_FSM::State_Tick()
{
	State_Init();

	m_iCurFrame = Get_CurFrame();

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
	if (m_bIsDead)
	{
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

		if (Get_Owner()->Get_RigidBody())
			Get_Owner()->Get_RigidBody()->RemoveRigidBody();
	}

	if (!m_pTarget.expired())
		m_pTarget.lock()->Get_FSM()->Set_State((_uint)Boss_Giant_Mir_FSM::STATE::groggy_start);
}
