#pragma once
#include "MonoBehaviour.h"

class PointLightWave :
    public MonoBehaviour
{
    PointLightWave(_float4 _vecCenterColor, _float _fWeight);
    PointLightWave(_float4 _vecStartColor, _float4 _vecEndColor);
    virtual ~PointLightWave() = default;

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

private:
    _float4 m_colStartColor = { 0.f, 0.f, 0.f, 0.f };
    _float4 m_colEndColor = { 0.f, 0.f, 0.f, 0.f };
    _float m_fDeltaTime = { 0.f };
    // 델타의 증가량의 부호 Plus 1 or Minus 1
    _float m_fDeltaPM = { 1.f };
};

