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

    m_fSpeed = 20.f;
    m_fMaxTime = 1.f;

    auto pScene = CUR_SCENE;
    m_pUiTotalController =  pScene->Get_UI(L"Main_UI_Controller");
    m_pUiCurQuest =         pScene->Get_UI(L"UI_Cur_Quest");
    m_pUiCurQuestName =     pScene->Get_UI(L"UI_Cur_Quest_Name");
    m_pUiCurQuestInfo =     pScene->Get_UI(L"UI_Cur_Quest_Info");
    m_pUiCurQuestCount =    pScene->Get_UI(L"UI_Cur_Quest_Count");

    m_bIsCreated = false;
    m_bHaveQuest = false;
    m_iMaxIndex = m_iCurIndex = 0;

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

    if (true == m_bHaveQuest)
        Set_Render(false);

    // 매우 중요
    // 현재 퀘스트 인덱스가 뭔지 얻어올 방법 마련해야함
    // 매우 중요

    if(false == m_bHaveQuest)
        m_eIndex = QUESTINDEX::KILL_DELLONS;

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
        // 첫 대화 정하기
        m_iMaxIndex = DATAMGR.Get_Dialog_Size(m_eIndex, m_bHaveQuest, m_iCurIndex, m_tagCurQuestData.IsClear);
        m_pNpcDialog.lock()->Get_FontRenderer()->Get_Text() = DATAMGR.Get_Dialog(m_eIndex, m_bHaveQuest, m_iCurIndex, m_tagCurQuestData.IsClear);
        m_pNpcDialog.lock()->Get_FontRenderer()->Set_TimePerChar(0.05f);
    }
    if (false == m_pNext.expired())
    {
        // 버튼 실행 함수 지정
        m_pNext.lock()->Get_Button()->AddOnClickedEvent([&]()
            {
                this->Next_Dialog();
            });
    }
    if (false == m_pUiTotalController.expired())
    {
        // 대화 하는 동안 다 끄기
        m_pUiTotalController.lock()->Get_Script<MainUiController>()->Set_MainUI_Render(false);
    }


}

void UiQuestController::Clear_Quest()
{
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
       EVENTMGR.Delete_Object(m_pNext.lock());
       EVENTMGR.Delete_Object(m_pNpcName.lock());
       EVENTMGR.Delete_Object(m_pNpcBg.lock());
       EVENTMGR.Delete_Object(m_pNpcDialog.lock());
       EVENTMGR.Delete_Object(m_pPlayerBg.lock());
       EVENTMGR.Delete_Object(m_pPlayerDialog.lock());
    }
}

void UiQuestController::Next_Dialog()
{
    // 인덱스 최대 비교후 미만이면 대화 가져오고 최대일때는 메인유아이 키고 다 지우기 
    // 마지막일때 m_bHaveQuest = true;
    // 퀘스트 가지게 되면 유아이에 띄워야함
    if (false == m_pNpcDialog.lock()->Get_FontRenderer()->Is_Finished())
    {
        m_pNpcDialog.lock()->Get_FontRenderer()->Set_Finished();
        return;
    }

    ++m_iCurIndex;
    if (m_iMaxIndex <= m_iCurIndex)
    {
        if (true == m_tagCurQuestData.IsClear)
        {
            Set_Render(false);
            m_tagCurQuestData = QUESTDATA{};
            m_bHaveQuest = false;
        }
        else if(false == m_bHaveQuest)
        {
            m_bHaveQuest = true;
            m_tagCurQuestData = GET_QUEST(m_eIndex);
            Set_Cur_Quest();
        }
        else if (true == m_bHaveQuest)
        {
            Set_Render(true);
        }

        m_iCurIndex = 0;
        
        Remove_Dialog();

        if (false == m_pUiTotalController.expired())
        {
            m_pUiTotalController.lock()->Get_Script<MainUiController>()->Set_MainUI_Render(true);
        }
    }

    else
    {
        m_pNpcDialog.lock()->Get_FontRenderer()->Get_Text() = DATAMGR.Get_Dialog(m_eIndex, m_bHaveQuest, m_iCurIndex, m_tagCurQuestData.IsClear);
        m_pNpcDialog.lock()->Get_FontRenderer()->Set_TimePerChar(0.05f);
    }

}

void UiQuestController::Change_Value()
{
    if (true == m_tagCurQuestData.IsClear)
        return;

    // 개수 비교를 통해 클리어로 바꾼다음 info 클리어로 바꾸기
    // 또는 그냥 클리어 문구로 바꾸기
    // 개수 바뀌는건 문구도 바꿔야함
    

    switch (m_tagCurQuestData.Type)
    {
    case QUESTTYPE::COLLECT:
    case QUESTTYPE::HUNT:
        ++m_tagCurQuestData.CurCount;
        if (m_tagCurQuestData.MaxCount <= m_tagCurQuestData.CurCount)
        {
            m_tagCurQuestData.IsClear = true;
            // 성공으로 바꾸기
            m_pUiCurQuestInfo.lock()->Get_FontRenderer()->Get_Text() = m_tagCurQuestData.Clear;
            m_pUiCurQuestCount.lock()->Set_Render(false);
        }
        else
        {
            // 개수 바꾼거 폰트도 바꾸기
            wstring strTemp = to_wstring(m_tagCurQuestData.CurCount) + L" / " + to_wstring(m_tagCurQuestData.MaxCount);
            m_pUiCurQuestCount.lock()->Get_FontRenderer()->Get_Text() = strTemp;
        }
        break;

    case QUESTTYPE::BOSS:
    case QUESTTYPE::ACT:
        m_tagCurQuestData.IsClear = true;
        m_pUiCurQuestInfo.lock()->Get_FontRenderer()->Get_Text() = m_tagCurQuestData.Clear;
        // 성공으로 바꾸기
        break;
    }

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
    
    m_pUiCurQuestName.lock()->Get_FontRenderer()->Get_Text() = m_tagCurQuestData.Name;
    m_pUiCurQuestInfo.lock()->Get_FontRenderer()->Get_Text() = m_tagCurQuestData.Info;

    //마리수 띄워야 할경우 판단하기
    if (QUESTTYPE::COLLECT == m_tagCurQuestData.Type || QUESTTYPE::HUNT == m_tagCurQuestData.Type)
    {
        m_pUiCurQuestCount.lock()->Set_Render(true);
        wstring strTemp = to_wstring(0) + L" / " + to_wstring(m_tagCurQuestData.MaxCount);
        m_pUiCurQuestCount.lock()->Get_FontRenderer()->Get_Text() = strTemp;
    }
}

void UiQuestController::Set_Render(_bool _bValue)
{
    if (true == m_pUiCurQuest.expired() ||
        true == m_pUiCurQuestName.expired() ||
        true == m_pUiCurQuestInfo.expired())
        return;

    m_pUiCurQuest.lock()->Set_Render(_bValue);
    m_pUiCurQuestName.lock()->Set_Render(_bValue);
    m_pUiCurQuestInfo.lock()->Set_Render(_bValue);
    
    if(QUESTTYPE::COLLECT == m_tagCurQuestData.Type || QUESTTYPE::HUNT == m_tagCurQuestData.Type)
        m_pUiCurQuestCount.lock()->Set_Render(_bValue);
}
