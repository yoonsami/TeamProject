#include "pch.h"
#include "Effect.h"

#include "Utils.h"

Effect::Effect(shared_ptr<Shader> shader)
	: Component(COMPONENT_TYPE::Effect)
	, m_pShader(shader)
{

}

Effect::~Effect()
{
}

HRESULT Effect::Init(void* pArg)
{
	// For. Setting basic info  
	DESC* pDesc = (DESC*)pArg;
	m_tDesc = *pDesc;

	// For. Model Components
	m_pModel = RESOURCES.Get<Model>(Utils::ToWString(m_tDesc.strVfxMesh));

	// For. Material Components
	m_pMaterial = make_shared<Material>();
	m_pMaterial->Set_Shader(m_pShader);
	for (_uint i = 0; i < 7; i++)
	{
		wstring wstrTag = TEXT("EffectTexture_") + i;
		if (TextureMapType::END != (TextureMapType)m_tDesc.pTexture[i].first)
			m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrTag, Utils::ToWString(m_tDesc.pTexture[i].second)), (TextureMapType)m_tDesc.pTexture[i].first);
	}
	
    return S_OK;
}

void Effect::Tick()
{
}

void Effect::Final_Tick()
{
}

void Effect::Render()
{
}
