#pragma once
#include "MonoBehaviour.h"
class MeteorRadialBlur :
    public MonoBehaviour
{
public:
    MeteorRadialBlur();
    shared_ptr<MonoBehaviour> Copy_Script() override { return make_shared<MeteorRadialBlur>(); }
};

