#include "pch.h"
#include "WeedGroup.h"
#include "ModelRenderer.h"
#include "MeshRenderer.h"

WeedGroup::WeedGroup()
	:Component(COMPONENT_TYPE::WeedGroup)
{
}

WeedGroup::~WeedGroup()
{
}

HRESULT WeedGroup::Init()
{
	return S_OK;
}

void WeedGroup::Tick()
{
	for (auto& Weed : m_Weeds)
	{
		Weed->Tick();
	}
}




