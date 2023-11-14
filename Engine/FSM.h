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
	virtual void Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget) = 0;

protected:
	virtual void State_Tick() = 0;
	virtual void State_Init() = 0;
	virtual void AttackCollider_On(const wstring& skillname) = 0;
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


	_bool Target_In_AttackRange();
	_bool Target_In_DetectRange();
public:
	void Set_Target(shared_ptr<GameObject> pTarget);
	void Set_Camera(shared_ptr<GameObject> pCamera);
	void Set_Vehicle(shared_ptr<GameObject> pVehicle);
	void Reset_Target();
	void Reset_Weapon();
	void Reset_Vehicle();
 protected:
	weak_ptr<GameObject> m_pTarget;
	weak_ptr<GameObject> m_pAttackCollider;
	weak_ptr<GameObject> m_pWeapon;
	weak_ptr<GameObject> m_pCamera;
	weak_ptr<GameObject> m_pVehicle;
	
	_bool m_bInvincible = false;
	_bool m_bSuperArmor = false;

	_float3 m_vHitDir = _float3{ 0.f };
	_float m_fAttackRange = 2.f;
	_float m_fDetectRange = 2.f;

	_uint m_iCenterBoneIndex = 0;
	_uint m_iDummy_CP_BoneIndex = 0;
	_uint m_iCamBoneIndex = 0;
	_uint m_iSkillCamBoneIndex = 0;

	_float4 m_vCenterBonePos = _float4(0.f);
	_float4 m_vDummy_CP_BonePos = _float4(0.f);
	_float4 m_vCamBonePos = _float4(0.f);
	_float4 m_vSkillCamBonePos = _float4(0.f);

	_float4x4 m_CenterBoneMatrix = XMMatrixIdentity();
	_float4x4 m_Dummy_CP_BoneMatrix = XMMatrixIdentity();
	_float4x4 m_CamBoneMatrix = XMMatrixIdentity();
	_float4x4 m_SkillCamBoneMatrix = XMMatrixIdentity();
};

