#pragma once
#include "MonoBehaviour.h"
class CounterMotionTrailScript :
    public MonoBehaviour
{

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

private:
    weak_ptr<GameObject> m_pMotionTrail;
    COOLTIMEINFO m_tTrailBiggerCool = { 0.1f, 0.f };
    _float3 m_vTrailScale = _float3(1.f);
    _float m_fTrailScale = 1.f;

};

