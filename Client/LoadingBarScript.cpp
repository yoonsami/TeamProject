#include "pch.h"
#include "LoadingBarScript.h"

#include "MeshRenderer.h"
#include "FontRenderer.h"

LoadingBarScript::LoadingBarScript()
{
}

HRESULT LoadingBarScript::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    auto pScene = CUR_SCENE;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Loading_Bar.dat", m_addedObj);

    _uint iSize = IDX(m_addedObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& pObj = m_addedObj[i];
        if (true == pObj.expired())
            continue;

        wstring strName = pObj.lock()->Get_Name();
        if (L"UI_Loading_Bar1" == strName)
        {
            m_iLoadingBarIndex = i;
        }

        else if (L"UI_Loading_Text" == strName)
        {
            m_iTextIndex = i;
        }

        else if (L"UI_Loading_Num" == strName)
        {
            m_iNumIndex = i;
        }

    }
    m_bIsEndActivated = false;
    m_bIsLoadEnd = false;
    m_fMaxTime = 1.f;
    m_fMaxAddTime = 0.1f;

    return S_OK;
}

void LoadingBarScript::Tick()
{
	if (m_pOwner.expired())
		return;

    if (false == m_bIsEndActivated)
        Change_Text();
    else if(true == m_bIsEndActivated)
        Check_Time();
}

void LoadingBarScript::Change_Text()
{
    for (_uint i = 0; i < IDX(m_addedObj.size()); ++i)
        if (true == m_addedObj[i].expired())
            return;

    m_iTargetNum = m_iCurLoadIndex * 10;
    if (100 <= m_iTargetNum && 100 <= m_iCurNum)
    {
        m_iTargetNum = 100;
        m_bIsEndActivated = true;
    }

    m_fAddValueTime += fDT;
    if (m_fMaxAddTime < m_fAddValueTime && m_iCurNum < m_iTargetNum)
    {
        m_fAddValueTime = 0.f;
        ++m_iCurNum;
    }

    _float fTemp = static_cast<_float>(m_iCurNum);
    fTemp /= 100.f;
    m_addedObj[m_iLoadingBarIndex].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = fTemp;

    wstring strTemp = to_wstring(m_iCurNum) + L" %";
    m_addedObj[m_iNumIndex].lock()->Get_FontRenderer()->Get_Text() = strTemp;

    m_fCheckTime += fDT;
    if (m_fMaxTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;

        m_iTextDotCount++;
        if (3 == m_iTextDotCount)
            m_iTextDotCount = 0;
    }

    if (0 == m_iTextDotCount)
        m_addedObj[m_iTextIndex].lock()->Get_FontRenderer()->Get_Text() = L"리소스 로딩 중입니다.";
    else if (1 == m_iTextDotCount)
        m_addedObj[m_iTextIndex].lock()->Get_FontRenderer()->Get_Text() = L"리소스 로딩 중입니다..";
    else if (2 == m_iTextDotCount)
        m_addedObj[m_iTextIndex].lock()->Get_FontRenderer()->Get_Text() = L"리소스 로딩 중입니다...";

}

void LoadingBarScript::Check_Time()
{
    m_fCheckEndTime += fDT;
    if (m_fMaxTime < m_fCheckEndTime)
        m_bIsLoadEnd = true;
}
