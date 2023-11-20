#include "pch.h"
#include "UiCardDeckController.h"

#include "Material.h"
#include "MeshRenderer.h"
#include "UiCharChange.h"
#include "UiCardDeckInvenChange.h"

UiCardDeckController::UiCardDeckController()
{
}

HRESULT UiCardDeckController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;
    
    auto pScene = CUR_SCENE;

    m_pUiCharChange             = pScene->Get_UI(L"UI_Char_Change");
    m_pUiCardDeckSelect         = pScene->Get_UI(L"UI_Card_Deck_Select");

    m_vecCardDeckObj.resize(18);
    m_vecCardDeckObj[0]         = pScene->Get_UI(L"UI_Card_Deck_Total_Bg0");
    m_vecCardDeckObj[1]         = pScene->Get_UI(L"UI_Card_Deck_Total_Bg1");
    m_vecCardDeckObj[2]         = pScene->Get_UI(L"UI_Card_Deck_Inven_Bg");
    m_vecCardDeckObj[3]         = pScene->Get_UI(L"UI_Card_Deck0");
    m_vecCardDeckObj[4]         = pScene->Get_UI(L"UI_Card_Deck1");
    m_vecCardDeckObj[5]         = pScene->Get_UI(L"UI_Card_Deck2");
    m_vecCardDeckObj[6]         = pScene->Get_UI(L"UI_Card_Deck_Line0");
    m_vecCardDeckObj[7]         = pScene->Get_UI(L"UI_Card_Deck_Line1");
    m_vecCardDeckObj[8]         = pScene->Get_UI(L"UI_Card_Deck_Line2");
    m_vecCardDeckObj[9]         = pScene->Get_UI(L"UI_Card_Deck_X0");
    m_vecCardDeckObj[10]        = pScene->Get_UI(L"UI_Card_Deck_X1");
    m_vecCardDeckObj[11]        = pScene->Get_UI(L"UI_Card_Deck_X2");
    m_vecCardDeckObj[12]        = pScene->Get_UI(L"UI_Card_Deck_Element0");
    m_vecCardDeckObj[13]        = pScene->Get_UI(L"UI_Card_Deck_Element1");
    m_vecCardDeckObj[14]        = pScene->Get_UI(L"UI_Card_Deck_Element2");
    m_vecCardDeckObj[15]        = pScene->Get_UI(L"UI_Card_Deck_Exit");
    m_vecCardDeckObj[16]        = pScene->Get_UI(L"UI_Card_Deck_Font");
    m_vecCardDeckObj[17]        = pScene->Get_UI(L"UI_Card_Deck_Select");
    
    m_vecInvenObj.resize(32);
    m_vecInvenObj[0]            = pScene->Get_UI(L"UI_Card_Deck_Inven0");
    m_vecInvenObj[1]            = pScene->Get_UI(L"UI_Card_Deck_Inven1");
    m_vecInvenObj[2]            = pScene->Get_UI(L"UI_Card_Deck_Inven2");
    m_vecInvenObj[3]            = pScene->Get_UI(L"UI_Card_Deck_Inven3");
    m_vecInvenObj[4]            = pScene->Get_UI(L"UI_Card_Deck_Inven4");
    m_vecInvenObj[5]            = pScene->Get_UI(L"UI_Card_Deck_Inven5");
    m_vecInvenObj[6]            = pScene->Get_UI(L"UI_Card_Deck_Inven6");
    m_vecInvenObj[7]            = pScene->Get_UI(L"UI_Card_Deck_Inven7");
    m_vecInvenObj[8]            = pScene->Get_UI(L"UI_Card_Deck_Inven8");
    m_vecInvenObj[9]            = pScene->Get_UI(L"UI_Card_Deck_Inven9");
    m_vecInvenObj[10]           = pScene->Get_UI(L"UI_Card_Deck_Inven10");
    m_vecInvenObj[11]           = pScene->Get_UI(L"UI_Card_Deck_Inven11");
    m_vecInvenObj[12]           = pScene->Get_UI(L"UI_Card_Deck_Inven12");
    m_vecInvenObj[13]           = pScene->Get_UI(L"UI_Card_Deck_Inven13");
    m_vecInvenObj[14]           = pScene->Get_UI(L"UI_Card_Deck_Inven14");
    m_vecInvenObj[15]           = pScene->Get_UI(L"UI_Card_Deck_Inven15");
    m_vecInvenObj[16]           = pScene->Get_UI(L"UI_Card_Deck_Inven16");
    m_vecInvenObj[17]           = pScene->Get_UI(L"UI_Card_Deck_Inven17");
    m_vecInvenObj[18]           = pScene->Get_UI(L"UI_Card_Deck_Inven18");
    m_vecInvenObj[19]           = pScene->Get_UI(L"UI_Card_Deck_Inven19");
    m_vecInvenObj[20]           = pScene->Get_UI(L"UI_Card_Deck_Inven20");
    m_vecInvenObj[21]           = pScene->Get_UI(L"UI_Card_Deck_Inven21");
    m_vecInvenObj[22]           = pScene->Get_UI(L"UI_Card_Deck_Inven22");
    m_vecInvenObj[23]           = pScene->Get_UI(L"UI_Card_Deck_Inven23");
    m_vecInvenObj[24]           = pScene->Get_UI(L"UI_Card_Deck_Inven24");
    m_vecInvenObj[25]           = pScene->Get_UI(L"UI_Card_Deck_Inven25");
    m_vecInvenObj[26]           = pScene->Get_UI(L"UI_Card_Deck_Inven26");
    m_vecInvenObj[27]           = pScene->Get_UI(L"UI_Card_Deck_Inven27");
    m_vecInvenObj[28]           = pScene->Get_UI(L"UI_Card_Deck_Inven28");
    m_vecInvenObj[29]           = pScene->Get_UI(L"UI_Card_Deck_Inven29");
    m_vecInvenObj[30]           = pScene->Get_UI(L"UI_Card_Deck_Inven30");
    m_vecInvenObj[31]           = pScene->Get_UI(L"UI_Card_Deck_Inven31");
    
    m_vecCardDeckBg.resize(32);
    m_vecCardDeckBg[0]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg0");
    m_vecCardDeckBg[1]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg1");
    m_vecCardDeckBg[2]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg2");
    m_vecCardDeckBg[3]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg3");
    m_vecCardDeckBg[4]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg4");
    m_vecCardDeckBg[5]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg5");
    m_vecCardDeckBg[6]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg6");
    m_vecCardDeckBg[7]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg7");
    m_vecCardDeckBg[8]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg8");
    m_vecCardDeckBg[9]          = pScene->Get_UI(L"UI_Card_Deck_InvenBg9");
    m_vecCardDeckBg[10]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg10");
    m_vecCardDeckBg[11]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg11");
    m_vecCardDeckBg[12]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg12");
    m_vecCardDeckBg[13]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg13");
    m_vecCardDeckBg[14]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg14");
    m_vecCardDeckBg[15]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg15");
    m_vecCardDeckBg[16]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg16");
    m_vecCardDeckBg[17]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg17");
    m_vecCardDeckBg[18]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg18");
    m_vecCardDeckBg[19]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg19");
    m_vecCardDeckBg[20]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg20");
    m_vecCardDeckBg[21]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg21");
    m_vecCardDeckBg[22]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg22");
    m_vecCardDeckBg[23]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg23");
    m_vecCardDeckBg[24]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg24");
    m_vecCardDeckBg[25]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg25");
    m_vecCardDeckBg[26]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg26");
    m_vecCardDeckBg[27]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg27");
    m_vecCardDeckBg[28]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg28");
    m_vecCardDeckBg[29]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg29");
    m_vecCardDeckBg[30]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg30");
    m_vecCardDeckBg[31]         = pScene->Get_UI(L"UI_Card_Deck_InvenBg31");
     
    m_vecCardDeckElement.resize(32);
    m_vecCardDeckElement[0]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element0");
    m_vecCardDeckElement[1]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element1");
    m_vecCardDeckElement[2]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element2");
    m_vecCardDeckElement[3]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element3");
    m_vecCardDeckElement[4]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element4");
    m_vecCardDeckElement[5]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element5");
    m_vecCardDeckElement[6]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element6");
    m_vecCardDeckElement[7]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element7");
    m_vecCardDeckElement[8]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element8");
    m_vecCardDeckElement[9]     = pScene->Get_UI(L"UI_Card_Deck_Inven_Element9");
    m_vecCardDeckElement[10]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element10");
    m_vecCardDeckElement[11]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element11");
    m_vecCardDeckElement[12]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element12");
    m_vecCardDeckElement[13]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element13");
    m_vecCardDeckElement[14]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element14");
    m_vecCardDeckElement[15]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element15");
    m_vecCardDeckElement[16]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element16");
    m_vecCardDeckElement[17]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element17");
    m_vecCardDeckElement[18]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element18");
    m_vecCardDeckElement[19]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element19");
    m_vecCardDeckElement[20]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element20");
    m_vecCardDeckElement[21]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element21");
    m_vecCardDeckElement[22]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element22");
    m_vecCardDeckElement[23]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element23");
    m_vecCardDeckElement[24]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element24");
    m_vecCardDeckElement[25]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element25");
    m_vecCardDeckElement[26]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element26");
    m_vecCardDeckElement[27]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element27");
    m_vecCardDeckElement[28]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element28");
    m_vecCardDeckElement[29]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element29");
    m_vecCardDeckElement[30]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element30");
    m_vecCardDeckElement[31]    = pScene->Get_UI(L"UI_Card_Deck_Inven_Element31");

    m_vecFont.resize(6);
    m_vecFont[0]                = pScene->Get_UI(L"UI_Card_Deck0_Hero_Info");
    m_vecFont[1]                = pScene->Get_UI(L"UI_Card_Deck0_Hero");
    m_vecFont[2]                = pScene->Get_UI(L"UI_Card_Deck1_Hero_Info");
    m_vecFont[3]                = pScene->Get_UI(L"UI_Card_Deck1_Hero");
    m_vecFont[4]                = pScene->Get_UI(L"UI_Card_Deck2_Hero_Info");
    m_vecFont[5]                = pScene->Get_UI(L"UI_Card_Deck2_Hero");

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
    
    iSize = IDX(m_vecCardDeckBg.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecCardDeckBg[i].expired())
            m_vecCardDeckBg[i].lock()->Set_Render(true);
    }
    
    iSize = IDX(m_vecCardDeckElement.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecCardDeckElement[i].expired())
            m_vecCardDeckElement[i].lock()->Set_Render(true);
    }
    
    iSize = IDX(m_vecFont.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecFont[i].expired())
            m_vecFont[i].lock()->Set_Render(true);
    }

}

void UiCardDeckController::Set_Hero(HERO eHero)
{
    _uint iSize = IDX(m_vecInvenObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecInvenObj[i].expired() && false == m_vecCardDeckBg[i].expired() && false == m_vecCardDeckElement[i].expired())
        {
            auto pScript0 = m_vecInvenObj[i].lock()->Get_Script<UiCardDeckInvenChange>();
            auto pScript1 = m_vecCardDeckBg[i].lock()->Get_Script<UiCardDeckInvenChange>();
            auto pScript2 = m_vecCardDeckElement[i].lock()->Get_Script<UiCardDeckInvenChange>();
            if (false == pScript0->Is_Set_Hero() && false == pScript1->Is_Set_Hero() && false == pScript2->Is_Set_Hero())
            {
                pScript0->Set_Hero(eHero);
                pScript1->Set_Hero(eHero);
                pScript2->Set_Hero(eHero);
                return;
            }
        }
    }
}

void UiCardDeckController::Click_Deck_Select(wstring strObjName)
{
    if (false == m_bIsRender)
        return;

    for (_uint i = 3; i < 6; ++i)
    {
        if (false == m_vecCardDeckObj[i].expired())
        {
            if (m_vecCardDeckObj[i].lock()->Get_Name() == strObjName)
            {
                // ¼¼ÆÃ
                m_bIsPicking = true;
                m_iPickingIndex = i;
                m_pUiCardDeckSelect.lock()->Set_Render(true);
                _float fPosX = {};
                switch (i)
                {
                case 3:
                    fPosX = -480;
                    break;
                case 4:
                    fPosX = -140;
                    break;
                case 5:
                    fPosX = 200;
                    break;
                }
                _float4 vecPos = m_pUiCardDeckSelect.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = fPosX;

            }
        }
    }
}

void UiCardDeckController::Click_Deck_Inven(wstring strInvenName)
{
    if (false == m_bIsRender)
        return;

    if (true == m_bIsPicking != 0)
    {
        _uint iSize = IDX(m_vecInvenObj.size());
        for (_uint i = 0; i < iSize; ++i)
        {
            if (m_vecInvenObj[i].lock()->Get_Name() == strInvenName)
            {
                auto pScript = m_vecInvenObj[i].lock()->Get_Script<UiCardDeckInvenChange>();
                if (true == pScript->Is_Set_Hero())
                {
                    HERO eHero = pScript->Get_Hero();
                    m_vecCardDeckObj[m_iPickingIndex].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(GET_DATA(eHero).KeyDeckSelect), TextureMapType::DIFFUSE);
                    m_pUiCharChange.lock()->Get_Script<UiCharChange>()->Set_Hero(m_iPickingIndex - 3, eHero);
                
                    m_vecCardDeckObj[m_iPickingIndex + 6].lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f;
                    
                    m_pUiCardDeckSelect.lock()->Set_Render(false);
                    m_eState = SELECT_STATE::DOWN;
                    _float4 vPos = m_pUiCardDeckSelect.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                    vPos.y = 480;
                    m_pUiCardDeckSelect.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vPos);
                }
            }
        }

    }
    else
    {
        m_bIsPicking = 0;


    }
}

void UiCardDeckController::Click_Deck_X(wstring strObjName)
{
    if (false == m_bIsRender)
        return;

    for (_uint i = 9; i < 12; ++i)
    {
        if (m_vecCardDeckObj[i].lock()->Get_Name() == strObjName)
        {
            m_vecCardDeckObj[i].lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 0.f;
            m_vecCardDeckObj[i - 6].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(L"Card_Deck_Bg_None"), TextureMapType::DIFFUSE);
            m_pUiCharChange.lock()->Get_Script<UiCharChange>()->Set_Hero(i - 9, HERO::MAX);
        }
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
    
    iSize = IDX(m_vecCardDeckBg.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecCardDeckBg[i].expired())
            m_vecCardDeckBg[i].lock()->Set_Render(false);
    }
    
    iSize = IDX(m_vecCardDeckElement.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecCardDeckElement[i].expired())
            m_vecCardDeckElement[i].lock()->Set_Render(false);
    }
    
    iSize = IDX(m_vecFont.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (false == m_vecFont[i].expired())
            m_vecFont[i].lock()->Set_Render(false);
    }

}
