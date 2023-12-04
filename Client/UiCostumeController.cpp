#include "pch.h"
#include "UiCostumeController.h" 

#include "Model.h"
#include "BaseUI.h"
#include "MeshRenderer.h"
#include "FontRenderer.h"
#include "MainUiController.h"

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
    
    m_TempSet = m_OriginSet = pair(3, 2);

    // first : texture key, second : text
    {
        m_vecHair[0].first = L"Am_ct_hat_009";
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

    m_pController = CUR_SCENE->Get_UI(L"Main_UI_Controller");

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

    m_TempSet = m_OriginSet;
    m_eType = COSTUME_TYPE::HAIR;

    m_bIsCreated = true;
    auto pScene = CUR_SCENE;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Costume_Test.dat", m_vecAddedObj);

    weak_ptr<GameObject> pE;
    _float4 vecPos = {};

    _uint iSize = IDX(m_vecAddedObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& pObj = m_vecAddedObj[i];
        if (true == pObj.expired())
            continue;

        wstring strName = pObj.lock()->Get_Name();
        if (L"UI_Costume_Exit_Button" == strName)
        {
            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Remove_Costume();
                });
        }
        else if (L"UI_Costume_Hair_Button" == strName)
        {
            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Change_Costume_Type(COSTUME_TYPE::HAIR);
                });
        }
        else if (L"UI_Costume_Uniform_Button" == strName)
        {
            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Change_Costume_Type(COSTUME_TYPE::UNIFORM);
                });
        }
        else if (L"UI_Costume_Select" == strName)
        {
            pObj.lock()->Set_Render(false);
        }
        else if (L"UI_Costume_Inven_0" == strName)
        {
            if (0 == m_OriginSet.first)
                vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Select_Inven(0);
                });
        }
        else if (L"UI_Costume_Inven_1" == strName)
        {
            if (1 == m_OriginSet.first)
                vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Select_Inven(1);
                });
        }
        else if (L"UI_Costume_Inven_2" == strName)
        {
            if (2 == m_OriginSet.first)
                vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Select_Inven(2);
                });
        }
        else if (L"UI_Costume_Inven_3" == strName)
        {
            if (3 == m_OriginSet.first)
                vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Select_Inven(3);
                });
        }
        else if (L"UI_Costume_Inven_4" == strName)
        {
            if (4 == m_OriginSet.first)
                vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Select_Inven(4);
                });
        }
        else if (L"UI_Costume_Inven_5" == strName)
        {
            if (5 == m_OriginSet.first)
                vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Select_Inven(5);
                });
        }
        else if (L"UI_Costume_Set_Button" == strName)
        {
            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Change_Costume();
                });
        }
        else if (L"UI_Costume_E" == strName)
        {
            pE = pObj;
        }
    }

    if (false == m_pController.expired())
        m_pController.lock()->Get_Script<MainUiController>()->Set_MainUI_Render(false);
    
    _float4 vecTemp = pE.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecTemp.x = vecPos.x + -95.f;
    vecTemp.y = vecPos.y + 50.f;
    pE.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecTemp);
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

    if (false == m_pController.expired())
        m_pController.lock()->Get_Script<MainUiController>()->Set_MainUI_Render(true);

    auto model = RESOURCES.Get<Model>(L"Player");
    if (!model)
        return;

    model->AddParts(m_vecHair[m_OriginSet.first].first, PARTS_INFO::Hair);
    model->AddParts(m_vecUniform[m_OriginSet.second].first, PARTS_INFO::Uniform);
}

void UiCostumeController::Change_Costume_Type(COSTUME_TYPE eType)
{
    if (m_eType == eType)
        return;

    m_eType = eType;

    auto& pResource = RESOURCES;

    _uint iSize = IDX(m_vecAddedObj.size());

    _float4 vecTemp = {};

    weak_ptr<GameObject> pE;
    switch (m_eType)
    {
    case COSTUME_TYPE::HAIR:
        for (_uint i = 0; i < iSize; ++i)
        {
            auto& pObj = m_vecAddedObj[i];
            if (true == pObj.expired())
                continue;

            wstring strName = pObj.lock()->Get_Name();
            if (L"UI_Costume_Hair_Button" == strName)
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(L"UI_Costume_Hat1"), TextureMapType::DIFFUSE);
            else if (L"UI_Costume_Uniform_Button" == strName)
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(L"UI_Costume_Uniform0"), TextureMapType::DIFFUSE);

            else if (L"UI_Costume_Inven_0" == strName)
            {
                if (0 == m_OriginSet.first)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecHair[0].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_1" == strName)
            {
                if (1 == m_OriginSet.first)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecHair[1].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_2" == strName)
            {
                if (2 == m_OriginSet.first)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecHair[2].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_3" == strName)
            {
                if (3 == m_OriginSet.first)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecHair[3].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_4" == strName)
            {
                if (4 == m_OriginSet.first)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecHair[4].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_5" == strName)
            {
                if (5 == m_OriginSet.first)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecHair[5].first), TextureMapType::DIFFUSE);
            }

            else if (L"UI_Costume_Inven_0_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 440.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecHair[0].second;
            }
            else if (L"UI_Costume_Inven_1_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 695.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecHair[1].second;
            }
            else if (L"UI_Costume_Inven_2_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 430.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecHair[2].second;
            }
            else if (L"UI_Costume_Inven_3_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 680.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecHair[3].second;
            }
            else if (L"UI_Costume_Inven_4_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 430.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecHair[4].second;
            }
            else if (L"UI_Costume_Inven_5_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 700.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecHair[5].second;
            }
            else if (L"UI_Costume_E" == strName)
            {
                
                pE = pObj;
                
            }
            else if (L"UI_Costume_Select" == strName)
            {
                pObj.lock()->Set_Render(false);
            }
        }
        break;

    case COSTUME_TYPE::UNIFORM:
        for (_uint i = 0; i < iSize; ++i)
        {
            auto& pObj = m_vecAddedObj[i];
            if (true == pObj.expired())
                continue;

            wstring strName = pObj.lock()->Get_Name();
            if (L"UI_Costume_Hair_Button" == strName)
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(L"UI_Costume_Hat0"), TextureMapType::DIFFUSE);
            else if (L"UI_Costume_Uniform_Button" == strName)
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(L"UI_Costume_Uniform1"), TextureMapType::DIFFUSE);

            else if (L"UI_Costume_Inven_0" == strName)
            {
                if (0 == m_OriginSet.second)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecUniform[0].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_1" == strName)
            {
                if (1 == m_OriginSet.second)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecUniform[1].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_2" == strName)
            {
                if (2 == m_OriginSet.second)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecUniform[2].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_3" == strName)
            {
                if (3 == m_OriginSet.second)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecUniform[3].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_4" == strName)
            {
                if (4 == m_OriginSet.second)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecUniform[4].first), TextureMapType::DIFFUSE);
            }
            else if (L"UI_Costume_Inven_5" == strName)
            {
                if (5 == m_OriginSet.second)
                    vecTemp = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(m_vecUniform[5].first), TextureMapType::DIFFUSE);
            }

            else if (L"UI_Costume_Inven_0_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 455.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecUniform[0].second;
            }
            else if (L"UI_Costume_Inven_1_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 675.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecUniform[1].second;
            }
            else if (L"UI_Costume_Inven_2_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 440.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecUniform[2].second;
            }
            else if (L"UI_Costume_Inven_3_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 675.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecUniform[3].second;
            }
            else if (L"UI_Costume_Inven_4_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 465.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecUniform[4].second;
            }
            else if (L"UI_Costume_Inven_5_Font" == strName)
            {
                _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                vecPos.x = 690.f;
                pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                pObj.lock()->Get_FontRenderer()->Get_Text() = m_vecUniform[5].second;
            }
            else if (L"UI_Costume_E" == strName)
            {
                pE = pObj;
            }
            else if (L"UI_Costume_Select" == strName)
            {
                pObj.lock()->Set_Render(false);
            }
        }
        break;
    }

    _float4 vecPos = pE.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.x = vecTemp.x + -95.f;
    vecPos.y = vecTemp.y + 50.f;
    pE.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
}

void UiCostumeController::Change_Costume()
{
    if (m_OriginSet == m_TempSet)
        return;

    m_OriginSet = m_TempSet;
    auto model = RESOURCES.Get<Model>(L"Player");
    if (!model)
        return;

    model->AddParts(m_vecHair[m_OriginSet.first].first, PARTS_INFO::Hair);
    model->AddParts(m_vecUniform[m_OriginSet.second].first, PARTS_INFO::Uniform);

    wstring strTemp = L"UI_Costume_Inven_";
    if (COSTUME_TYPE::HAIR == m_eType)
        strTemp += to_wstring(m_OriginSet.first);
    else if(COSTUME_TYPE::UNIFORM == m_eType)
        strTemp += to_wstring(m_OriginSet.second);

    weak_ptr<GameObject> pE;
    weak_ptr<GameObject> pInven;
    for (_uint i = 0; i < IDX(m_vecAddedObj.size()); ++i)
    {
        if (true == m_vecAddedObj[i].expired())
            continue;

        if (strTemp == m_vecAddedObj[i].lock()->Get_Name())
            pInven = m_vecAddedObj[i];
        else if (L"UI_Costume_E" == m_vecAddedObj[i].lock()->Get_Name())
            pE = m_vecAddedObj[i];
    }

    _float4 vecPos = pInven.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.x += -95.f;
    vecPos.y += 50.f;
    vecPos.z = 4.5f;
    pE.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
}

void UiCostumeController::Select_Inven(_uint iIndex)
{
    weak_ptr<GameObject> pSelect;
    weak_ptr<GameObject> pInven;
    wstring strTemp = L"UI_Costume_Inven_" + to_wstring(iIndex);
    for (_uint i = 0; i < IDX(m_vecAddedObj.size()); ++i)
    {
        if (true == m_vecAddedObj[i].expired())
            continue;

        if (L"UI_Costume_Select" == m_vecAddedObj[i].lock()->Get_Name())
        {
            m_vecAddedObj[i].lock()->Set_Render(true);
            pSelect = m_vecAddedObj[i];
        }

        else if (strTemp == m_vecAddedObj[i].lock()->Get_Name())
        {
            m_vecAddedObj[i].lock()->Set_Render(true);
            pInven = m_vecAddedObj[i];
        }
    }

    _float4 vecPos1 = pInven.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    _float4 vecPos2 = pSelect.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos2.x = vecPos1.x;
    vecPos2.y = vecPos1.y;
    pSelect.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos2);

    if (COSTUME_TYPE::HAIR == m_eType)
    {
        if (m_TempSet.first == iIndex)
            return;

        m_TempSet.first = iIndex;
    }
    else if (COSTUME_TYPE::UNIFORM == m_eType)
    {
        if (m_TempSet.second == iIndex)
            return;

        m_TempSet.second = iIndex;
    }

    auto model = RESOURCES.Get<Model>(L"Player");
    if (!model)
        return;

    model->AddParts(m_vecHair[m_TempSet.first].first, PARTS_INFO::Hair);
    model->AddParts(m_vecUniform[m_TempSet.second].first, PARTS_INFO::Uniform);
}
