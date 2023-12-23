#include "pch.h"
#include "Glow_WitcherSense.h"
#include "ModelRenderer.h"

Glow_WitcherSense::Glow_WitcherSense(shared_ptr<GameObject> pPlayer)
	: m_pPlayer(pPlayer)
{
}

void Glow_WitcherSense::Tick()
{
	if (m_pPlayer.expired() || m_pOwner.expired())
		return;

	_float3 vPlayerPos = m_pPlayer.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vOwenrPos = Get_Transform()->Get_State(Transform_State::POS).xyz();

	if ((vPlayerPos - vOwenrPos).LengthSquared() < 15.f * 15.f && g_bWitcherSenseOn)
	{
		if(Get_Owner()->Get_ModelRenderer())
		{
			Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[1].x = 1.2f;
			Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[1].y = 0.f;
			Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[1].z = 0.f;
		}

	}
	else
	{
		if (Get_Owner()->Get_ModelRenderer())
		{
			Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[1].x = 0.f;
			Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[1].y = 0.f;
			Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[1].z = 0.f;
		}
	}
}
