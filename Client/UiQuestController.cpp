#include "pch.h"
#include "UiQuestController.h" 

#include "BaseUI.h"
#include "MeshRenderer.h"
#include "FontRenderer.h"
#include "MainUiController.h"

UiQuestController::UiQuestController()
{
}

HRESULT UiQuestController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    m_bIsCreated = false;
    m_fSpeed = 20.f;
    m_fMaxTime = 1.f;

    auto pScene = CUR_SCENE;
    m_pUiTotalController =  pScene->Get_UI(L"Main_Ui_Controller");
    m_pUiCurQuest =         pScene->Get_UI(L"UI_Cur_Quest");
    m_pUiCurQuestName =     pScene->Get_UI(L"UI_Cur_Quest_Name");
    m_pUiCurQuestInfo =     pScene->Get_UI(L"UI_Cur_Quest_Info");
    m_pUiCurQuestCount =    pScene->Get_UI(L"UI_Cur_Quest_Count");

    return S_OK;
}

void UiQuestController::Tick()
{
	if (true == m_pOwner.expired()      ||
        true == m_pNext.expired()       ||
        true == m_pNpcName.expired()    ||
        true == m_pNpcBg.expired()      ||
        true == m_pNpcDialog.expired()  ||
        true == m_pPlayerBg.expired()   ||
        true == m_pPlayerDialog.expired())
		return;

    Move_Next();
}

void UiQuestController::Create_Dialog(NPCTYPE eType)
{
    if (true == m_bIsCreated)
        return;

    m_bIsCreated = true;
    auto pScene = CUR_SCENE;
    vector<weak_ptr<GameObject>> addedObj;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Dialog.dat", addedObj);

    _uint iSize = IDX(addedObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (true == addedObj[i].expired())
            continue;

        wstring strName = addedObj[i].lock()->Get_Name();
        if (L"UI_Dialog_Next" == strName)
            m_pNext = addedObj[i];
        else if (L"UI_Dialog_Npc_Name" == strName)
            m_pNpcName = addedObj[i];
        else if (L"UI_Dialog_Npc_Bg" == strName)
            m_pNpcBg = addedObj[i];
        else if (L"UI_Dialog_Npc" == strName)
            m_pNpcDialog = addedObj[i];
        else if (L"UI_Dialog_Player_Bg" == strName)
            m_pPlayerBg = addedObj[i];
        else if (L"UI_Dialog_Player" == strName)
            m_pPlayerDialog = addedObj[i];
    }


    if (false == m_pNpcName.expired())
    {
        // npc type 정하기
        wstring strNpcName = GET_NPC_NAME(eType);
        m_pNpcName.lock()->Get_FontRenderer()->Get_Text() = strNpcName;
        _float4 vecPos = m_pNpcName.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
        vecPos.x = (strNpcName.length() / 2.f) * -28.f;
        m_pNpcName.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
    }
    
    if (false == m_pNpcDialog.expired())
    {
        m_pNpcDialog.lock()->Get_FontRenderer()->Get_Text() = L"";
        m_pNpcDialog.lock()->Get_FontRenderer()->Set_TimePerChar(0.05f);
    }

    if (false == m_pNext.expired())
    {
        m_pNext.lock()->Get_Button()->AddOnClickedEvent([&]()
            {
                this->Next_Dialog();
            });
    }

    if (false == m_pUiTotalController.expired())
        m_pUiTotalController.lock()->Get_Script<MainUiController>()->Set_MainUI_Render(false);

}

void UiQuestController::Remove_Dialog()
{
    if (false == m_pNext.expired()      ||
        false == m_pNpcName.expired()   ||
        false == m_pNpcBg.expired()     ||
        false == m_pNpcDialog.expired() ||
        false == m_pPlayerBg.expired()  ||
        false == m_pPlayerDialog.expired())
    {
        m_bIsCreated = false;
        auto pScene = CUR_SCENE;
        pScene->Remove_GameObject(m_pNext.lock());
        pScene->Remove_GameObject(m_pNpcName.lock());
        pScene->Remove_GameObject(m_pNpcBg.lock());
        pScene->Remove_GameObject(m_pNpcDialog.lock());
        pScene->Remove_GameObject(m_pPlayerBg.lock());
        pScene->Remove_GameObject(m_pPlayerDialog.lock());
    }
}

void UiQuestController::Next_Dialog()
{
    
}

void UiQuestController::Move_Next()
{
    switch (m_eState)
    {
    case NEXT_STATE::DOWN:
        Next_Down();
        break;
    case NEXT_STATE::UP:
        Next_Up();
        break;
    }
}

void UiQuestController::Next_Down()
{
    m_fCheckTime += fDT;
    if (m_fMaxTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;
        m_eState = NEXT_STATE::UP;

        return;
    }

    _float PosY = -460 - m_fSpeed * m_fCheckTime / m_fMaxTime;
    _float4 vecPos = m_pNext.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.y = PosY;
    m_pNext.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
}

void UiQuestController::Next_Up()
{
    m_fCheckTime += fDT;
    if (m_fMaxTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;
        m_eState = NEXT_STATE::DOWN;

        return;
    }

    _float PosY = -480 + m_fSpeed * m_fCheckTime / m_fMaxTime;
    _float4 vecPos = m_pNext.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.y = PosY;
    m_pNext.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
}

void UiQuestController::Set_Cur_Quest()
{
    if (true == m_pUiCurQuest.expired() ||
        true == m_pUiCurQuestName.expired() ||
        true == m_pUiCurQuestInfo.expired())
        return;

    m_pUiCurQuest.lock()->Set_Render(true);
    m_pUiCurQuestName.lock()->Set_Render(true);
    m_pUiCurQuestInfo.lock()->Set_Render(true);
    
    m_pUiCurQuestName.lock()->Get_FontRenderer()->Get_Text() = L"";
    m_pUiCurQuestInfo.lock()->Get_FontRenderer()->Get_Text() = L"";

    //마리수 띄워야 할경우 판단하기
    m_pUiCurQuestCount.lock()->Set_Render(true);
    m_pUiCurQuestCount.lock()->Get_FontRenderer()->Get_Text() = L"";
}

void UiQuestController::Set_Render_Off()
{
    if (true == m_pUiCurQuest.expired() ||
        true == m_pUiCurQuestName.expired() ||
        true == m_pUiCurQuestInfo.expired())
        return;

    m_pUiCurQuest.lock()->Set_Render(false);
    m_pUiCurQuestName.lock()->Set_Render(false);
    m_pUiCurQuestInfo.lock()->Set_Render(false);
    m_pUiCurQuestCount.lock()->Set_Render(false);
}
