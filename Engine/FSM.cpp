#include "pch.h"
#include "FSM.h"
#include "ModelAnimator.h"

FSM::FSM() : Component(COMPONENT_TYPE::FSM)
{
}


_uint FSM::Get_CurFrame()
{
	if (m_pOwner.expired())
		assert(false);

	if (!Get_Owner()->Get_Animator())
		assert(false);

	auto& desc = Get_Owner()->Get_Animator()->Get_TweenDesc();
	if (desc.next.animIndex >= 0)
		return desc.next.currentFrame;
	else
		return desc.curr.currentFrame;
}

_uint FSM::Get_FinalFrame()
{
	if (m_pOwner.expired())
		assert(false);

	if (!Get_Owner()->Get_Animator())
		assert(false);

	auto& desc = Get_Owner()->Get_Animator()->Get_TweenDesc();
	if (desc.next.animIndex >= 0)
		return Get_Owner()->Get_Animator()->Get_FinalFrame(desc.next.animIndex);
	else
		return Get_Owner()->Get_Animator()->Get_FinalFrame(desc.curr.animIndex);
}

_bool FSM::Is_AnimFinished()
{
	if (m_pOwner.expired())
		assert(false);

	if (!Get_Owner()->Get_Animator())
		assert(false);

	auto animator = Get_Owner()->Get_Animator();
	auto& desc = Get_Owner()->Get_Animator()->Get_TweenDesc();
	if (desc.next.animIndex < 0 && animator->Is_Finished())
		return true;
	return false;
}

_float3 FSM::Get_InputDirVector()
{
	auto pCamera = CUR_SCENE->Get_MainCamera();
	if (!pCamera)
		return _float3(0.f);

	_float3 vCameraLook = pCamera->Get_Transform()->Get_State(Transform_State::LOOK).xyz();
	vCameraLook.y = 0;
	vCameraLook.Normalize();
	_float3 vCameraRight = pCamera->Get_Transform()->Get_State(Transform_State::RIGHT).xyz();
	_float3 vCurLook = Get_Transform()->Get_State(Transform_State::LOOK).xyz();
	vCurLook.Normalize();

	_float2 vInput = { 0.f,0.f };
	if (KEYTAP(KEY_TYPE::W) || KEYHOLD(KEY_TYPE::W))
		vInput.x += 1.f;
	if (KEYTAP(KEY_TYPE::A) || KEYHOLD(KEY_TYPE::A))
		vInput.y -= 1.f;
	if (KEYTAP(KEY_TYPE::S) || KEYHOLD(KEY_TYPE::S))
		vInput.x -= 1.f;
	if (KEYTAP(KEY_TYPE::D) || KEYHOLD(KEY_TYPE::D))
		vInput.y += 1.f;

	vInput.Normalize();

	_float3 vInputDir = vInput.x * vCameraLook + vInput.y * vCameraRight;

	vInputDir.Normalize();
	return vInputDir;
}

void FSM::Soft_Turn_ToInputDir(const _float3& vInputDir, _float turnSpeed)
{
	Quaternion qCurRot = Get_Transform()->Get_Rotation();
	Quaternion InputQuaternion = Transform::Get_QuatFromLook(vInputDir);

	Quaternion newQ = Quaternion::Slerp(qCurRot, InputQuaternion, turnSpeed * fDT);

	Get_Transform()->Set_Quaternion(newQ);
}

_float3 FSM::Soft_Turn_ToTarget(const _float3& vTargetPos, _float turnSpeed)
{
	return Soft_Turn_ToTarget(_float4(vTargetPos, 1.f), turnSpeed);
}

_float3 FSM::Soft_Turn_ToTarget(const _float4& vTargetPos, _float turnSpeed)
{
	Quaternion qCurRot = Get_Transform()->Get_Rotation();

	_float3 vDir = (vTargetPos - Get_Transform()->Get_State(Transform_State::POS)).xyz();
	vDir.y = 0.f;
	Quaternion InputQuaternion = Transform::Get_QuatFromLook(vDir);

	Quaternion newQ = Quaternion::Slerp(qCurRot, InputQuaternion, turnSpeed * fDT);

	Get_Transform()->Set_Quaternion(newQ);

	return vDir;
}

void FSM::Set_Target(shared_ptr<GameObject> pTarget)
{
	m_pTarget = pTarget;
}

void FSM::Reset_Target()
{
	m_pTarget.reset();
}
