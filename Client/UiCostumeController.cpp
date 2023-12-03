#include "pch.h"
#include "UiCostumeController.h" 

#include "BaseUI.h"

UiCostumeController::UiCostumeController()
{
}

HRESULT UiCostumeController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    m_vecHair.resize(6);
    m_vecUniform.resize(6);
    
    // first : texture key, second : text
    {
        m_vecHair[0].first = L"Am_Ct_Hat_009";
        m_vecHair[0].second = L"사라진 여왕의 왕관";

        m_vecHair[1].first = L"Am_Ct_Hat_013";
        m_vecHair[1].second = L"해군 장교 모자";

        m_vecHair[2].first = L"Am_Ct_Hat_020";
        m_vecHair[2].second = L"트로피컬 바캉스 헤어";

        m_vecHair[3].first = L"Am_Ct_Hat_022";
        m_vecHair[3].second = L"보름달 대축제 헤어";

        m_vecHair[4].first = L"Am_Ct_Hat_036";
        m_vecHair[4].second = L"빅토리아 해적단 모자";

        m_vecHair[5].first = L"Am_Ct_Hat_039";
        m_vecHair[5].second = L"월하미인 헤어";
    }


    {
        m_vecUniform[0].first = L"Am_Ct_Uniform_013";
        m_vecUniform[0].second = L"해군 장교의 제복";

        m_vecUniform[1].first = L"Am_Ct_Uniform_020";
        m_vecUniform[1].second = L"트로피컬 바캉스 룩";

        m_vecUniform[2].first = L"Am_Ct_Uniform_022";
        m_vecUniform[2].second = L"보름달 대축제 의상";

        m_vecUniform[3].first = L"Am_Ct_Uniform_037";
        m_vecUniform[3].second = L"쓰리테일 바캉스 룩";

        m_vecUniform[4].first = L"Am_Ct_Uniform_039";
        m_vecUniform[4].second = L"월하미인 예복";

        m_vecUniform[5].first = L"Am_Light_Uniform_006";
        m_vecUniform[5].second = L"프라임 컬렉션 룩";
    }



    return S_OK;
}

void UiCostumeController::Tick()
{
	if (m_pOwner.expired())
		return;


}

void UiCostumeController::Create_Costume()
{
    if (true == m_bIsCreated)
        return;

    m_bIsCreated = true;
    auto pScene = CUR_SCENE;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Costume.dat", m_vecAddedObj);

    _uint iSize = IDX(m_vecAddedObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        if (true == m_vecAddedObj[i].expired())
            continue;

        wstring strName = m_vecAddedObj[i].lock()->Get_Name();
        if (L"UI_Costume_Exit_Button" == strName)
            m_vecAddedObj[i].lock()->Get_Button()->AddOnClickedEvent([this]()
            {
                    this->Remove_Costume();
            });
        else if (L"UI_Costume_Hair_Button" == strName)
            m_vecAddedObj[i].lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Change_Costume_Type(COSTUME_TYPE::HAIR);
                });
        else if (L"UI_Costume_Uniform_Button" == strName)
            m_vecAddedObj[i].lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Change_Costume_Type(COSTUME_TYPE::UNIFORM);
                });
        else if (L"UI_Dialog_Npc" == strName)
            return;
    }
}

void UiCostumeController::Remove_Costume()
{
    if (false == m_bIsCreated)
        return;

    m_bIsCreated = false;

    auto& pEventMgr = EVENTMGR;

    for (_uint i = 0; i < IDX(m_vecAddedObj.size()); ++i)
    {
        auto& pObj = m_vecAddedObj[i];
        if (false == pObj.expired())
        {
            pEventMgr.Delete_Object(pObj.lock());
            pObj.reset();
        }
    }

    m_vecAddedObj.clear();
}

void UiCostumeController::Change_Costume_Type(COSTUME_TYPE eType)
{
    switch (eType)
    {
    case COSTUME_TYPE::HAIR:

        break;
    case COSTUME_TYPE::UNIFORM:

        break;
    }
}
