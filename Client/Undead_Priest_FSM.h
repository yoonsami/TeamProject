#pragma once
#include "FSM.h"
#include "ForwardMovingSkillScript.h"
#include "InstallationSkill_Script.h"


class Undead_Priest_FSM :
	public FSM
{
public:
	enum class STATE
	{
		b_idle,
		b_run,
		n_run,
		wander,
		die,
		gaze_b,
		gaze_bl,
		gaze_br,
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
	virtual void Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget) override;


private:
	virtual void State_Tick() override; // ���¸� �׻� ������Ʈ����
	virtual void State_Init() override; // ���°� �ٲ� �� �ѹ� �ʱ�ȭ ����
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void AttackCollider_On(const wstring& skillname) override;
	virtual void AttackCollider_Off() override;
	virtual void Set_State(_uint iIndex) override;

	void b_idle();
	void b_idle_Init();
	void b_run();
	void b_run_Init();
	void n_run();
	void n_run_Init();
	void wander();
	void wander_Init();

	void die();
	void die_Init();

	void gaze_b();
	void gaze_b_Init();
	void gaze_bl();
	void gaze_bl_Init();
	void gaze_br();
	void gaze_br_Init();
	

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
	
	void CalCulate_PatrolTime();
	void Set_Gaze();
	void Entry_Battle();
	void Set_AttackSkill();

	_float3 Calculate_TargetTurnVector();
	void Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType);
	void Create_InstallationSkillCollider(const _float4& vPos, _float fSkillRange, INSTALLATIONSKILLDESC desc);


private:
	STATE m_eCurState = STATE::b_idle;
	STATE m_ePreState = STATE::NONE;
	STATE m_ePatternState = STATE::NONE;

	_float m_fTurnSpeed = XM_PI * 5.f;
	_float m_fPatrolDistance = 1.f;
	_float m_fPatrolDistanceCnt = 0.f;

	_float3 m_vTurnVector = _float3(0.f);
	_float4 m_vPatrolFirstPos = { 0.f,0.f,0.f,1.f };

	_bool m_bSetPattern = false;
	_bool m_bDetected = false;
	_bool m_bPatrolMove = false;

	COOLTIMEINFO m_tAttackCoolTime = { 4.f, 0.f };
	COOLTIMEINFO m_tPatrolMoveCool = { 4.f,0.f };

	_uint m_iPreAttack = 100;
};
