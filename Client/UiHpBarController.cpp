#include "pch.h"
#include "UiHpBarController.h"

#include "MeshRenderer.h"
#include "FontRenderer.h"

UiHpBarController::UiHpBarController(_uint iType)
    : m_iObjType(iType)
{
}

HRESULT UiHpBarController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_fMaxHp = m_pOwner.lock()->Get_MaxHp();
    m_fPreHp = m_pOwner.lock()->Get_CurHp();

    auto pScene = CUR_SCENE;
    switch (m_iObjType)
    {
    case 0:
        m_pFrontHp  = pScene->Get_UI(L"UI_HpBar");
        m_pBackHp   = pScene->Get_UI(L"UI_HpBar1");
        m_pFont     = pScene->Get_UI(L"UI_HpFont");
        break;
    case 1:
        break;
    }

    return S_OK;
}

void UiHpBarController::Tick()
{
	if (m_pOwner.expired())
		return;

    Change_Param();
    
    Change_Hp_Ratio();

    Change_Hp_Slow();

    if (KEYTAP(KEY_TYPE::F5))
        m_pOwner.lock()->Get_Hurt(10.f);
}

void UiHpBarController::Change_Param()
{
    if (true == m_pFrontHp.expired())
        return;

    m_fValue += fDT * 0.2f;
    m_pFrontHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[1] = m_fValue;

    wstring strHp = to_wstring(IDX(m_pOwner.lock()->Get_CurHp()));
    strHp += L"/";
    strHp += to_wstring(IDX(m_pOwner.lock()->Get_MaxHp()));

    m_pFont.lock()->Get_FontRenderer()->Get_Text() = strHp;
    _float4 vecPos = m_pFont.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.x = strHp.length() * 10.f / -2.f;
    m_pFont.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
}

void UiHpBarController::Change_Hp_Ratio()
{
    if (true == m_pFrontHp.expired() || true == m_pBackHp.expired())
        return;

    _float HpRatio = m_pOwner.lock()->Get_HpRatio();
    m_pFrontHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = HpRatio;

    m_fRatio = m_pBackHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0];

    if (HpRatio != m_fRatio && false == m_bIsWork)
    {
        m_bIsWork = true;
        m_fSpeed = m_fRatio - HpRatio;
        m_fTargetRatio = HpRatio;
    }
}

void UiHpBarController::Change_Hp_Slow()
{
    if (true == m_pFrontHp.expired() || true == m_pBackHp.expired())
        return;

    if(false == m_bIsWork)
        return;

    m_fRatio -= m_fSpeed * fDT;
    if (m_fRatio < m_fTargetRatio)
    {
        m_fRatio = m_fTargetRatio;
        m_pBackHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_fTargetRatio;
        m_bIsWork = false;
    }
    else
    {
        m_pBackHp.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_fRatio;
    }

}