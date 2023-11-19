#pragma once
#include "MonoBehaviour.h"

class UiSkillGauge : public MonoBehaviour
{
public:
    UiSkillGauge();


public:
    virtual HRESULT Init() override;

    void Change_Ratio(_float fRatio);
    void Change_Render(_bool bSet);

private:


private:
    weak_ptr<GameObject> m_pGaugeBg;

    _float  m_fMinGauge = {};
    _float  m_fMaxGauge = {};
    _float  m_fValue    = {};
    _bool   m_bIsRender = {};
};

