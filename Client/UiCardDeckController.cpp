#include "pch.h"
#include "UiCardDeckController.h" 

UiCardDeckController::UiCardDeckController()
{
}

HRESULT UiCardDeckController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;
    
    auto pScene = CUR_SCENE;

    m_vecCardDeckObj.resize(15);
    m_vecCardDeckObj[0]     = pScene->Get_UI(L"UI_Card_Deck_Total_Bg0");
    m_vecCardDeckObj[1]     = pScene->Get_UI(L"UI_Card_Deck_Total_Bg1");
    m_vecCardDeckObj[2]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Bg");
    m_vecCardDeckObj[3]     = pScene->Get_UI(L"UI_Card_Deck0");
    m_vecCardDeckObj[4]     = pScene->Get_UI(L"UI_Card_Deck1");
    m_vecCardDeckObj[5]     = pScene->Get_UI(L"UI_Card_Deck2");
    m_vecCardDeckObj[6]     = pScene->Get_UI(L"UI_Card_Deck_Line0");
    m_vecCardDeckObj[7]     = pScene->Get_UI(L"UI_Card_Deck_Line1");
    m_vecCardDeckObj[8]     = pScene->Get_UI(L"UI_Card_Deck_Line2");
    m_vecCardDeckObj[9]     = pScene->Get_UI(L"UI_Card_Deck_X0");
    m_vecCardDeckObj[10]    = pScene->Get_UI(L"UI_Card_Deck_X1");
    m_vecCardDeckObj[11]    = pScene->Get_UI(L"UI_Card_Deck_X2");
    m_vecCardDeckObj[12]    = pScene->Get_UI(L"UI_Card_Deck_Element0");
    m_vecCardDeckObj[13]    = pScene->Get_UI(L"UI_Card_Deck_Element1");
    m_vecCardDeckObj[14]    = pScene->Get_UI(L"UI_Card_Deck_Element2");
    
    m_vecInvenObj.resize(32);
    m_vecInvenObj[0]    = pScene->Get_UI(L"UI_Card_Deck_Inven0");
    m_vecInvenObj[1]    = pScene->Get_UI(L"UI_Card_Deck_Inven1");
    m_vecInvenObj[2]    = pScene->Get_UI(L"UI_Card_Deck_Inven2");
    m_vecInvenObj[3]    = pScene->Get_UI(L"UI_Card_Deck_Inven3");
    m_vecInvenObj[4]    = pScene->Get_UI(L"UI_Card_Deck_Inven4");
    m_vecInvenObj[5]    = pScene->Get_UI(L"UI_Card_Deck_Inven5");
    m_vecInvenObj[6]    = pScene->Get_UI(L"UI_Card_Deck_Inven6");
    m_vecInvenObj[7]    = pScene->Get_UI(L"UI_Card_Deck_Inven7");
    m_vecInvenObj[8]    = pScene->Get_UI(L"UI_Card_Deck_Inven8");
    m_vecInvenObj[9]    = pScene->Get_UI(L"UI_Card_Deck_Inven9");
    m_vecInvenObj[10]   = pScene->Get_UI(L"UI_Card_Deck_Inven10");
    m_vecInvenObj[11]   = pScene->Get_UI(L"UI_Card_Deck_Inven11");
    m_vecInvenObj[12]   = pScene->Get_UI(L"UI_Card_Deck_Inven12");
    m_vecInvenObj[13]   = pScene->Get_UI(L"UI_Card_Deck_Inven13");
    m_vecInvenObj[14]   = pScene->Get_UI(L"UI_Card_Deck_Inven14");
    m_vecInvenObj[15]   = pScene->Get_UI(L"UI_Card_Deck_Inven15");
    m_vecInvenObj[16]   = pScene->Get_UI(L"UI_Card_Deck_Inven16");
    m_vecInvenObj[17]   = pScene->Get_UI(L"UI_Card_Deck_Inven17");
    m_vecInvenObj[18]   = pScene->Get_UI(L"UI_Card_Deck_Inven18");
    m_vecInvenObj[19]   = pScene->Get_UI(L"UI_Card_Deck_Inven19");
    m_vecInvenObj[20]   = pScene->Get_UI(L"UI_Card_Deck_Inven20");
    m_vecInvenObj[21]   = pScene->Get_UI(L"UI_Card_Deck_Inven21");
    m_vecInvenObj[22]   = pScene->Get_UI(L"UI_Card_Deck_Inven22");
    m_vecInvenObj[23]   = pScene->Get_UI(L"UI_Card_Deck_Inven23");
    m_vecInvenObj[24]   = pScene->Get_UI(L"UI_Card_Deck_Inven24");
    m_vecInvenObj[25]   = pScene->Get_UI(L"UI_Card_Deck_Inven25");
    m_vecInvenObj[26]   = pScene->Get_UI(L"UI_Card_Deck_Inven26");
    m_vecInvenObj[27]   = pScene->Get_UI(L"UI_Card_Deck_Inven27");
    m_vecInvenObj[28]   = pScene->Get_UI(L"UI_Card_Deck_Inven28");
    m_vecInvenObj[29]   = pScene->Get_UI(L"UI_Card_Deck_Inven29");
    m_vecInvenObj[30]   = pScene->Get_UI(L"UI_Card_Deck_Inven30");
    m_vecInvenObj[31]   = pScene->Get_UI(L"UI_Card_Deck_Inven1");

    return S_OK;
}

void UiCardDeckController::Tick()
{
	if (m_pOwner.expired())
		return;

    if (KEYTAP(KEY_TYPE::ESC) && true == m_bIsRender)
    {
        Render_Off();
    }
}

void UiCardDeckController::Render_On()
{
    m_bIsRender = true;
    auto pScene = CUR_SCENE;

    _uint iSize = IDX(m_vecCardDeckObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if(false == m_vecCardDeckObj[i].expired())
            m_vecCardDeckObj[i].lock()->Set_Render(true);
    }

    iSize = IDX(m_vecInvenObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecInvenObj[i].expired())
            m_vecInvenObj[i].lock()->Set_Render(true);
    }
}

void UiCardDeckController::Render_Off()
{
    m_bIsRender = false;
    auto pScene = CUR_SCENE;

    _uint iSize = IDX(m_vecCardDeckObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecCardDeckObj[i].expired())
            m_vecCardDeckObj[i].lock()->Set_Render(false);
    }

    iSize = IDX(m_vecInvenObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecInvenObj[i].expired())
            m_vecInvenObj[i].lock()->Set_Render(false);
    }
}
