#pragma once
#include "Component.h"
class FSM abstract : public Component
{
public:
	FSM();
	virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) = 0;
	virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) = 0;
	virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) = 0;

protected:
	virtual void State_Tick() = 0;
	virtual void State_Init() = 0;

	_uint Get_CurFrame();
	_uint Get_FinalFrame();
	_bool Is_AnimFinished();

	_float3 Get_InputDirVector(); // 키보드 입력에 따라 카메라 기준 앞방향 : +x, 오른쪽 방향 : +y인 크기 1짜리 벡터 반환
	void Soft_Turn_ToInputDir(const _float3& vInputDir, _float turnSpeed);
	_float3 Soft_Turn_ToTarget(const _float3& vTargetPos, _float turnSpeed);
	_float3 Soft_Turn_ToTarget(const _float4& vTargetPos, _float turnSpeed);
};

