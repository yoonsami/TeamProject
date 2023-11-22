#include "pch.h"
#include "UIBossHpBar.h" 

#include "MeshRenderer.h"
#include "FontRenderer.h"

UIBossHpBar::UIBossHpBar(BOSS eBoss)
    : m_eBoss(eBoss)
{
}

HRESULT UIBossHpBar::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    auto pScene = CUR_SCENE;
    m_pBgHp         = pScene->Get_UI(L"UI_BossHpBar0");
    m_pFrontHp      = pScene->Get_UI(L"UI_BossHpBar1");
    m_pBackHp       = pScene->Get_UI(L"UI_BossHpBar2");
    m_pHpFont       = pScene->Get_UI(L"UI_BossHpFont");
    m_pBgGroggy     = pScene->Get_UI(L"UI_BossGroggy0");
    m_pFrontGroggy  = pScene->Get_UI(L"UI_BossGroggy1");
    m_pElement      = pScene->Get_UI(L"UI_BossElement");
    m_pBossName     = pScene->Get_UI(L"UI_BossName");

    if (true == m_pElement.expired() || true == m_pBossName.expired())
        return E_FAIL;

    if (BOSS::MAX != m_eBoss)
    {
        auto BossData = GET_DATA(m_eBoss);

        wstring strName = BossData.Name;
        m_pBossName.lock()->Get_FontRenderer()->Get_Text() = strName;
        _float4 vecPos = m_pBossName.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
        vecPos.x = strName.length() / 2.f * -21.f;
        m_pBossName.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);

        vecPos.y = m_pElement.lock()->GetOrAddTransform()->Get_State(Transform_State::POS).y;
        vecPos.x -= 20.f;
        m_pElement.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
        m_pElement.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(GET_ELEMENT(m_eBoss)), TextureMapType::DIFFUSE);
    }
    

    return S_OK;
}

void UIBossHpBar::Tick()
{
	if (m_pOwner.expired()          ||
        m_pBgHp.expired()           ||
        m_pFrontHp.expired()        ||
        m_pBackHp.expired()         ||
        m_pHpFont.expired()         ||
        m_pBgGroggy.expired()       ||
        m_pFrontGroggy.expired()    ||
        m_pElement.expired()        ||
        m_pBossName.expired())
		return;

    Check_Target();
    Change_Hp_Ratio();
    Change_Hp_Slow();
    Change_Param();

    if (KEYTAP(KEY_TYPE::F5))
        m_pOwner.lock()->Get_Hurt(10.f);
}

void UIBossHpBar::Check_Target()
{
    if (0.f >= m_pOwner.lock()->Get_CurHp())
    {
        auto pScene = CUR_SCENE;
        pScene->Remove_GameObject(m_pBgHp.lock());
        pScene->Remove_GameObject(m_pFrontHp.lock());
        pScene->Remove_GameObject(m_pBackHp.lock());
        pScene->Remove_GameObject(m_pHpFont.lock());
        pScene->Remove_GameObject(m_pBgGroggy.lock());
        pScene->Remove_GameObject(m_pFrontGroggy.lock());
        pScene->Remove_GameObject(m_pElement.lock());
        pScene->Remove_GameObject(m_pBossName.lock());
        pScene->Remove_GameObject(m_pOwner.lock());
    }
}

void UIBossHpBar::Change_Hp_Ratio()
{
    m_pFrontGroggy.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = 1.f - m_pOwner.lock()->Get_Groggy_Gauge_Ratio();

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

void UIBossHpBar::Change_Hp_Slow()
{
    if (false == m_bIsWork)
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

void UIBossHpBar::Change_Param()
{
    wstring strHp = to_wstring(IDX(m_pOwner.lock()->Get_CurHp()));
    strHp += L" / ";
    strHp += to_wstring(IDX(m_pOwner.lock()->Get_MaxHp()));

    m_pHpFont.lock()->Get_FontRenderer()->Get_Text() = strHp;
    _float4 vecPos = m_pHpFont.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.x = ((strHp.length() - 3) / 2.f * -10.f) - 10.f;
    m_pHpFont.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
}
