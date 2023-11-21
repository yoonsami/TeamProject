#include "pch.h"
#include "UiGachaController.h" 

#include "BaseUI.h"
#include "UiGachaCardMove.h"
#include "UiGachaEffectController.h"

UiGachaController::UiGachaController()
{
}

HRESULT UiGachaController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_vecObjTag.resize(10);
    m_vecObjTag[0] = L"UI_Gacha_Card0";
    m_vecObjTag[1] = L"UI_Gacha_Card1";
    m_vecObjTag[2] = L"UI_Gacha_Card2";
    m_vecObjTag[3] = L"UI_Gacha_Card3";
    m_vecObjTag[4] = L"UI_Gacha_Card4";
    m_vecObjTag[5] = L"UI_Gacha_Card5";
    m_vecObjTag[6] = L"UI_Gacha_Card6";
    m_vecObjTag[7] = L"UI_Gacha_Card7";
    m_vecObjTag[8] = L"UI_Gacha_Card8";
    m_vecObjTag[9] = L"UI_Gacha_Card9";

    m_vecObjEffectTag.resize(3);
    m_vecObjEffectTag[0] = L"UI_Gacha_Effect0";
    m_vecObjEffectTag[1] = L"UI_Gacha_Effect1";
    m_vecObjEffectTag[2] = L"UI_Gacha_Effect2";

    m_vecObjBgTag.resize(4);
    m_vecObjBgTag[0] = L"UI_Gacha_Bg0";
    m_vecObjBgTag[1] = L"UI_Gacha_Bg1";
    m_vecObjBgTag[2] = L"UI_Gacha_Bg2";
    m_vecObjBgTag[3] = L"UI_Gacha_Bg3";

    m_vecObjButton.resize(4);
    m_vecObjButton[0] = L"UI_Gacha_Open_Button";
    m_vecObjButton[1] = L"UI_Gacha_Close_Button";
    m_vecObjButton[2] = L"UI_Gacha_Font_Open";
    m_vecObjButton[3] = L"UI_Gacha_Font_Close";

    m_iSize = IDX(m_vecObjTag.size());

    return S_OK;
}

void UiGachaController::Tick()
{
    if (m_pOwner.expired())
        return;

    if (true == m_bIsStartOpen)
        Start_All_Open();

    //test code
    if (KEYTAP(KEY_TYPE::Q))
    {
        Create_Gacha_Card();
    }
    if (KEYTAP(KEY_TYPE::E))
    {
        Delete_All();
    }
}

void UiGachaController::Create_Gacha_Card()
{
    if (true == m_bIsCreate)
        return;

    m_bIsCreate = true;
    auto pScene = CUR_SCENE;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Gacha.dat");

    _uint iSize = IDX(m_vecObjTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        weak_ptr<GameObject> pObj = pScene->Get_UI(m_vecObjTag[i]);
        if (true == pObj.expired())
            continue;
        auto pScript = make_shared<UiGachaCardMove>(i);
        pObj.lock()->Add_Component(pScript);
        pObj.lock()->Init();

        pObj.lock()->Get_Button()->AddOnClickedEvent([pObj]()
            {
                pObj.lock()->Get_Script<UiGachaCardMove>()->Card_Open();
            });
    }

    iSize = IDX(m_vecObjEffectTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        weak_ptr<GameObject> pObj = pScene->Get_UI(m_vecObjEffectTag[i]);
        if (true == pObj.expired())
            continue;
        auto pScript = make_shared<UiGachaEffectController>();
        pObj.lock()->Add_Component(pScript);
        pObj.lock()->Init();
    }


    {
        weak_ptr<GameObject> pObj = pScene->Get_UI(m_vecObjButton[0]);
        if (true == pObj.expired())
            return;
        pObj.lock()->Get_Button()->AddOnClickedEvent([]()
            {
                weak_ptr<GameObject> pObject = CUR_SCENE->Get_GameObject(L"UI_Gacha_Controller");
                if (true == pObject.expired())
                    return;

                pObject.lock()->Get_Script<UiGachaController>()->Open_All_Card();
            });

        pObj = pScene->Get_UI(m_vecObjButton[1]);
        if (true == pObj.expired())
            return;
        pObj.lock()->Get_Button()->AddOnClickedEvent([]()
            {
                weak_ptr<GameObject> pObject = CUR_SCENE->Get_GameObject(L"UI_Gacha_Controller");
                if (true == pObject.expired())
                    return;

                pObject.lock()->Get_Script<UiGachaController>()->Delete_All();
            });
    }


}

void UiGachaController::Delete_Gacha_Card()
{
    auto pScene = CUR_SCENE;

    _uint iSize = IDX(m_vecObjTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        weak_ptr<GameObject> pObj = pScene->Get_UI(m_vecObjTag[i]);
        if (true == pObj.expired())
            continue;

        pScene->Remove_GameObject(pObj.lock());
    }
}

void UiGachaController::Delete_Gacha_Bg()
{
    auto pScene = CUR_SCENE;

    _uint iSize = IDX(m_vecObjBgTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        weak_ptr<GameObject> pObj = pScene->Get_UI(m_vecObjBgTag[i]);
        if (true == pObj.expired())
            continue;

        pScene->Remove_GameObject(pObj.lock());
    }
}

void UiGachaController::Delete_Gacha_Button()
{
    auto pScene = CUR_SCENE;

    _uint iSize = IDX(m_vecObjButton.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        weak_ptr<GameObject> pObj = pScene->Get_UI(m_vecObjButton[i]);
        if (true == pObj.expired())
            continue;

        pScene->Remove_GameObject(pObj.lock());
    }
}

void UiGachaController::Start_All_Open()
{
    if (m_iIndex == m_iSize)
        return;

    m_fCheckTime += fDT;
    if (m_fMaxTime < m_fCheckTime)
    {
        weak_ptr<GameObject> pObj = CUR_SCENE->Get_UI(m_vecObjTag[m_iIndex]);
        if (true == pObj.expired())
            return;

        pObj.lock()->Get_Script<UiGachaCardMove>()->Card_Open();

        m_fCheckTime = 0.f;
        m_iIndex++;
    }
}

void UiGachaController::Open_All_Card()
{
    if (true == m_bIsStartOpen)
        return;

    m_bIsStartOpen = true;
    m_fCheckTime = 0.f;
    m_fMaxTime = 0.15f;
    m_iIndex = 0;
}

void UiGachaController::Delete_All()
{
    m_bIsCreate = false;
    m_bIsStartOpen = false;

    Delete_Gacha_Card();
    Delete_Gacha_Bg();
    Delete_Gacha_Button();
}
