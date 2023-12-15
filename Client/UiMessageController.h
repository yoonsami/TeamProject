#pragma once
#include "MonoBehaviour.h"

class UiMessageController : public MonoBehaviour
{
public:
    UiMessageController(wstring strMessage, shared_ptr<GameObject> pTarget, _bool bIsBg, _uint iType = 0);

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UiMessageController>(L"", nullptr, true); }

private:
    void Check_Distance();
    void Change_Pos();
    void IDLE();
    void DECREASE();
    void REMOVE();

    void Check_In_Camera();

private:
    weak_ptr<GameObject>    m_pCamera;
    weak_ptr<GameObject>    m_pTarget;
    _bool   m_bIsBg = {};

    enum class MESSAGE_STATE { IDLE, DECREASE, REMOVE, NONE };
    MESSAGE_STATE   m_eState    = { MESSAGE_STATE::NONE };

    _float  m_fMaxIdleTime      = {};
    _float  m_fMaxDecreaseTime  = {};
    _float  m_fCheckTime        = {};
    _float  m_fDecreaseRatio    = {};
    _uint   m_iType             = {};

    _float  m_fValuePosX        = {};
    _float  m_fValuePosY        = {};
    wstring m_strMessage;
};

