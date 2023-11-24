#pragma once
#include "Component.h"

class FSM abstract : public Component
{
public:
	FSM();
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) = 0;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) = 0;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) = 0;
	virtual void Set_State(_uint iIndex) = 0;
	virtual void Get_Hit(const wstring& skillname, _float fDamage ,shared_ptr<GameObject> pLookTarget) = 0;
	shared_ptr<GameObject> Get_AttackCollider() { return m_pAttackCollider.lock(); }
	void Set_AttackCollider(shared_ptr<GameObject> attackCollider) { m_pAttackCollider = attackCollider; }
protected:
	virtual void State_Tick() = 0;
	virtual void State_Init() = 0;
	virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage) = 0;
	virtual void AttackCollider_Off() = 0;

	void Calculate_CamBoneMatrix();

	_uint Get_CurFrame();
	_uint Get_FinalFrame();
	void Reset_Frame();
	_bool Is_AnimFinished();
	_float3 Get_InputDirVector(); // 키보드 입력에 따라 카메라 기준 앞방향 : +x, 오른쪽 방향 : +y인 크기 1짜리 벡터 반환
	void Soft_Turn_ToInputDir(const _float3& vInputDir, _float turnSpeed);
	_float3 Soft_Turn_ToTarget(const _float3& vTargetPos, _float turnSpeed);
	_float3 Soft_Turn_ToTarget(const _float4& vTargetPos, _float turnSpeed);

	_bool Get_Invincible() { return m_bInvincible; }
	void Set_Invincible(_bool bFlag) { m_bInvincible = bFlag; }

	_bool Get_SuperArmor() { return m_bSuperArmor; }
	void Set_SuperArmor(_bool bFlag) { m_bSuperArmor = bFlag; }

	_bool Target_In_AttackRange(_float* pGap = nullptr);
	_bool Target_In_DetectRange();
	_bool Target_In_GazeCheckRange();

	_bool CounterAttackCheck(_float fCheckDegree);
	void Set_DirToTarget();
	void Set_DirToTargetOrInput(_uint eType);
	void Look_DirToTarget();
	shared_ptr<GameObject> Find_TargetInFrustum(_uint eType);

	_bool Init_CurFrame(const _uint curFrame);

	void Add_Effect(const wstring& strSkilltag);
	void Add_And_Set_Effect(const wstring& strSkilltag);
	void Add_GroupEffectOwner(const wstring& strSkilltag, _float3 vPosOffset);
	void Cal_SkillCamDirection(const _float dist);
	_bool Check_Combo(_uint minFrame, KEY_TYPE eKeyType);

	_bool DeadCheck();

public:
	void Set_Target(shared_ptr<GameObject> pTarget);
	void Set_Camera(shared_ptr<GameObject> pCamera);
	void Set_Vehicle(shared_ptr<GameObject> pVehicle);
	void Reset_Target();
	void Reset_Weapon();
	void Reset_Vehicle();
	_float Get_ChargingRatio() { return m_fChargingRatio; }

 protected:
	weak_ptr<GameObject> m_pTarget;
	weak_ptr<GameObject> m_pAttackCollider;
	weak_ptr<GameObject> m_pSkillCollider;
	weak_ptr<GameObject> m_pWeapon;
	weak_ptr<GameObject> m_pCamera;
	weak_ptr<GameObject> m_pVehicle;

	weak_ptr<GameObject> m_pLookingTarget;
	weak_ptr<GameObject> m_pGroupEffect;

	//Frame Check
	_uint m_iPreFrame = 10000;
	_uint m_iCurFrame = 0;


	_bool m_bInitialize = false;
	_bool m_bInvincible = false;
	_bool m_bSuperArmor = false;
	_bool m_bCanCombo = false;
	_bool m_bIsDead = false;

	_float3 m_vHitDir = _float3{ 0.f };
	_float3 m_vDirToTarget = _float3(0.f);

	_float m_fAttackRange = 2.f;
	_float m_fDetectRange = 2.f;
	_float m_fGazeCheckRange = 2.f;

	//For Camera CutScene
	_float4 m_vCamStopPos = _float4(0.f);
	_float4 m_vSkillCamRight = _float4(0.f);

	//BoneIndex ,BonePosition , BoneMatrix
	_uint m_iCenterBoneIndex = 0;
	_uint m_iDummy_CP_BoneIndex = 0;
	_uint m_iCamBoneIndex = 0;
	_uint m_iSkillCamBoneIndex = 0;
	_uint m_iSkillBoneIndex = 0;

	_float4 m_vCenterBonePos = _float4(0.f);
	_float4 m_vDummy_CP_BonePos = _float4(0.f);
	_float4 m_vCamBonePos = _float4(0.f);
	_float4 m_vSkillCamBonePos = _float4(0.f);

	_float4x4 m_CenterBoneMatrix = XMMatrixIdentity();
	_float4x4 m_Dummy_CP_BoneMatrix = XMMatrixIdentity();
	_float4x4 m_CamBoneMatrix = XMMatrixIdentity();
	_float4x4 m_SkillCamBoneMatrix = XMMatrixIdentity();
	_float4x4 matBoneMatrix = XMMatrixIdentity();

	//Speed For. Movement
	_float m_fRunSpeed = 6.f;
	_float m_fSprintSpeed = 8.f;
	_float m_fKnockBackSpeed = 6.f;
	_float m_fKnockDownSpeed = 6.f;

	//Speed For. Animation
	_float m_fNormalAttack_AnimationSpeed = 2.f;
	_float m_fSkillAttack_AnimationSpeed = 1.5f;
	_float m_fEvade_AnimationSpeed = 1.5f;
	_float m_fChargingRatio = 0.f;
	_float m_fCurrentHoldingSkillTime = 0.f;
	_float m_fMaxHoldingSkillTime = 0.f;
	_float m_fTimePerFrame = 0.f;

	//For Effect.Create
	_float m_fEffectCreateTimer[4] = { 0.f, 0.f, 0.f, 0.f };

	//MotionCoolTime
	COOLTIMEINFO m_tRunEndDelay = { 0.2f,0.f };
	COOLTIMEINFO m_tKnockDownEndCoolTime = { 2.f, 0.f };
};

