#pragma once
#include "MonoBehaviour.h"
class Debug_CreateMotionTrail :
    public MonoBehaviour
{
public:
    virtual void Tick() override;

private:
    _float m_fAcc = 0.f;
};

