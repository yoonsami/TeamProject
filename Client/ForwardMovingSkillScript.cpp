#include "pch.h"
#include "ForwardMovingSkillScript.h"

ForwardMovingSkillScript::ForwardMovingSkillScript(const _float4& vLook, _float fMoveSpeed, _float fLifeTime, _float fLimitDistance)
{
	m_vLook = vLook;
	m_fMoveSpeed = fMoveSpeed;
	m_tLifeTime.fCoolTime = fLifeTime;

	if (fLimitDistance > 0.f)
		m_fLimitDistance = fLimitDistance;
}

ForwardMovingSkillScript::~ForwardMovingSkillScript()
{
}

HRESULT ForwardMovingSkillScript::Init()
{
	Get_Transform()->Set_State(Transform_State::LOOK, m_vLook);
	Get_Transform()->Set_Speed(m_fMoveSpeed);
	m_vFirstPos = Get_Transform()->Get_State(Transform_State::POS);

	return S_OK;
}

void ForwardMovingSkillScript::Tick()
{
	m_tLifeTime.fAccTime += fDT;

	if (m_fLimitDistance == 0.f)
		Get_Transform()->Go_Straight();
	else
	{
		//IF LIMITDISTANCE IS NOT ZERO, THIS SKILL GO STRAIGHT AND STOP ON LIMIT

		_float fGap = (Get_Transform()->Get_State(Transform_State::POS).xyz() - m_vFirstPos.xyz()).Length();

		if (fGap < m_fLimitDistance)
			Get_Transform()->Go_Straight();
	}

	if (m_tLifeTime.fAccTime >= m_tLifeTime.fCoolTime)
		CUR_SCENE->Remove_GameObject(m_pOwner.lock());
}
