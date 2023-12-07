#include "pch.h"
#include "UiMessageNotHere.h" 

#include "FontRenderer.h"
#include "MeshRenderer.h"

UiMessageNotHere::UiMessageNotHere()
{
}

HRESULT UiMessageNotHere::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    m_fMaxIdleTime = 0.5f;
    m_fMaxDecreaseTime = 1.f;
    m_fCheckTime = 0.f;;
    m_fDecreaseRatio = 1.f / m_fMaxDecreaseTime;


    return S_OK;
}

void UiMessageNotHere::Tick()
{
    if (true == m_pOwner.expired())
        return;

    switch (m_eState)
    {
    case MESSAGE_STATE::IDLE:
        IDLE();
        break;
    case MESSAGE_STATE::DECREASE:
        DECREASE();
        break;
    case MESSAGE_STATE::REMOVE:
        REMOVE();
        break;
    case MESSAGE_STATE::NONE:
        break;
    }
}

void UiMessageNotHere::Create_Message()
{
    if (true == m_bIsCreated)
        return;

    m_bIsCreated = true;
    m_eState = MESSAGE_STATE::IDLE;
    m_fCheckTime = 0.f;

    auto pScene = CUR_SCENE;
    vector<weak_ptr<GameObject>> addedObj;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Message_NotHere.dat", addedObj);

    _uint iSize = IDX(addedObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& pObj = addedObj[i];
        if (true == pObj.expired())
            continue;

        wstring strName = pObj.lock()->Get_Name();
        if (L"UI_Message_NotHere_Bg" == strName)
            m_pBg = pObj;
        else if (L"UI_Message_NotHere_Font" == strName)
            m_pFont = pObj;
    }
}

void UiMessageNotHere::IDLE()
{
    m_fCheckTime += fDT;
    if (m_fMaxIdleTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;
        m_eState = MESSAGE_STATE::DECREASE;
    }
}

void UiMessageNotHere::DECREASE()
{
    // 1 -> 0
    m_fCheckTime += fDT;
    if (m_fMaxDecreaseTime < m_fCheckTime)
    {
        m_fCheckTime = m_fMaxDecreaseTime;
        m_eState = MESSAGE_STATE::REMOVE;
    }

    _float fTemp = 1.f - (m_fCheckTime / m_fMaxDecreaseTime) * m_fDecreaseRatio;
    if (false == m_pBg.expired())
        m_pBg.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = fTemp;
    if (false == m_pFont.expired())
        m_pFont.lock()->Get_FontRenderer()->Get_Color() = Color(fTemp, fTemp, fTemp, fTemp);
}

void UiMessageNotHere::REMOVE()
{
    if (true == m_pBg.expired() ||
        true == m_pFont.expired())
        return;

    m_eState = MESSAGE_STATE::NONE;

    EVENTMGR.Delete_Object(m_pBg.lock());
    EVENTMGR.Delete_Object(m_pFont.lock());

    m_bIsCreated = false;
}