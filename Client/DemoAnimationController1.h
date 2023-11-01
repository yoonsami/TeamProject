#pragma once
#include "MonoBehaviour.h"
class DemoAnimationController1 :
    public MonoBehaviour
{
public:
    virtual void Tick() override;;

private:
    _int m_iCurAnimIndex = 0;
};

