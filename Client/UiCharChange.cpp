#include "pch.h"
#include "UiCharChange.h"

#include "Material.h"
#include "MeshRenderer.h"
#include "HeroChangeScript.h"

UiCharChange::UiCharChange()
{
}

HRESULT UiCharChange::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_vecDesc.resize(3);
    m_vecDesc[0].fCoolTime = 10.f;
    m_vecDesc[1].fCoolTime = 10.f;
    m_vecDesc[2].fCoolTime = 10.f;

    /*m_vecDesc[0].fAccTime = m_vecDesc[0].fCoolTime;
    m_vecDesc[1].fAccTime = m_vecDesc[1].fCoolTime;
    m_vecDesc[2].fAccTime = m_vecDesc[2].fCoolTime;*/

    m_pObj.resize(3);
    m_pObj[0] = CUR_SCENE->Get_UI(L"UI_Char_Change0");
    m_pObj[1] = CUR_SCENE->Get_UI(L"UI_Char_Change1");
    m_pObj[2] = CUR_SCENE->Get_UI(L"UI_Char_Change2");
    
    m_pElement.resize(3);
    m_pElement[0] = CUR_SCENE->Get_UI(L"UI_Char_Change_Element0");
    m_pElement[1] = CUR_SCENE->Get_UI(L"UI_Char_Change_Element1");
    m_pElement[2] = CUR_SCENE->Get_UI(L"UI_Char_Change_Element2");

    m_eHero.resize(3);
    m_eHero[0] = HERO::MAX;
    m_eHero[1] = HERO::MAX;
    m_eHero[2] = HERO::MAX;


    return S_OK;
}

void UiCharChange::Tick()
{
	if (m_pOwner.expired())
		return;

    Check_Change_Cool();

    Set_Param_Value();

    Change_Hero();
}

void UiCharChange::Set_Hero(_uint iIndex, HERO eHero)
{
    if (m_pOwner.expired() || 2 < iIndex)
        return;
    
    if (true == m_pObj[iIndex].expired() || true == m_pElement[iIndex].expired())
        return;

    // UI의 Index 위치에 업데이트할 영웅 사진 껴주기
    if (HERO::MAX == eHero)
    {
        m_pObj[iIndex].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(L"Card_None"), TextureMapType::DIFFUSE);
        m_pElement[iIndex].lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 0.f;
        m_eHero[iIndex] = HERO::MAX;
    }
    else
    {
        m_pObj[iIndex].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap((RESOURCES.Get<Texture>(GET_DATA(eHero).KeyChangeCard)), TextureMapType::DIFFUSE);
        m_pElement[iIndex].lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(GET_ELEMENT(eHero)), TextureMapType::DIFFUSE);
        m_pElement[iIndex].lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f;
        m_eHero[iIndex] = eHero;
    }
}

_bool UiCharChange::IsChangePossible(_uint iIndex)
{
    if (m_pOwner.expired() || 2 < iIndex)
        return false;

    auto& Desc = m_vecDesc[iIndex];
    if(HERO::MAX == Desc.eHero)
        return false;

    if (Desc.fCoolTime < Desc.fAccTime)
    {
        // Hero Change Script 호출
        // Desc.eHero의 영웅 값을 따라서 호출
        
        return true;
    }

    return false;
}

void UiCharChange::Check_Change_Cool()
{
    _uint iSize = IDX(m_vecDesc.size());
    for (_uint i = 0; i < iSize; ++i)
    {
        auto& Desc = m_vecDesc[i];
        if (HERO::MAX == Desc.eHero)
        {
            Desc.fAccTime = Desc.fCoolTime;
        }
        else
        {
            if (Desc.fCoolTime > Desc.fAccTime)
                Desc.fAccTime += fDT;
        }
    }
}

void UiCharChange::Set_Param_Value()
{
    _uint iSize = IDX(m_vecDesc.size());
    for (_uint i = 0; i < iSize; ++i)
        if (true == m_pObj[i].expired())
            return;

    for (_uint i = 0; i < iSize; ++i)
    {
        m_pObj[i].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = m_vecDesc[i].fAccTime / m_vecDesc[i].fCoolTime;
        if (1.f <= m_pObj[i].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0])
            m_pObj[i].lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = 1.f;
    }
}

void UiCharChange::Change_Hero()
{
    if (KEYTAP(KEY_TYPE::F5))
    {
        if (HERO::MAX == m_eHero[0])
            return;

        // hero change script
        auto pScript = CUR_SCENE->Get_GameObject(L"Player")->Get_Script<HeroChangeScript>();

        pScript->Change_Hero(m_eHero[0]);
    }

    if (KEYTAP(KEY_TYPE::F6))
    {
        if (HERO::MAX == m_eHero[1])
            return;

        auto pScript = CUR_SCENE->Get_GameObject(L"Player")->Get_Script<HeroChangeScript>();

        pScript->Change_Hero(m_eHero[1]);
    }

    if (KEYTAP(KEY_TYPE::F7))
    {
        if (HERO::MAX == m_eHero[2])
            return;

        auto pScript = CUR_SCENE->Get_GameObject(L"Player")->Get_Script<HeroChangeScript>();

        pScript->Change_Hero(m_eHero[2]);
    }
}
