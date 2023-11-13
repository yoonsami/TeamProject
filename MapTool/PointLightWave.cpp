#include "pch.h"
#include "PointLightWave.h"

PointLightWave::PointLightWave(_float4 _vecCenterColor, _float _fWeight)
{
	_vecCenterColor;
}

PointLightWave::PointLightWave(_float4 _vecStartColor, _float4 _vecEndColor)
{
	m_colStartColor = _vecStartColor;
	m_colEndColor = _vecEndColor;
}

HRESULT PointLightWave::Init()
{
	

	return S_OK;
}

void PointLightWave::Tick()
{
	m_fDeltaTime += fDT * m_fDeltaPM;
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

	_float4 Color = _float4::Lerp(m_colStartColor, m_colEndColor, m_fDeltaTime);


}