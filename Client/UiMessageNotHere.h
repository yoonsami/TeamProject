#pragma once
#include "MonoBehaviour.h"

class UiMessageNotHere : public MonoBehaviour
{
public:
    UiMessageNotHere();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UiMessageNotHere>(); }

    void Create_Message();

private:
    void IDLE();
    void DECREASE();
    void REMOVE();

private:
    weak_ptr<GameObject> m_pBg;
    weak_ptr<GameObject> m_pFont;

    enum class MESSAGE_STATE { IDLE, DECREASE, REMOVE, NONE };
    MESSAGE_STATE   m_eState    = { MESSAGE_STATE::NONE };

    _float  m_fMaxIdleTime      = {};
    _float  m_fMaxDecreaseTime  = {};
    _float  m_fCheckTime        = {};
    _float  m_fDecreaseRatio    = {};

    _bool   m_bIsCreated        = {};
};

