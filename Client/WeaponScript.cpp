#include "pch.h"
#include "WeaponScript.h"
#include "Model.h"
#include "ModelAnimator.h"
#include "Utils.h"


WeaponScript::WeaponScript(WEAPONDESC weapondesc)
{
	m_strBoneName = weapondesc.strBoneName;
	m_matPivot = weapondesc.matPivot;
	m_pWeaponOwner = weapondesc.pWeaponOwner;
}

WeaponScript::~WeaponScript()
{
}

HRESULT WeaponScript::Init()
{
	_uint index = m_pWeaponOwner.lock()->Get_Model()->Get_BoneIndexByName(m_strBoneName);

	//MEMO :: current animation's selected Index bonematrix
	_float4x4 matSetting = m_pWeaponOwner.lock()->Get_Animator()->Get_CurAnimTransform(index);

	Get_Transform()->Set_WorldMat(m_matPivot * matSetting);
	Get_Transform()->Scaled(_float3(1.f));

	return S_OK;
}

void WeaponScript::Tick()
{
	// weak_ptr expired() -> 이 포인터가 이미 소멸해서 사용할 수 없는 상태면 true 반환
	if (m_pOwner.expired() || m_pWeaponOwner.expired())
		return;

	_uint index = m_pWeaponOwner.lock()->Get_Model()->Get_BoneIndexByName(m_strBoneName);

	_float4x4 matSetting = m_pWeaponOwner.lock()->Get_Animator()->Get_CurAnimTransform(index)
		* Utils::m_matPivot * m_pWeaponOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

	Get_Transform()->Set_WorldMat(m_matPivot * matSetting);
	Get_Transform()->Scaled(_float3(1.f));
}
