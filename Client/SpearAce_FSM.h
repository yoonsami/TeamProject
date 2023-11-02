#pragma once
#include "FSM.h"

class SpearAce_FSM :
	public FSM
{
public:
	enum class STATE
	{
		b_idle,
		b_run_start,
		b_run,
		b_run_end_r,
		b_run_end_l,
		b_sprint,
		b_walk,
		die,
		skill_1100, //normal attack1
		skill_1200, //normal attack2
		skill_1300, //normal attack3
		skill_91100, //Evade
		skill_93100, //Backward_Evade
		skill_100100, // Skill 1
		skill_200100, // Skill 2-1
		skill_200200, // Skill 2-2
		skill_300100, // Skill 3
		skill_502100, // Skill 4
		skill_500100, // Skill 5
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
	void b_run_start();
	void b_run_start_Init();
	void b_run();
	void b_run_Init();
	void b_run_end_r();
	void b_run_end_r_Init();
	void b_run_end_l();
	void b_run_end_l_Init();
	void b_sprint();
	void b_sprint_Init();
	void b_walk();
	void b_walk_Init();
	void die();
	void die_Init();

	void skill_1100();
	void skill_1100_Init();
	void skill_1200();
	void skill_1200_Init();
	void skill_1300();
	void skill_1300_Init();

	void skill_91100();
	void skill_91100_Init();
	void skill_93100();
	void skill_93100_Init();
	void skill_100100();
	void skill_100100_Init();
	void skill_200100();
	void skill_200100_Init();
	void skill_200200();
	void skill_200200_Init();
	void skill_300100();
	void skill_300100_Init();

	void skill_502100();
	void skill_502100_Init();
	void skill_500100();
	void skill_500100_Init();

private:
	STATE m_eCurState = STATE::b_idle;
	STATE m_ePreState = STATE::NONE;

	_bool m_bIsSprint = false; // IsSprint
	_bool m_bCanCombo = false;
};

