#pragma once
#include "MonoBehaviour.h"

class UiMouseClick : public MonoBehaviour
{
public:
    UiMouseClick();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

private:
    void Start_Effect();
    void Start();

private:
    enum class STATE { START, NONE };
    STATE   m_eState        = { STATE::NONE };

    _float  m_fMaxTime      = {};
    _float  m_fCheckTime    = {};
    _float  m_fRatio        = {};

};

