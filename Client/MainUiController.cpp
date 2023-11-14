#include "pch.h"
#include "MainUiController.h" 

MainUiController::MainUiController()
{
}

HRESULT MainUiController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_vecUi.resize(35);

    auto pScene = CUR_SCENE;
    m_vecUi[0] = pScene->Get_UI(L"UI_HpBar");
    m_vecUi[1] = pScene->Get_UI(L"UI_HpBarBg");
    m_vecUi[2] = pScene->Get_UI(L"UI_SkillBg");
    m_vecUi[3] = pScene->Get_UI(L"UI_Skill_Gauge");
    m_vecUi[4] = pScene->Get_UI(L"UI_Skill0");
    m_vecUi[5] = pScene->Get_UI(L"UI_Skill1");
    m_vecUi[6] = pScene->Get_UI(L"UI_Skill2");
    m_vecUi[7] = pScene->Get_UI(L"UI_Skill3");
    m_vecUi[8] = pScene->Get_UI(L"UI_Skill4");
    m_vecUi[9] = pScene->Get_UI(L"UI_Skill5");
    m_vecUi[10] = pScene->Get_UI(L"UI_Skill6");
    m_vecUi[11] = pScene->Get_UI(L"UI_Skill0_Effect");
    m_vecUi[12] = pScene->Get_UI(L"UI_Skill2_Effect");
    m_vecUi[13] = pScene->Get_UI(L"UI_Skill3_Effect");
    m_vecUi[14] = pScene->Get_UI(L"UI_Skill4_Effect");
    m_vecUi[15] = pScene->Get_UI(L"UI_Skill5_Effect");
    m_vecUi[16] = pScene->Get_UI(L"UI_Skill6_Effect");
    m_vecUi[17] = pScene->Get_UI(L"UI_Cool_End2");
    m_vecUi[18] = pScene->Get_UI(L"UI_Cool_End3");
    m_vecUi[19] = pScene->Get_UI(L"UI_Cool_End4");
    m_vecUi[20] = pScene->Get_UI(L"UI_Cool_End5");
    m_vecUi[21] = pScene->Get_UI(L"UI_Cool_End6");
    m_vecUi[22] = pScene->Get_UI(L"UI_Main_Button0");
    m_vecUi[23] = pScene->Get_UI(L"UI_Main_Button1");
    m_vecUi[24] = pScene->Get_UI(L"UI_Main_Button2");
    m_vecUi[25] = pScene->Get_UI(L"UI_Main_Button3");
    m_vecUi[26] = pScene->Get_UI(L"UI_Char_Change0");
    m_vecUi[27] = pScene->Get_UI(L"UI_Char_Change1");
    m_vecUi[28] = pScene->Get_UI(L"UI_Char_Change2");
    m_vecUi[29] = pScene->Get_UI(L"UI_Char_Change_Line0");
    m_vecUi[30] = pScene->Get_UI(L"UI_Char_Change_Line1");
    m_vecUi[31] = pScene->Get_UI(L"UI_Char_Change_Line2");
    m_vecUi[32] = pScene->Get_UI(L"UI_Char_Change_Element0");
    m_vecUi[33] = pScene->Get_UI(L"UI_Char_Change_Element1");
    m_vecUi[34] = pScene->Get_UI(L"UI_Char_Change_Element2");

    return S_OK;
}

void MainUiController::Tick()
{
	if (m_pOwner.expired())
		return;

  
}

void MainUiController::Set_MainUI_Render(_bool bValue)
{
    _uint iSize = IDX(m_vecUi.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecUi[i].expired())
            m_vecUi[i].lock()->Set_Render(bValue);
    }
}
