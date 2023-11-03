#pragma once
#include "MonoBehaviour.h"
class DemoCameraScript2 :
    public MonoBehaviour
{
public:
    virtual void Tick() override;

private:
    _float m_fRotationSpeed = XM_PI / 4.f;
};

