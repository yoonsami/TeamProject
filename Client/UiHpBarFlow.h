#pragma once
#include "MonoBehaviour.h"

class UiHpBarFlow : public MonoBehaviour
{
public:
    UiHpBarFlow();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

private:
    _float m_fValue = { 0.f };
};

