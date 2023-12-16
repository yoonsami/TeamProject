#include "pch.h"
#include "UiDamageCreate.h" 

#include "Camera.h"
#include "FontRenderer.h"
#include "MeshRenderer.h"
#include "UiDamageMove.h"
#include "CoolTimeCheckScript.h"
#include "Utils.h"

UiDamageCreate::UiDamageCreate()
{
}

HRESULT UiDamageCreate::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_pPlayer = GET_PLAYER;
    m_pCamera = CUR_SCENE->Get_Camera(L"Default");

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    return S_OK;
}

void UiDamageCreate::Create_Damage_Font(weak_ptr<GameObject> pTarget, _float fDamage, ElementType eType)
{
    if (true == m_pOwner.expired() ||
        true == m_pPlayer.expired() ||
        true == m_pCamera.expired())
        return;

    if (true == pTarget.expired())
        return;

    _uint iDamage = IDX(fDamage);
    if (0 == iDamage)
        return;

    _uint iMaxSize = 0;
    while (0 != iDamage)
    {
        ++iMaxSize;
        iDamage /= 10;
    }
    iDamage = IDX(fDamage);

    wstring strKey = L"Damage_";
    switch (eType)
    {
    case DARK:
        strKey += L"Dark_";
        break;
    case EARTH:
        strKey += L"Earth_";
        break;
    case FIRE:
        strKey += L"Fire_";
        break;
    case LIGHT:
        strKey += L"Light_";
        break;
    case WATER:
        strKey += L"Water_";
        break;
    case WIND:
        strKey += L"Wind_";
        break;
    case ElementEnd:
        return;
    }

    auto pScene = CUR_SCENE;
    vector<weak_ptr<GameObject>> addedObj;
    pScene->Load_UIFile(L"..\\Resources\\UIData\\UI_Damage.dat", addedObj, iMaxSize);

    _float2 vecRandAdd = { Utils::Random_In_Range(-80.f, 80.f), Utils::Random_In_Range(-80.f, 80.f) };

    shared_ptr<GameObject> pFirstNum;
    for (_uint i = 0; i < iMaxSize; ++i)
    {
        if (true == addedObj[i].expired())
            continue;

        auto& pObj = addedObj[i];

        _uint iOneDigit = iDamage % 10;
        wstring strTemp = strKey + to_wstring(iOneDigit);
        iDamage /= 10;
        if (0 == i)
        {
            pFirstNum = pObj.lock();
            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(strTemp), TextureMapType::DIFFUSE);
            pObj.lock()->Add_Component(make_shared<UiDamageMove>(pTarget, i, vecRandAdd));
        }
        else
        {
            pObj.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(strTemp), TextureMapType::DIFFUSE);
            pObj.lock()->Add_Component(make_shared<UiDamageMove>(pTarget, i, vecRandAdd, pFirstNum));
        }

        pObj.lock()->Init();
        EVENTMGR.Create_Object(pObj.lock());
    }

    return;
}

void UiDamageCreate::Create_Damage_Hit()
{

}