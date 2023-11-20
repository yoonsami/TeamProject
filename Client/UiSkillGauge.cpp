#include "pch.h"
#include "UiSkillGauge.h"

#include "MeshRenderer.h"

UiSkillGauge::UiSkillGauge()
{
}

HRESULT UiSkillGauge::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    auto pScene = CUR_SCENE;
    m_pGaugeBg  = pScene->Get_UI(L"UI_Skill_Use_GaugeBg");

    m_pOwner.lock()->Set_Render(false);
    m_pGaugeBg.lock()->Set_Render(false);
    m_bIsRender = false;

    m_fMinGauge = 0.44f;
    m_fMaxGauge = 0.08f;
    m_fValue = m_fMinGauge - m_fMaxGauge;

    return S_OK;
}

void UiSkillGauge::Change_Ratio(_float fRatio)
{
    if (true == m_pOwner.expired() ||
        true == m_pGaugeBg.expired())
        return;

    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_fMinGauge - m_fValue * fRatio;
}

void UiSkillGauge::Change_Render(_bool bSet)
{
    if (true == m_pOwner.expired() ||
        true == m_pGaugeBg.expired())
        return;

    m_bIsRender = bSet;
    m_pOwner.lock()->Set_Render(bSet);
    m_pGaugeBg.lock()->Set_Render(bSet);
}
