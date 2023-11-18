#pragma once
#include "FSM.h"
#include "ForwardMovingSkillScript.h"

class Boss_Spike_FSM :
	public FSM
{
public:
	enum class STATE
	{
		SQ_Appear_01,
		SQ_Appear_02,
		SQ_Appear_03,
		Spawn,
		
		b_idle,
		b_run,
		
		gaze_b,
		gaze_f,
		gaze_l,
		gaze_r,

		die,
		hit, //normal_hit
		hit_b,
		hit_f,
		hit_l,
		hit_r,
		groggy_start,
		groggy_end,
		groggy_loop,


		skill_1100, 
		skill_1200, 
		skill_1300, 
		skill_1400, 

		skill_9100, //Evade
		skill_9200, //Evade
		skill_9300, //Evade
		skill_9400, //Evade
		
		skill_2100, 
		skill_2200, 
		skill_3100, 
		skill_3200, 
		skill_3300, 
		skill_4100, 
		skill_4200, 
		skill_5100, 
		skill_6100, 
		skill_7100, 
		skill_8100, 
		skill_10100, 
		skill_100000, 
		skill_100100, 
		skill_101100, 
		skill_201100,  //CUTSCENE (DOWN)
		skill_201200,  //CUTSCENE (UP)


		NONE
	};
public:
	Boss_Spike_FSM();
	~Boss_Spike_FSM();


public:
	virtual HRESULT Init() override;
	virtual void Tick() override;
	virtual void Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget) override;


private:
	virtual void State_Tick() override; // 상태를 항상 업데이트해줌
	virtual void State_Init() override; // 상태가 바뀔 때 한번 초기화 해줌
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void AttackCollider_On(const wstring& skillname) override;
	virtual void AttackCollider_Off() override;
	virtual void Set_State(_uint iIndex) override;


	//SQ_Appear_01,
	//	SQ_Appear_02,
	//	SQ_Appear_03,

	//	b_idle,
	//	b_run,

	//	gaze_b,
	//	gaze_f,
	//	gaze_l,
	//	gaze_r,
	//	die,

	//	hit, //normal_hit
	//	hit_b,
	//	hit_f,
	//	hit_l,
	//	hit_r,
	//	groggy_start,
	//	groggy_end,
	//	groggy_loop,



	void SQ_Appear_01();
	void SQ_Appear_01_Init();
	void SQ_Appear_02();
	void SQ_Appear_02_Init();
	void SQ_Appear_03();
	void SQ_Appear_03_Init();
	void Spawn();
	void SQ_Appear_03_Init();

	void b_idle();
	void b_idle_Init();
	void b_run();
	void b_run_Init();

	void gaze_b();
	void gaze_b_Init();
	void gaze_f();
	void gaze_f_Init();
	void gaze_l();
	void gaze_l_Init();
	void gaze_r();
	void gaze_r_Init();

	void die();
	void die_Init();

	
	void hit();
	void hit_Init();
	
	void skill_1100();
	void skill_1100_Init();
	void skill_1200();
	void skill_1200_Init();
	void skill_1300();
	void skill_1300_Init();
	void skill_1400();
	void skill_1400_Init();

	void skill_9100();
	void skill_9100_Init();
	void skill_9200();
	void skill_9200_Init();
	void skill_9300();
	void skill_9300_Init();
	void skill_9400();
	void skill_9400_Init();
	


	void Battle_Start();
	void Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType);
	void Calculate_SkillCamRight();
	void Set_AttackSkill_Phase1();
	void Create_BossSpikeChair();
	void Set_Gaze();

	_float3 Calculate_TargetTurnVector();

private:
	STATE m_eCurState = STATE::b_idle;
	STATE m_ePreState = STATE::NONE;
	STATE m_ePatternState = STATE::NONE;


	_float3 m_vTurnVector = _float3(0.f);
	_float4 m_vFirstPos = _float4(0.f);
	_float m_fTurnSpeed = XM_PI * 5.f;

	COOLTIMEINFO m_tBattleStartTime = { 1.f, 0.f };
	COOLTIMEINFO m_tAttackCoolTime = { 1.f, 0.f };
	COOLTIMEINFO m_tChaseCoolTime = { 2.f, 0.f };

	_bool m_bChaseSkill = false;
	_bool m_bDetected = false;
	_bool m_bBattleStart = false;
	_bool m_bCounter = false;
	_bool m_bSetPattern = false;

	_uint m_iGroggy_Gauge = 0;
	_uint m_iPreAttack = 100;

	_float m_fGazeCheckRange = 2.f;
	
	_float4 m_vPlayerBodyPos = _float4(0.f);
	_float4 m_vChairPos = _float4(0.f);
	
	_uint m_iChairBoneIndex = 0;
	_float4 m_vChairBonePos = _float4(0.f);
	_float4x4 m_ChairBoneMatrix = XMMatrixIdentity();
};

