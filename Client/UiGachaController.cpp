#include "pch.h"
#include "UiGachaController.h" 

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

    m_vecObjBgTag.resize(3);
    m_vecObjBgTag[0] = L"UI_Gacha_Bg0";
    m_vecObjBgTag[1] = L"UI_Gacha_Bg1";
    m_vecObjBgTag[2] = L"UI_Gacha_Bg2";

    return S_OK;
}

void UiGachaController::Tick()
{
	if (m_pOwner.expired())
		return;

    // test code
    if (KEYTAP(KEY_TYPE::Q))
    {
        Create_Gacha_Card();
    }
    if (KEYTAP(KEY_TYPE::E))
    {
        Delete_Gacha_Card();
        Delete_Gacha_Bg();
    }
}

void UiGachaController::Create_Gacha_Card()
{
    auto pScene = CUR_SCENE;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Gacha.dat");

    _uint iSize = IDX(m_vecObjTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto pObj = pScene->Get_GameObject(m_vecObjTag[i]);
        auto pScript = make_shared<UiGachaCardMove>(i);
        pObj->Add_Component(pScript);
        pObj->Init();
    }
    
    iSize = IDX(m_vecObjEffectTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto pObj = pScene->Get_GameObject(m_vecObjEffectTag[i]);
        auto pScript = make_shared<UiGachaEffectController>();
        pObj->Add_Component(pScript);
        pObj->Init();
    }

}

void UiGachaController::Delete_Gacha_Card()
{
    _uint iSize = IDX(m_vecObjTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto pScene = CUR_SCENE;
        pScene->Remove_GameObject(pScene->Get_GameObject(m_vecObjTag[i]));
    }
}

void UiGachaController::Delete_Gacha_Bg()
{
    _uint iSize = IDX(m_vecObjBgTag.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto pScene = CUR_SCENE;
        pScene->Remove_GameObject(pScene->Get_GameObject(m_vecObjBgTag[i]));
    }
}
