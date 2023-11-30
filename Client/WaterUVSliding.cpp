#include "pch.h"
#include "WaterUVSliding.h"
#include "ModelRenderer.h"

void WaterUVSliding::Tick()
{
	if (Get_Owner()->Get_ModelRenderer())
	{
		Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec2Params[0] -= _float2(0.01,0.03) * fDT;
		Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec2Params[1] += _float2(0.03f) * fDT;
		Get_Owner()->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[0].x = 20.f;
	}


}
