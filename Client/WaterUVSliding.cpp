#include "pch.h"
#include "WaterUVSliding.h"
#include "ModelRenderer.h"
#include "MeshRenderer.h"

void WaterUVSliding::Tick()
{
	if (Get_Owner()->Get_MeshRenderer())
	{
		Get_Owner()->Get_MeshRenderer()->Get_RenderParamDesc().vec2Params[0] -= _float2(0.01f,0.03f) * fDT;
		Get_Owner()->Get_MeshRenderer()->Get_RenderParamDesc().vec2Params[1] += _float2(0.03f) * fDT;

	}


}
