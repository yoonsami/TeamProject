#pragma once
#include "Component.h"


class  MonoBehaviour :
    public Component
{
public:
    MonoBehaviour();
    virtual ~MonoBehaviour();

    virtual HRESULT Init() override;
    virtual void Tick() override;


};
