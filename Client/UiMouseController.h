#pragma once
#include "MonoBehaviour.h"

class UiMouseController : public MonoBehaviour
{
public:
    UiMouseController();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

private:
    void Change_Mouse_State();
    void Start_Effect();
    void Start();
    void Change_Mouse_Pos();

private:
    weak_ptr<GameObject> m_pMouse;

    enum class STATE { START, NONE };
    STATE   m_eState        = { STATE::NONE };

    _float  m_fMaxTime      = {};
    _float  m_fCheckTime    = {};
    _float  m_fRatio        = {};

    _bool   m_bIsCanMove    = { true };
};

