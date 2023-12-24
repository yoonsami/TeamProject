#pragma once
#include "MonoBehaviour.h"
class MeteorRadialBlur :
    public MonoBehaviour
{
public:
    MeteorRadialBlur(_float fExploseTime, _float fBiggerSpeed, _float fMaintainTime) : m_fExploseTime(fExploseTime),m_fBiggerSpeed(fBiggerSpeed), m_fEndTime(fMaintainTime + fExploseTime) {}
    shared_ptr<MonoBehaviour> Copy_Script() override { return make_shared<MeteorRadialBlur>(m_fExploseTime,m_fBiggerSpeed,m_fEndTime); }

    virtual void Tick() override;

private:
    _float m_fExploseTime = 0.f;
    _float m_fAcc = 0.f;

    _float m_fEndTime = 0.f;

    _float m_fBiggerSpeed = 0.f;
};

