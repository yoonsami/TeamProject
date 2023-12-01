#pragma once
#include "MonoBehaviour.h"
class TimerScript :
    public MonoBehaviour
{
public:
    TimerScript(_float fTime);
public:
    virtual void Tick() override;
    void Set_Function(function<void(void)> func) { m_Func = func; }

private:
    _float m_fAcc = 0.f;
    _float m_fTime = 0.f;
    function<void(void)> m_Func = nullptr;
};

