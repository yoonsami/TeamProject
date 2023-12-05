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
	_float4x4 matSetting = m_pWeaponOwner.lock()->Get_Animator()->Get_CurAnimTransform(index)
		* _float4x4::CreateRotationX(XM_PI * -0.5f) * Utils::m_matPivot * m_pWeaponOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

	Get_Transform()->Set_WorldMat(m_matPivot * matSetting);
	Get_Transform()->Scaled(_float3(1.f));

	return S_OK;
}

void WeaponScript::Late_Tick()
{
	// weak_ptr expired() -> �� �����Ͱ� �̹� �Ҹ��ؼ� ����� �� ���� ���¸� true ��ȯ
	if (m_pOwner.expired() || m_pWeaponOwner.expired())
		return;

	if (Get_Owner()->Get_Name().empty())
		return;


	_uint index = m_pWeaponOwner.lock()->Get_Model()->Get_BoneIndexByName(m_strBoneName);

	_float4x4 matSetting = m_pWeaponOwner.lock()->Get_Animator()->Get_CurAnimTransform(index)
		* _float4x4::CreateRotationX(XM_PI * -0.5f) * Utils::m_matPivot * m_pWeaponOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

	Get_Transform()->Set_WorldMat(m_matPivot * matSetting);
	Get_Transform()->Scaled(_float3(1.f));

	if (m_bModelChanged)
	{
		m_bModelChanged = false;
		Get_Transform()->Reset_Position();
	}
}
