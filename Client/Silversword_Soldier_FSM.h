#pragma once
#include "Client_FSM.h"

class Silversword_Soldier_FSM :
	public Client_FSM
{
public:
	enum class STATE
	{
		b_idle,
		b_run,
		n_run,
		die_01,
		die_02,
		gaze_b,
		gaze_f,
		gaze_l,
		gaze_r,
		airborne_start, //airborne_start -> airborne_end -> airborne_up
		airborne_end,
		airborne_up, // airborne_end_up Animation = knock_end_up animation
		hit, //normal_hit
		knock_start, // knock_start -> knock_end -> knock_end_loop -> knock_up
		knock_end, 
		knock_end_loop, 
		knock_end_hit, // hit on knock_state
		knock_up,
		knockdown_start, //knockdown_start -> knockdown_end -> knock_up
		knockdown_end,
		skill_1100, //SKILL 1
		skill_2100, //SKILL 2
		skill_3100, //SKILL 3
		NONE
	};

public:
	virtual HRESULT Init() override;
	virtual void Tick() override;
	virtual void Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget) override;


private:
	virtual void State_Tick() override; // 상태를 항상 업데이트해줌
	virtual void State_Init() override; // 상태가 바뀔 때 한번 초기화 해줌
	virtual void Set_State(_uint iIndex) override;

	void b_idle();
	void b_idle_Init();
	void b_run();
	void b_run_Init();
	void n_run();
	void n_run_Init();

	void die_01();
	void die_01_Init();
	void die_02();
	void die_02_Init();


	void gaze_b();
	void gaze_b_Init();
	void gaze_f();
	void gaze_f_Init();
	void gaze_l();
	void gaze_l_Init();
	void gaze_r();
	void gaze_r_Init();


	void airborne_start();
	void airborne_start_Init();
	void airborne_end();
	void airborne_end_Init();
	void airborne_up();
	void airborne_up_Init();
	void hit();
	void hit_Init();
	void knock_start();
	void knock_start_Init();

	void knock_end();
	void knock_end_Init();
	void knock_end_loop();
	void knock_end_loop_Init();
	void knock_end_hit();
	void knock_end_hit_Init();
	void knock_up();
	void knock_up_Init();
	void knockdown_start();
	void knockdown_start_Init();
	void knockdown_end();
	void knockdown_end_Init();

	void skill_1100();
	void skill_1100_Init();
	void skill_2100();
	void skill_2100_Init();
	void skill_3100();
	void skill_3100_Init();

	void Dead_Setting();
	void CalCulate_PatrolTime();
	void Execute_AttackSkill();
	void Detect_Target();
	void Target_DeadCheck();

private:
	STATE m_eCurState = STATE::b_idle;
	STATE m_ePreState = STATE::NONE;

	_float m_fPatrolDistance = 1.f;
	_float m_fPatrolDistanceCnt = 0.f;
	
	_float3 m_vTurnVector = _float3(0.f);
	_float4 m_vPatrolFirstPos = { 0.f,0.f,0.f,1.f };

	COOLTIMEINFO m_tAttackCoolTime = { 2.f, 0.f };
	COOLTIMEINFO m_tPatrolMoveCool = { 4.f, 0.f };
	COOLTIMEINFO m_tDetectCoolTime = { 1.f, 0.f };

	_bool m_bPatrolMove = false;

	_uint m_iPreAttack = 100;
};

