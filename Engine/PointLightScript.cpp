#include "pch.h"
#include "PointLightScript.h"
#include "Light.h"

PointLightScript::PointLightScript()
{
}

HRESULT PointLightScript::Init()
{
	if (Get_Owner()->Get_Light() == nullptr)
		return E_FAIL;

	//// 기본초기화
	//m_AmbientStartColor = Get_Owner()->Get_Light()->Get_LightInfo().color.ambient;
	//m_AmbientEndColor = Get_Owner()->Get_Light()->Get_LightInfo().color.ambient;
	//m_DiffuseStartColor = Get_Owner()->Get_Light()->Get_LightInfo().color.diffuse;
	//m_DiffuseEndColor = Get_Owner()->Get_Light()->Get_LightInfo().color.diffuse;
	//m_fStartRange = Get_Owner()->Get_Light()->Get_LightInfo().range;
	//m_fEndRange = Get_Owner()->Get_Light()->Get_LightInfo().range;
	//m_fEffectSpeed = 1.f;

	return S_OK;
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
