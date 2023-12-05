#pragma once
#include "MonoBehaviour.h"
class WaterUVSliding :
    public MonoBehaviour
{
public:
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script()override { return make_shared<WaterUVSliding>(); }

};

