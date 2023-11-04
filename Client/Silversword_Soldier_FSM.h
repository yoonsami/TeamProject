#pragma once
#include "FSM.h"

class Silversword_Soldier_FSM :
	public FSM
{
public:
	enum class STATE
	{
		b_idle,
		b_run,
		die,
		skill_1100, //normal attack1
		skill_1200, //normal attack2
		skill_1300, //normal attack3
		NONE
	};

public:
	virtual HRESULT Init() override;
	virtual void Tick() override;


private:
	virtual void State_Tick() override; // 상태를 항상 업데이트해줌
	virtual void State_Init() override; // 상태가 바뀔 때 한번 초기화 해줌
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override;

	void b_idle();
	void b_idle_Init();
	void b_run();
	void b_run_Init();

	void die();
	void die_Init();

	void skill_1100();
	void skill_1100_Init();
	void skill_1200();
	void skill_1200_Init();
	void skill_1300();
	void skill_1300_Init();

private:
	STATE m_eCurState = STATE::b_idle;
	STATE m_ePreState = STATE::NONE;

	_float3 m_vInputTurnVector = _float3(0.f);

	_float m_fRunSpeed = 6.f;
	_float m_fSprintSpeed = 8.f;
	_float m_fNormalAttack_AnimationSpeed = 2.f;
	_float m_fSkillAttack_AnimationSpeed = 2.f;
};

