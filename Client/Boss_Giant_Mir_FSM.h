#pragma once
#include "FSM.h"
#include "ForwardMovingSkillScript.h"

class Boss_Giant_Mir_FSM :
	public FSM
{
public:
	enum class STATE
	{
		SQ_Spawn,
		b_idle,
		skill_1100, // Breath
		skill_100100, // Breath 
		SQ_Leave,
		NONE
	};


public:
	virtual HRESULT Init() override;
	virtual void Tick() override;
	virtual void Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget) override;


private:
	virtual void State_Tick() override; // 상태를 항상 업데이트해줌
	virtual void State_Init() override; // 상태가 바뀔 때 한번 초기화 해줌
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override;
	virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage) override;
	virtual void AttackCollider_Off() override;
	virtual void Set_State(_uint iIndex) override;

	void SQ_Spawn();
	void SQ_Spawn_Init();
	void b_idle();
	void b_idle_Init();
	void skill_1100();
	void skill_1100_Init();
	void skill_100100();
	void skill_100100_Init();
	void SQ_Leave();
	void SQ_Leave_Init();

	void Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType, _float fAttackDamage);
	void Create_Meteor();
	void Set_AttackPattern();
	void Calculate_IntroHeadCam();
	void Setting_DragonBall();

	_float CamDistanceLerp(_float fStart, _float fEnd, _float fRatio);

private:
	STATE m_eCurState = STATE::b_idle;
	STATE m_ePreState = STATE::NONE;

	_float m_fIntroCamDistance = 0.f;
	_float m_fCamRatio = 0.f;

	COOLTIMEINFO m_tAttackCoolTime = { 2.f, 0.f };
	COOLTIMEINFO m_tBreathCoolTime = { 0.15f, 0.f };
	COOLTIMEINFO m_tMeteorCoolTime = { 1.f, 0.f };

	_bool m_bIntroCam = false;
	_bool m_bEmissive = false;
	
	_uint m_iPreAttack = 100;
	
	_uint m_iHeadBoneIndex = 0;
	_uint m_iMouseBoneIndex = 0;

	_float3 m_vFirstPos = _float3(0.f);

	_float3 m_vHeadCamDir = _float3(0.f);
	_float4 m_vHeadBonePos = _float4(0.f);
	_float4 m_vHeadCamPos = _float4(0.f);

	_float4x4 HeadBoneMatrix = XMMatrixIdentity();
	_float4x4 MouseBoneMatrix = XMMatrixIdentity();

};