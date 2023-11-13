#include "pch.h"
#include "UiCardDeckController.h" 

UiCardDeckController::UiCardDeckController()
{
}

HRESULT UiCardDeckController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_vecObjTag.resize(15);
    m_vecObjTag[0] = L"UI_Card_Deck_Total_Bg0";
    m_vecObjTag[1] = L"UI_Card_Deck_Total_Bg1";
    m_vecObjTag[2] = L"UI_Card_Deck_Inven_Bg";
    m_vecObjTag[3] = L"UI_Card_Deck0";
    m_vecObjTag[4] = L"UI_Card_Deck1";
    m_vecObjTag[5] = L"UI_Card_Deck2";
    m_vecObjTag[6] = L"UI_Card_Deck_Line0";
    m_vecObjTag[7] = L"UI_Card_Deck_Line1";
    m_vecObjTag[8] = L"UI_Card_Deck_Line2";
    m_vecObjTag[9] = L"UI_Card_Deck_X0";
    m_vecObjTag[10] = L"UI_Card_Deck_X1";
    m_vecObjTag[11] = L"UI_Card_Deck_X2";
    m_vecObjTag[12] = L"UI_Card_Deck_Element0";
    m_vecObjTag[13] = L"UI_Card_Deck_Element1";
    m_vecObjTag[14] = L"UI_Card_Deck_Element2";


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

    _uint iSize = IDX(m_vecObjTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        pScene->Get_GameObject(m_vecObjTag[i])->Set_Render(true);
    }
}

void UiCardDeckController::Render_Off()
{
    m_bIsRender = false;
    auto pScene = CUR_SCENE;

    _uint iSize = IDX(m_vecObjTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        pScene->Get_GameObject(m_vecObjTag[i])->Set_Render(false);
    }
}
