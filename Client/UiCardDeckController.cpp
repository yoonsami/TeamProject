#include "pch.h"
#include "UiCardDeckController.h"

#include "BaseUI.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "FontRenderer.h"
#include "UiCharChange.h"
#include "UiCardDeckSwitch.h"
#include "HeroChangeScript.h"

_bool g_bIsCanRotation = true;

UiCardDeckController::UiCardDeckController()
{
}

HRESULT UiCardDeckController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    m_fMaxTime = 1.f;
    m_fSpeed = 30.f;
    m_fCheckTime = 0.f;
    m_bIsMoveDown = true;


    auto pScene = CUR_SCENE;

    m_pCharChange = pScene->Get_UI(L"UI_Char_Change");

    //for (_uint i = 0; i < IDX(m_vecInvenObj.size()); ++i)
    //{
    //    auto& pObj = m_vecInvenObj[i];
    //    if (true == pObj.expired())
    //        continue;
    //
    //    pObj.lock()->Get_Button()->AddOnClickedEvent([pObj, this]()
    //        {
    //            this->Click_Deck_Inven(pObj.lock()->Get_Name());
    //        });
    //}
    //
    //for (_uint i = 3; i < 6; ++i)
    //{
    //    auto& pObj = m_vecCardDeckObj[i];
    //    if (true == pObj.expired())
    //        continue;
    //
    //    pObj.lock()->Get_Button()->AddOnClickedEvent([pObj, this]()
    //        {
    //            this->Click_Deck_Select(pObj.lock()->Get_Name());
    //        });
    //}
    //
    //for (_uint i = 6; i < 9; ++i)
    //{
    //    auto& pObj = m_vecCardDeckObj[i];
    //    if (true == pObj.expired())
    //        continue;
    //
    //    pObj.lock()->Get_Button()->AddOnClickedEvent([pObj, this]()
    //        {
    //            this->Click_Deck_X(pObj.lock()->Get_Name());
    //        });
    //}

    return S_OK;
}

void UiCardDeckController::Tick()
{
    if (true == m_pOwner.expired() ||
        false == m_bIsCreated)
        return;

    Move_Select_Mark();
    Check_Scroll();
}

void UiCardDeckController::Click_Deck_Select(wstring strObjName, _uint iIndex)
{
    if (false == m_bIsCreated || false == m_bIsClickSet)
        return;

    m_bIsClickSet = false;
    for (_uint i = 0; i < IDX(m_vecCardDeckObj.size()); ++i)
    {
        auto& pObj = m_vecCardDeckObj[i];
        if (false == pObj.expired())
        {
            if (pObj.lock()->Get_Name() == strObjName)
            {
                if (false == DATAMGR.Set_Cur_Hero(iIndex, m_iSetIndex))
                {
                    Remove_Select_Mark();
                    return;
                }
                
                wstring strKey = DATAMGR.Get_Card_Inven(m_iSetIndex).KeyDeckSelect;
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(strKey), TextureMapType::DIFFUSE);

                // 장착 성공 창 띄우기
                Create_Switch_Complete(true);

                if (false == m_pCharChange.expired())
                    m_pCharChange.lock()->Get_Script<UiCharChange>()->Set_Hero(iIndex);

                wstring strName = L"UI_Card_Deck_X" + to_wstring(iIndex);
                for (_uint i = 0; i < IDX(m_vecCardDeckObj.size()); ++i)
                {
                    auto& pObj = m_vecCardDeckObj[i];
                    if (true == pObj.expired())
                        continue;

                    if (strName == pObj.lock()->Get_Name())
                    {
                        pObj.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f;
                    }
                }

                Set_Font(iIndex);

                Remove_Select_Mark();
            }
        }
    }
}

void UiCardDeckController::Click_Deck_Inven(_uint iIndex)
{
    if (false == m_bIsCreated)
        return;

    // 클릭 한 인벤토리 인덱스의 해당하는 데이터 가져와서 정보창 켜기
    Create_Info(iIndex);
}

void UiCardDeckController::Click_Deck_X(wstring strObjName, _uint iIndex)
{
    if (false == m_bIsCreated)
        return;

    for (_uint i = 0; i < IDX(m_vecCardDeckObj.size()); ++i)
    {
        auto& pObj = m_vecCardDeckObj[i];
        if (pObj.lock()->Get_Name() == strObjName)
        {
            if (true == pObj.expired())
                continue;

            pObj.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 0.f;

            if (true == m_vecCardDeckObj[iIndex].expired())
                continue;
            m_vecCardDeckObj[iIndex].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(L"Card_Deck_Bg_None"), TextureMapType::DIFFUSE);

            // 장착 해제 창 띄우기
            Create_Switch_Complete(false);

            // 데이터에서 빼야함
            DATAMGR.Remove_Cur_Hero(iIndex);
            if (false == m_pCharChange.expired())
                m_pCharChange.lock()->Get_Script<UiCharChange>()->Set_Hero(iIndex);

            Remove_Font(iIndex);
        }
    }
}

void UiCardDeckController::Click_Info_Set(_uint iIndex)
{
    m_bIsClickSet = true;
    m_iSetIndex = iIndex;
    Remove_Info();

    Create_Select_Mark();
}

void UiCardDeckController::Remove_Info()
{
    if (false == m_bIsInfoCreate)
        return;

    m_bIsInfoCreate = false;

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

void UiCardDeckController::Create_Card_Deck()
{
    if (true == m_bIsCreated)
        return;

    g_bIsCanRotation = false;

    m_bIsCreated = true;
    auto pScene = CUR_SCENE;

    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Card_Deck_Bg.dat", m_vecCardDeckObj);

    auto& pDataMgr = DATAMGR;

    _bool bIsSet0 = false;
    if (HERO::MAX != pDataMgr.Get_Cur_Set_Hero(0))
        bIsSet0 = true;

    _bool bIsSet1 = false;
    if (HERO::MAX != pDataMgr.Get_Cur_Set_Hero(1))
        bIsSet1 = true;

    _bool bIsSet2 = false;
    if (HERO::MAX != pDataMgr.Get_Cur_Set_Hero(2))
        bIsSet2 = true;

    _uint iSize = IDX(m_vecCardDeckObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& pObj = m_vecCardDeckObj[i];
        if (true == pObj.expired())
            continue;

        wstring strName = pObj.lock()->Get_Name();
        if (L"UI_Card_Deck_Total_Bg0" == strName)
        {
            pObj.lock()->Get_Button()->AddOnClickedEvent([this]()
                {
                    this->Remove_Card_Deck();
                });
        }
        else if (L"UI_Card_Deck_Scroll_Button" == strName)
        {
            m_pScroll = pObj;
            pObj.lock()->Get_Button()->Set_Type(false);
            pObj.lock()->Get_Button()->AddOnClickedEvent([this, pObj]()
                {
                    POINT ptMouse = INPUT.GetMousePosToPoint();
                    _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
                    vecPos.y = static_cast<_float>(ptMouse.y * -1.f + g_iWinSizeY / 2.f);
                    if (-450.f > vecPos.y)
                        vecPos.y = -450.f;
                    if (450.f < vecPos.y)
                        vecPos.y = 450.f;
                    pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
                });
        }
        else if (L"UI_Card_Deck_X0" == strName)
        {
            if(true == bIsSet0)
                pObj.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f;

            pObj.lock()->Get_Button()->AddOnClickedEvent([this, strName]()
                {
                    this->Click_Deck_X(strName, 0);
                });
        }
        else if (L"UI_Card_Deck_X1" == strName)
        {
            if (true == bIsSet1)
                pObj.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f;

            pObj.lock()->Get_Button()->AddOnClickedEvent([this, strName]()
                {
                    this->Click_Deck_X(strName, 1);
                });
        }
        else if (L"UI_Card_Deck_X2" == strName)
        {
            if (true == bIsSet2)
                pObj.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f;

            pObj.lock()->Get_Button()->AddOnClickedEvent([this, strName]()
                {
                    this->Click_Deck_X(strName, 2);
                });
        }
        else if (L"UI_Card_Deck0" == strName)
        {
            if (true == bIsSet0)
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(pDataMgr.Get_Data(pDataMgr.Get_Cur_Set_Hero(0)).KeyDeckSelect), TextureMapType::DIFFUSE);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this, strName]()
                {
                    this->Click_Deck_Select(strName, 0);
                });
        }
        else if (L"UI_Card_Deck1" == strName)
        {
            if (true == bIsSet1)
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(pDataMgr.Get_Data(pDataMgr.Get_Cur_Set_Hero(1)).KeyDeckSelect), TextureMapType::DIFFUSE);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this, strName]()
                {
                    this->Click_Deck_Select(strName, 1);
                });
        }
        else if (L"UI_Card_Deck2" == strName)
        {
            if (true == bIsSet2)
                pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(pDataMgr.Get_Data(pDataMgr.Get_Cur_Set_Hero(2)).KeyDeckSelect), TextureMapType::DIFFUSE);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this, strName]()
                {
                    this->Click_Deck_Select(strName, 2);
                });
        }
    }

    if (true == bIsSet0)
    {
        if(-1 != pDataMgr.Get_Cur_Set_Hero_Index(0))
        {
            m_iSetIndex = pDataMgr.Get_Cur_Set_Hero_Index(0);
            Set_Font(0);
        }
    }

    if (true == bIsSet1)
        if (-1 != pDataMgr.Get_Cur_Set_Hero_Index(1))
        {
            m_iSetIndex = pDataMgr.Get_Cur_Set_Hero_Index(1);
            Set_Font(1);
        }

    if (true == bIsSet2)
        if (-1 != pDataMgr.Get_Cur_Set_Hero_Index(2))
        {
            m_iSetIndex = pDataMgr.Get_Cur_Set_Hero_Index(2);
            Set_Font(2);
        }

    _uint iUseSize = pDataMgr.Get_Card_Inven_Use_Size();
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Card_Deck_Inven.dat", m_vecInvenObj);

    iSize = IDX(m_vecInvenObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& pObj = m_vecInvenObj[i];
        if (true == pObj.expired())
            continue;

        if (i < iUseSize)
        {

            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(pDataMgr.Get_Card_Inven(i).KeyDeckMiniAddBg), TextureMapType::DIFFUSE);

            _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
            vecPos.x = 490.f + static_cast<_float>(i % 4) * 105.f;
            vecPos.y = 430.f - static_cast<_float>(i / 4) * 110.f;
            m_fMaxPosY = vecPos.y;
            pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);

            pObj.lock()->Get_Button()->Get_Desc().ptCenter.x = static_cast<LONG>(vecPos.x + g_iWinSizeX / 2);
            pObj.lock()->Get_Button()->Get_Desc().ptCenter.y = static_cast<LONG>(vecPos.y * -1.f + g_iWinSizeY / 2);

            pObj.lock()->Get_Button()->AddOnClickedEvent([this, i]()
                {
                    this->Click_Deck_Inven(i);
                });
        }
        else
        {
            pObj.lock()->Set_Tick(false);
            pObj.lock()->Set_Render(false);
        }
    }
}

void UiCardDeckController::Remove_Card_Deck()
{
    if (false == m_bIsCreated)
        return;

    g_bIsCanRotation = true;

    m_bIsCreated = false;

    auto& pEventMgr = EVENTMGR;

    for (_uint i = 0; i < IDX(m_vecCardDeckObj.size()); ++i)
    {
        auto& pObj = m_vecCardDeckObj[i];
        if (false == pObj.expired())
        {
            pEventMgr.Delete_Object(pObj.lock());
            pObj.reset();
        }
    }
    m_vecCardDeckObj.clear();

    
    for (_uint i = 0; i < IDX(m_vecInvenObj.size()); ++i)
    {
        auto& pObj = m_vecInvenObj[i];
        if (false == pObj.expired())
        {
            pEventMgr.Delete_Object(pObj.lock());
            pObj.reset();
        }
    }
    m_vecInvenObj.clear();



}

void UiCardDeckController::Set_Card()
{
    auto& pDataMgr = DATAMGR;
    _uint iUseSize = pDataMgr.Get_Card_Inven_Use_Size();
    for (_uint i = 0; i < iUseSize; ++i)
    {
        if (i < iUseSize)
        {
            if (true == m_vecInvenObj[i].expired())
                continue;

            auto& Data = pDataMgr.Get_Card_Inven(i);
            m_vecInvenObj[i].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(Data.KeyDeckMini), TextureMapType::DIFFUSE);
        }
    }
}

void UiCardDeckController::Create_Switch_Complete(_bool bValue)
{
    wstring strPath = L"";
    if (true == bValue)
        strPath = L"..\\Resources\\UIData\\UI_Card_Deck_Switch_In.dat";
    else
        strPath = L"..\\Resources\\UIData\\UI_Card_Deck_Switch_Out.dat";

    auto pScene = CUR_SCENE;
    vector<weak_ptr<GameObject>> addedObj;
    pScene->Load_UIFile(strPath, addedObj);

    _uint iSize = IDX(addedObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& pObj = addedObj[i];
        if (true == pObj.expired())
            continue;

        wstring strName = pObj.lock()->Get_Name();
        _bool bIsFont = false;
        if (L"UI_Card_Deck_Switch_Font" == strName)
            bIsFont = true;

        auto pScript = make_shared<UiCardDeckSwitch>(bIsFont);
        pObj.lock()->Add_Component(pScript);
        pScript->Init();
    }
}

void UiCardDeckController::Create_Info(_uint iIndex)
{
    if (true == m_bIsInfoCreate)
        return;

    auto& pData = DATAMGR;
    auto& tagData = pData.Get_Card_Inven(iIndex);
    if (ElementType::ElementEnd == tagData.Element)
        return;

    m_bIsInfoCreate = true;

    auto pScene = CUR_SCENE;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Card_Deck_Info.dat", m_vecAddedObj);

    auto& pResource = RESOURCES;

    _uint iSize = IDX(m_vecAddedObj.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& pObj = m_vecAddedObj[i];
        if (true == pObj.expired())
            continue;

        wstring strName = pObj.lock()->Get_Name();
        if (L"Card_Deck_info_Element" == strName)
            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(pData.Get_Card_Inven_Element(iIndex)), TextureMapType::DIFFUSE);
        else if (L"Card_Deck_info_Hero" == strName)
            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(tagData.KeyDeckMini), TextureMapType::DIFFUSE);
        else if (L"Card_Deck_info_Hero_Info" == strName)
            pObj.lock()->Get_FontRenderer()->Get_Text() = tagData.KeyHeroInfo;
        else if (L"Card_Deck_info_Hero_Name" == strName)
            pObj.lock()->Get_FontRenderer()->Get_Text() = tagData.KeyHeroName;
        else if (L"Card_Deck_info_Hero_Level_Value" == strName)
            pObj.lock()->Get_FontRenderer()->Get_Text() = to_wstring(tagData.Level);
        else if (L"Card_Deck_info_Hero_Weapon" == strName)
            pObj.lock()->Get_FontRenderer()->Get_Text() = tagData.KeyWeapon;
        else if (L"Card_Deck_info_Hero_Star" == strName)
        {
            _uint iStar = tagData.Star;
            wstring strTextureTag = L"";

            switch (iStar)
            {
            case 1:
                strTextureTag = L"Card_Deck_info_Star1";
                break;
            case 2:
                strTextureTag = L"Card_Deck_info_Star2";
                break;
            case 3:
                strTextureTag = L"Card_Deck_info_Star3";
                break;
            case 4:
                strTextureTag = L"Card_Deck_info_Star4";
                break;
            }

            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(strTextureTag), TextureMapType::DIFFUSE);
        }
        else if (L"Card_Deck_info_Hero_Attack_Value" == strName)
            pObj.lock()->Get_FontRenderer()->Get_Text() = to_wstring(IDX(tagData.AttackDamage));
        else if (L"Card_Deck_info_Hero_Deffence_Value" == strName)
            pObj.lock()->Get_FontRenderer()->Get_Text() = to_wstring(IDX(tagData.Deffence));
        else  if (L"Card_Deck_info_Skill1" == strName)
            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(tagData.KeySkill1), TextureMapType::DIFFUSE);
        else if (L"Card_Deck_info_Skill2" == strName)
            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(tagData.KeySkill2), TextureMapType::DIFFUSE);
        else if (L"Card_Deck_info_Skill3" == strName)
            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(tagData.KeySkill3), TextureMapType::DIFFUSE);
        else if (L"Card_Deck_info_Skill4" == strName)
            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(tagData.KeySkill4), TextureMapType::DIFFUSE);
        else if (L"Card_Deck_info_Skill5" == strName)
            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(pResource.Get<Texture>(tagData.KeySkill5), TextureMapType::DIFFUSE);
        else if (L"Card_Deck_info_Close_Button" == strName)
            pObj.lock()->Get_Button()->AddOnClickedEvent([]()
                {
                    CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Remove_Info();
                });
        else if (L"Card_Deck_info_Set_Button" == strName)
            pObj.lock()->Get_Button()->AddOnClickedEvent([iIndex]()
                {
                    CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Click_Info_Set(iIndex);
                });

    }
}

void UiCardDeckController::Set_Font(_uint iIndex)
{
    Remove_Font(iIndex);

    auto& tagData = DATAMGR.Get_Card_Inven(m_iSetIndex);
    wstring strHero = L"UI_Card_Deck" + to_wstring(iIndex) + L"_Hero";
    wstring strInfo = L"UI_Card_Deck" + to_wstring(iIndex) + L"_Hero_Info";
    for (_uint i = 0; i < IDX(m_vecCardDeckObj.size()); ++i)
    {
        auto& pObj = m_vecCardDeckObj[i];
        if (true == pObj.expired())
            continue;

        if (strHero == pObj.lock()->Get_Name())
        {
            pObj.lock()->Get_FontRenderer()->Get_Text() = tagData.KeyHeroName;
            _float fLength = static_cast<_float>(tagData.KeyHeroName.length());
            _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
            vecPos.x += fLength / 2.f * -27.f;
            pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
        }
        else if (strInfo == pObj.lock()->Get_Name())
        {
            pObj.lock()->Get_FontRenderer()->Get_Text() = tagData.KeyHeroInfo;
            _float fLength = static_cast<_float>(tagData.KeyHeroInfo.length());
            _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
            vecPos.x += fLength / 2.f * -27.f;
            pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
        }
    }
    
}

void UiCardDeckController::Remove_Font(_uint iIndex)
{
    auto& tagData = DATAMGR.Get_Card_Inven(m_iSetIndex);
    wstring strHero = L"UI_Card_Deck" + to_wstring(iIndex) + L"_Hero";
    wstring strInfo = L"UI_Card_Deck" + to_wstring(iIndex) + L"_Hero_Info";

    _float fPosX = 0.f;
    switch (iIndex)
    {
    case 0:
        fPosX = -480;
        break;
    case 1:
        fPosX = -140;
        break;
    case 2:
        fPosX = 200;
        break;
    }

    for (_uint i = 0; i < IDX(m_vecCardDeckObj.size()); ++i)
    {
        auto& pObj = m_vecCardDeckObj[i];
        if (true == pObj.expired())
            continue;

        if (strHero == pObj.lock()->Get_Name())
        {
            _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
            vecPos.x = fPosX;
            pObj.lock()->Get_FontRenderer()->Get_Text() = L"";
            pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
        }
        else if (strInfo == pObj.lock()->Get_Name())
        {
            _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
            vecPos.x = fPosX;
            pObj.lock()->Get_FontRenderer()->Get_Text() = L"";
            pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
        }
    }
}

void UiCardDeckController::Create_Select_Mark()
{
    if (true == m_bIsSelectMarkCreate)
        return;

    m_bIsSelectMarkCreate = true;

    auto pScene = CUR_SCENE;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Card_Select.dat", m_vecSelectMark);
}

void UiCardDeckController::Remove_Select_Mark()
{
    if (false == m_bIsSelectMarkCreate)
        return;

    m_bIsSelectMarkCreate = false;
    m_bIsMoveDown = true;

    auto& pEventMgr = EVENTMGR;

    for (_uint i = 0; i < IDX(m_vecSelectMark.size()); ++i)
    {
        auto& pObj = m_vecSelectMark[i];
        if (false == pObj.expired())
        {
            pEventMgr.Delete_Object(pObj.lock());
            pObj.reset();
        }
    }

    m_vecSelectMark.clear();
}

void UiCardDeckController::Move_Select_Mark()
{
    if (false == m_bIsSelectMarkCreate)
        return;

    m_fCheckTime += fDT;
    if (m_fMaxTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;
        m_bIsMoveDown = !m_bIsMoveDown;
    }

    if (true == m_vecSelectMark[0].expired() ||
        true == m_vecSelectMark[1].expired() ||
        true == m_vecSelectMark[2].expired())
        return;

    _float4 vecPos = m_vecSelectMark[0].lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    _float fY = {};
    if (true == m_bIsMoveDown)
        fY = vecPos.y - fDT * m_fSpeed;
    else
        fY = vecPos.y + fDT * m_fSpeed;

    vecPos.y = fY;
    m_vecSelectMark[0].lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);

    vecPos = m_vecSelectMark[1].lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.y = fY;
    m_vecSelectMark[1].lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);

    vecPos = m_vecSelectMark[2].lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.y = fY;
    m_vecSelectMark[2].lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
}

void UiCardDeckController::Check_Scroll()
{
    _uint iUseSize = DATAMGR.Get_Card_Inven_Use_Size();
    if (32 > iUseSize)
        return;

    _float fPosY = m_pScroll.lock()->GetOrAddTransform()->Get_State(Transform_State::POS).y;
    fPosY -= 450.f; // 0 ~ -900 값
    
    // -340이 최대값이 되야함
    _float fRatio = m_fMaxPosY + 340.f;
    fRatio /= 900.f;

    fPosY *= fRatio;
    for (_uint i = 0; i < iUseSize; ++i)
    {
        auto& pObj = m_vecInvenObj[i];
        if (true == pObj.expired())
            continue;

        _float4 vecPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
        vecPos.y = 430.f - static_cast<_float>(i / 4) * 110.f + fPosY;
        pObj.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);

        pObj.lock()->Get_Button()->Get_Desc().ptCenter.x = static_cast<LONG>(vecPos.x + g_iWinSizeX / 2);
        pObj.lock()->Get_Button()->Get_Desc().ptCenter.y = static_cast<LONG>(vecPos.y * -1.f + g_iWinSizeY / 2);
    }

}
