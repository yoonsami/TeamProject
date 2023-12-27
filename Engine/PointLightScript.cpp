#include "pch.h"
#include "PointLightScript.h"
#include "Light.h"
#include "GroupEffectOwner.h"
#include "Utils.h"

PointLightScript::PointLightScript()
{
}

HRESULT PointLightScript::Init()
{
	return S_OK;
}

void PointLightScript::Clear()
{
	if (Get_Owner()->Get_Light() == nullptr)
		return;

	// 기본초기화
	m_AmbientStartColor = Get_Owner()->Get_Light()->Get_LightInfo().color.ambient;
	m_AmbientEndColor = Get_Owner()->Get_Light()->Get_LightInfo().color.ambient;
	m_DiffuseStartColor = Get_Owner()->Get_Light()->Get_LightInfo().color.diffuse;
	m_DiffuseEndColor = Get_Owner()->Get_Light()->Get_LightInfo().color.diffuse;
	m_fStartRange = Get_Owner()->Get_Light()->Get_LightInfo().range;
	m_fEndRange = Get_Owner()->Get_Light()->Get_LightInfo().range;
	m_fDeltaStart = 0.f;
	m_fDeltaTime = 0.f;
	m_fEffectSpeed = 1.f;
}

HRESULT PointLightScript::Init_Reverse()
{
	LightInfo& OwnerLightInfo = Get_Owner()->Get_Light()->Get_LightInfo();
	OwnerLightInfo.color.ambient = m_AmbientStartColor;
	OwnerLightInfo.color.diffuse = m_DiffuseStartColor;
	OwnerLightInfo.range = m_fStartRange;
	return S_OK;
}

void PointLightScript::Tick()
{
	if(m_bUseEffect)
	{
		m_fDeltaTime += fDT * m_fDeltaPM * m_fEffectSpeed;
		// m_fDeltaTime은 0~1을 반복
		if (m_fDeltaTime > 1.f)
		{
			m_fDeltaPM = -1.f;
			m_fDeltaTime = 1.f;
		}
		else if (m_fDeltaTime < 0.f)
		{
			if (m_strEffectName != "")
				Add_GroupEffectOwner(Utils::ToWString(m_strEffectName), _float3{0.f, 0.f, 0.f}, false, nullptr);
			m_fDeltaPM = 1.f;
			m_fDeltaTime = 0.f;
		}

		_float4 AmbientColor = _float4::Lerp(m_AmbientStartColor, m_AmbientEndColor, m_fDeltaTime);
		Get_Owner()->Get_Light()->Set_Ambient(AmbientColor);
		_float4 DiffuseColor = _float4::Lerp(m_DiffuseStartColor, m_DiffuseEndColor, m_fDeltaTime);
		Get_Owner()->Get_Light()->Set_Diffuse(DiffuseColor);
		_float fRange = Lerp(m_fStartRange, m_fEndRange, m_fDeltaTime);
		Get_Owner()->Get_Light()->Set_LightRange(fRange);
	}
}

_float PointLightScript::Lerp(_float& f1, _float& f2, _float _fWeight)
{
	return f1 + _fWeight * (f2 - f1);
}

void PointLightScript::Add_GroupEffectOwner(const wstring& strSkilltag, _float3 vPosOffset, _bool usePosAs, shared_ptr<MonoBehaviour> pScript)
{
	shared_ptr<GameObject> pGroupEffectOwnerObj = make_shared<GameObject>();

	// For. Transform 
	pGroupEffectOwnerObj->GetOrAddTransform();
	if (!usePosAs)
	{
		_float4 vOwnerLook = m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::LOOK);
		vOwnerLook.Normalize();
		_float4 vOwnerRight = m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::RIGHT);
		vOwnerRight.Normalize();
		_float4 vOwnerUp = m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::UP);
		vOwnerUp.Normalize();
		_float4 vOwnerPos = m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS)
			+ vOwnerRight * vPosOffset.x
			+ vOwnerUp * vPosOffset.y
			+ vOwnerLook * vPosOffset.z;
		pGroupEffectOwnerObj->Get_Transform()->Set_State(Transform_State::POS, vOwnerPos);
	}
	else
		pGroupEffectOwnerObj->Get_Transform()->Set_State(Transform_State::POS, _float4(vPosOffset, 1.f));
	pGroupEffectOwnerObj->Get_Transform()->Set_Quaternion(Get_Transform()->Get_Rotation());

	// For. GroupEffect component 
	shared_ptr<GroupEffectOwner> pGroupEffect = make_shared<GroupEffectOwner>();
	pGroupEffectOwnerObj->Add_Component(pGroupEffect);
	pGroupEffectOwnerObj->Get_GroupEffectOwner()->Set_GroupEffectTag(strSkilltag);
	if (nullptr != pScript)
		pGroupEffectOwnerObj->Get_GroupEffectOwner()->Set_GroupEffectScript(pScript);

	// For. Add Effect GameObject to current scene
	EVENTMGR.Create_Object(pGroupEffectOwnerObj);
}
