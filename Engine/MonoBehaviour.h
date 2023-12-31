#pragma once
#include "Component.h"

class  MonoBehaviour : public Component
{
public:
    MonoBehaviour();
    virtual ~MonoBehaviour();

    virtual HRESULT Init()      override;
    virtual void Tick()         override;
    virtual void Late_Tick()    override;

protected:
    _bool   m_bIsInit = { false };
};
