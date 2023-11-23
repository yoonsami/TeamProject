#include "pch.h"
#include "UiCardDeckInvenChange.h"

#include "Material.h"
#include "MeshRenderer.h"

UiCardDeckInvenChange::UiCardDeckInvenChange(_uint iType)
    : m_iType(iType)
{
}

HRESULT UiCardDeckInvenChange::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    //m_pOwner.lock()->Set_Render(false);
    //m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 0.f;

    return S_OK;
}

void UiCardDeckInvenChange::Tick()
{
	if (m_pOwner.expired())
		return;

    
}

void UiCardDeckInvenChange::Set_Hero(HERO eHero)
{
    if (true == m_pOwner.expired())
        return;

    m_eHero = eHero;

    m_bIsSetHero = true;
    // m_pOwner.lock()->Set_Render(true);
    if(0 == m_iType)
        m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(GET_DATA(eHero).KeyDeckMini), TextureMapType::DIFFUSE);
    else if(1 == m_iType)
    {
        wstring strTemp = GET_ELEMENT(eHero);
        m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(strTemp), TextureMapType::DIFFUSE);
    }

    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f;
}

void UiCardDeckInvenChange::Set_Empty()
{
    if (true == m_pOwner.expired())
        return;

    m_bIsSetHero = false;
    //m_pOwner.lock()->Set_Render(false);
    m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(nullptr, TextureMapType::DIFFUSE);
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 0.f;
}
