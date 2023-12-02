#include "pch.h"
#include "UIInteraction.h"

#include "BaseUI.h"
#include "FontRenderer.h"
#include "UiGachaController.h"
#include "UiQuestController.h"

UIInteraction::UIInteraction()
{
}

HRESULT UIInteraction::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;
    
    m_bIsInit = true;

    m_bIsCreated = false;

    auto pScene = CUR_SCENE;
    m_pDialog_Controller = pScene->Get_UI(L"UI_Dialog_Controller");
    m_pGachaController = pScene->Get_UI(L"UI_Gacha_Controller");

    return S_OK;
}

void UIInteraction::Create_Interaction(NPCTYPE eType)
{
    if (NPCTYPE::MAX == eType)
        return;

    if (true == m_bIsCreated)
        return;

    m_bIsCreated = true;
    m_bIsActivate = false;

    auto pScene = CUR_SCENE;
    vector<weak_ptr<GameObject>> addedObj;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Interaction.dat", addedObj);

    _uint iSize = IDX(addedObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (true == addedObj[i].expired())
            continue;

        wstring strName = addedObj[i].lock()->Get_Name();
        if (L"UI_Interaction" == strName)
            m_pInteraction_Bg = addedObj[i];
        else if (L"UI_Interaction_Font" == strName)
        {
            m_pInteraction_Font = addedObj[i];
            if(false == m_pInteraction_Font.expired())
                m_pInteraction_Font.lock()->Get_FontRenderer()->Get_Text() = GET_NPC_NAME(eType);
        }
    }

    if (false == m_pInteraction_Bg.expired())
    {
        weak_ptr<GameObject> pObj;
        QUESTINDEX eIndex = QUESTINDEX::MAX;
        switch (eType)
        {
        case NPCTYPE::QUEST:
            eIndex = QUESTINDEX::KILL_DELLONS;
            break;
        case NPCTYPE::GACHA:
            eIndex = QUESTINDEX::TRY_GACHA;
            break;
        }

        pObj = m_pDialog_Controller;
        m_pInteraction_Bg.lock()->Get_Button()->AddOnClickedEvent([pObj, eType, this, eIndex]()
            {
                if (false == pObj.expired())
                {
                    pObj.lock()->Get_Script<UiQuestController>()->Create_Dialog(eType, eIndex);
                    this->Remove_Interaction();

                }
            });

        //pObj = m_pGachaController;
        //m_pInteraction_Bg.lock()->Get_Button()->AddOnClickedEvent([pObj, this]()
        //    {
        //        if (false == pObj.expired())
        //        {
        //            pObj.lock()->Get_Script<UiGachaController>()->Create_Gacha_Card();
        //            this->Remove_Interaction();
        //        }
        //    });
    }

}

void UIInteraction::Remove_Interaction()
{
    if (false == m_pInteraction_Bg.expired() ||
        false == m_pInteraction_Font.expired())
    {
        m_bIsCreated = false;
        m_bIsActivate = true;

        EVENTMGR.Delete_Object(m_pInteraction_Bg.lock());
        EVENTMGR.Delete_Object(m_pInteraction_Font.lock());
    }
}
