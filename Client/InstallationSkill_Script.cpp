#include "pch.h"
#include "InstallationSkill_Script.h"
#include "AttackColliderInfoScript.h"
#include "SphereCollider.h"

InstallationSkill_Script::InstallationSkill_Script(INSTALLATIONSKILLDESC desc)
{
	m_vLook = desc.vSkillDir;
	m_fMoveSpeed = desc.fMoveSpeed;
	m_bFirstAttack = desc.bFirstAttack;

	m_tAttackTickTime.fCoolTime = desc.fAttackTickTime;
	
	if (m_bFirstAttack)
		m_tAttackTickTime.fAccTime = desc.fAttackTickTime;
	else
		m_tAttackTickTime.fAccTime = 0.f;

	m_iLimitAttackCnt = desc.iLimitAttackCnt;

	if (desc.fLimitDistance > 0.f)
		m_fLimitDistance = desc.fLimitDistance;
	
	m_strAttackType = desc.strAttackType;
	m_strLastAttackType = desc.strLastAttackType;
}

InstallationSkill_Script::~InstallationSkill_Script()
{
}

HRESULT InstallationSkill_Script::Init()
{
	Get_Transform()->Set_State(Transform_State::LOOK, m_vLook);
	Get_Transform()->Set_Speed(m_fMoveSpeed);
	m_vFirstPos = Get_Transform()->Get_State(Transform_State::POS);

	if (m_pOwner.lock()->Get_Script<AttackColliderInfoScript>())
	{
		m_pOwner.lock()->Get_Collider()->Set_Activate(false);
		m_pOwner.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(m_strAttackType);
	}

	return S_OK;
}

void InstallationSkill_Script::Tick()
{
	m_tAttackTickTime.fAccTime+= fDT;

	_float fGap = (Get_Transform()->Get_State(Transform_State::POS).xyz() - m_vFirstPos.xyz()).Length();
	
	if (fGap < m_fLimitDistance)
		Get_Transform()->Go_Straight();

	if (m_pOwner.lock()->Get_Collider())
	{
		if (m_tAttackTickTime.fAccTime >= m_tAttackTickTime.fCoolTime)
		{
			m_pOwner.lock()->Get_Collider()->Set_Activate(true);
			m_tAttackTickTime.fAccTime = 0.f;
			m_bAttackOn = true;
			m_iCurrentAttackCnt++;

			if (m_iCurrentAttackCnt == m_iLimitAttackCnt)
				m_pOwner.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(m_strLastAttackType);
		}
		else
		{
			if (m_bAttackOn)
			{
				m_bAttackOn = false;
				m_pOwner.lock()->Get_Collider()->Set_Activate(false);
			}
		}
	}

	if (m_iCurrentAttackCnt > m_iLimitAttackCnt)
		CUR_SCENE->Remove_GameObject(m_pOwner.lock());
}
