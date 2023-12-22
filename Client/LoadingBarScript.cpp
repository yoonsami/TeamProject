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

    m_bIsLoadEnd = false;

    return S_OK;
}

void LoadingBarScript::Tick()
{
	if (m_pOwner.expired())
		return;

    if(false == m_bIsLoadEnd)
        Change_Text();
}

void LoadingBarScript::Change_Text()
{
    for (_uint i = 0; i < IDX(m_addedObj.size()); ++i)
        if (true == m_addedObj[i].expired())
            return;

    m_iTargetNum = m_iCurLoadIndex * 10;
    if (100 < m_iTargetNum)
    {
        m_iTargetNum = 100;
        m_bIsLoadEnd = true;
    }

    if (m_iCurNum < m_iTargetNum)
    {
        ++m_iCurNum;
    }

    _float fTemp = static_cast<_float>(m_iCurNum);
    fTemp /= 100.f;
    m_addedObj[m_iLoadingBarIndex].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = fTemp;

    wstring strTemp = to_wstring(m_iCurNum) + L" %";
    m_addedObj[m_iNumIndex].lock()->Get_FontRenderer()->Get_Text() = strTemp;
}
