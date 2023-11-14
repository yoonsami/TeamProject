#pragma once
#include "FSM.h"
#include "ForwardMovingSkillScript.h"

class Boss_Mir_FSM :
	public FSM
{
public:
	enum class STATE
	{
		First_Meet,
		sq_Intro,
		sq_Intro2,
		b_idle,
		turn_l,
		turn_r,
		die,
		groggy_start,
		groggy_loop,
		groggy_end,

		SQ_SBRin_Roar,

		skill_1100, //SKILL 1
		skill_2100, //SKILL 2 ASSAULT
		skill_3100, //SKILL 3 Tail_Attack - TO RIGHT
		skill_4100, //SKILL 4 Tail_Attack - TO LEFT
		skill_5100, //SKILL 5 Fly_Back 
		skill_9100, //SKILL 6 Fly_To_The_Sky And FallBack
		skill_11100, //SKILL 7 FootAttack 
		skill_12100, //SKILL 8 ThunderAttack
		skill_13100, //SKILL 9 Breath to Forward
		skill_14100, //SKILL 10 Breath to Ground Twice
		
		skill_100000, //SKILL 11  Ready to PowerBreath
		skill_100100, //SKILL 11 - 1-  Shoot PowerBreath (Short)- end

		skill_200000, //SKILL 12  Ready to PowerBreath
		skill_200100, //SKILL 12  Ready to PowerBreath

		NONE
	};

	enum class DIR
	{
		FORWARD_LEFT,
		FORWARD_RIGHT,
		BACKWARD_LEFT,
		BACKWARD_RIGHT,
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
	virtual void Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget) override;
	virtual void AttackCollider_On(const wstring& skillname) override;
	virtual void AttackCollider_Off() override;
	virtual void Set_State(_uint iIndex) override;


	void First_Meet();
	void First_Meet_Init();
	void sq_Intro();
	void sq_Intro_Init();
	void sq_Intro2();
	void sq_Intro2_Init();
	void b_idle();
	void b_idle_Init();
	void turn_l();
	void turn_l_Init();
	void turn_r();
	void turn_r_Init();
	void die();
	void die_Init();

	void groggy_start();
	void groggy_start_Init();
	void groggy_loop();
	void groggy_loop_Init();
	void groggy_end();
	void groggy_end_Init();

	void SQ_SBRin_Roar();
	void SQ_SBRin_Roar_Init();
	void skill_1100();
	void skill_1100_Init();
	void skill_2100();
	void skill_2100_Init();
	void skill_3100();
	void skill_3100_Init();
	void skill_4100();
	void skill_4100_Init();
	void skill_5100();
	void skill_5100_Init();
	void skill_9100();
	void skill_9100_Init();
	void skill_11100();
	void skill_11100_Init();
	void skill_12100();
	void skill_12100_Init();
	void skill_13100();
	void skill_13100_Init();
	void skill_14100();
	void skill_14100_Init();
	void skill_100000();
	void skill_100000_Init();
	void skill_100100();
	void skill_100100_Init();
	void skill_200000();
	void skill_200000_Init();
	void skill_200100();
	void skill_200100_Init();

	DIR CalCulate_PlayerDir();
	_bool CounterAttackCheck();
	void Add_Boss_Mir_Collider();
	void Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType);
	void Create_CounterMotionTrail();

	void TailAttackCollider_On(const wstring& skillname);
	void TailAttackCollider_Off();



private:
	STATE m_eCurState = STATE::b_idle;
	STATE m_ePreState = STATE::NONE;

	DIR m_eAttackDir = DIR::NONE;

	_float3 m_vTurnVector = _float3(0.f);

	_float3 m_vFirstPos = _float3(0.f);

	_float m_fRunSpeed = 4.f;
	_float m_fKnockDownSpeed = 4.f;
	_float m_fTurnSpeed = XM_PI * 0.5f;

	_float m_fNormalAttack_AnimationSpeed = 2.f;

	COOLTIMEINFO m_tAttackCoolTime = { 2.f, 0.f };
	COOLTIMEINFO m_tBreathCoolTime = { 0.15f, 0.f };
	
	_bool m_bDetected = false;
	_bool m_bSkillCreate = false;
	_bool m_bTurnMotion = false;
	_bool m_bCounter = false;

	_uint m_iPreAttack = 100;
	_uint m_iBreathType = 0;

	_uint m_iHeadBoneIndex = 0;
	_uint m_iMouseBoneIndex = 0;
	_uint m_iTailBoneIndex = 0;

	weak_ptr<GameObject> m_pTailCollider;


	_bool m_bCamDirSetting = false;
	_float3 m_vHeadCamDir = _float3(0.f);
	_float4 m_vHeadBonePos = _float4(0.f);
	_float4 m_vHeadCamPos = _float4(0.f);

	_float4x4 HeadBoneMatrix = XMMatrixIdentity();
	_float4x4 MouseBoneMatrix = XMMatrixIdentity();
	_float4x4 TailBoneMatrix = XMMatrixIdentity();

};