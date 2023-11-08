#pragma once
#include "MonoBehaviour.h"

class UiLoadingScript : public MonoBehaviour
{
public:
    UiLoadingScript(_bool bIsEven);

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    void Change_Texture();

private:
    void On_Idle();
    void Off();
    void Change();
    void Off_Idle();
    void On();

private:
    _bool m_bIsEven = { false };
    vector<wstring> m_strEvenTextureName;
    vector<wstring> m_strOddTextureName;

    enum class CHANGE_TYPE { ON_IDLE, OFF, CHANGE, OFF_IDLE, ON };
    CHANGE_TYPE     m_eType = { CHANGE_TYPE::ON_IDLE };

    _float          m_fMaxIdleTime      = { 3.f };
    _float          m_fOnOffTime        = { 2.f };
    _float          m_fCheckTime        = { 0.f };
    _uint           m_iIndex            = { 0 };

};

