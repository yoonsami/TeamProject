#pragma once
#include "MonoBehaviour.h"
class WaterUVSliding :
    public MonoBehaviour
{
public:
    virtual void Tick() override;

public:
    void Set_Color1(_float4 _Color1) { m_Color1 = _Color1; }
    void Set_Color2(_float4 _Color2) { m_Color2 = _Color2; }
    virtual shared_ptr<MonoBehaviour> Copy_Script() { return nullptr; }
    
private:
    _float4 m_Color1 = { 1.f, 1.f, 1.f ,1.f };
    _float4 m_Color2 = { 1.f, 1.f, 1.f ,1.f };
};

