#include "pch.h"
#include "MapObjectScript.h"
MapObjectScript::MapObjectScript(shared_ptr<ModelRenderer> _renderer, ModelRenderer::INSTANCE_PASSTYPE _PassType, _float _fUVWeight)
{
	_renderer->Set_PassType(_PassType);
	m_fUVWeight = _fUVWeight;
	_renderer->SetFloat(3, m_fUVWeight);
}

HRESULT MapObjectScript::Init()
{
	return S_OK;
}

void MapObjectScript::Tick()
{
}
