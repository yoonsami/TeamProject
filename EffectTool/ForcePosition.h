#pragma once
#include "MonoBehaviour.h"
class ForcePosition :
    public MonoBehaviour
{
public:
    virtual void Final_Tick() override;


public:
    _bool bForcePosition = false;
};

