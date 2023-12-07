#include "pch.h"
#include "Client_FSM.h"
#include "BaseCollider.h"
#include "AttackColliderInfoScript.h"

void Client_FSM::AttackCollider_On(const wstring& skillname, _float fAttackDamage)
{
	if (!m_pAttackCollider.expired())
	{
		m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(fAttackDamage);
	}
}

void Client_FSM::AttackCollider_Off()
{
	if (!m_pAttackCollider.expired())
	{
		m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
		m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_AttackDamage(0.f);
	}
}
