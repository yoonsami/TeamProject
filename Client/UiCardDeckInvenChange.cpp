#include "pch.h"
#include "UiCardDeckInvenChange.h"

#include "Material.h"
#include "MeshRenderer.h"

UiCardDeckInvenChange::UiCardDeckInvenChange()
{
}

HRESULT UiCardDeckInvenChange::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_pOwner.lock()->Set_Render(false);
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 0.f;

    return S_OK;
}

void UiCardDeckInvenChange::Tick()
{
	if (m_pOwner.expired())
		return;

    
}

void UiCardDeckInvenChange::Set_Hero(HERO eHero)
{
    if (m_pOwner.expired())
        return;

    m_pOwner.lock()->Set_Render(true);

    m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(GET_DATA(eHero).KeyDeckMini), TextureMapType::DIFFUSE);
}

void UiCardDeckInvenChange::Set_Empty()
{
    if (m_pOwner.expired())
        return;

    m_pOwner.lock()->Set_Render(false);
    m_pOwner.lock()->Get_MeshRenderer()->Get_Material()->Set_TextureMap(nullptr, TextureMapType::DIFFUSE);
}
