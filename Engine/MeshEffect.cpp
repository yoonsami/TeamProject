#include "pch.h"
#include "MeshEffect.h"

#include "Model.h"
#include "Utils.h"

MeshEffect::MeshEffect(shared_ptr<Shader> shader)
	: Component(COMPONENT_TYPE::MeshEffect)
	, m_pShader(shader)
{

}

MeshEffect::~MeshEffect()
{
}

HRESULT MeshEffect::Init(void* pArg)
{
	// For. Setting basic info  
	DESC* pDesc = (DESC*)pArg;
	m_tDesc = *pDesc;

	Init_RenderParams();

	// For. Model Components
	m_pModel = RESOURCES.Get<Model>(Utils::ToWString(m_tDesc.strVfxMesh));

	// For. Material Components
	m_pMaterial = make_shared<Material>();
	m_pMaterial->Set_Shader(m_pShader);
	for (_uint i = 0; i < 8; i++)
	{
		if ("None" != m_tDesc.strTextures[i])
		{
			wstring wstrTag = TEXT("EffectTexture_") + i;
			m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrTag, Utils::ToWString(m_tDesc.strTextures[i])), (TextureMapType)i);
		}
	}

    return S_OK;
}

void MeshEffect::Tick()
{
}

void MeshEffect::Final_Tick()
{
}

void MeshEffect::Render()
{

	// For. Material Tick ( for Bind Texture to Shader )
	m_pMaterial->Tick();
}

void MeshEffect::Init_RenderParams()
{	
	// Duration and Current time 
	m_RenderParams.SetVec2(0, _float2(m_fCurrAge, m_tDesc.fDuration));

	m_RenderParams.SetVec4(0, m_tDesc.vStartColor);
	m_RenderParams.SetVec4(1, m_tDesc.vDestColor);

	m_RenderParams.SetFloat(0, m_tDesc.fGradationIntensity);
}

void MeshEffect::Bind_RenderParams_ToShader()
{
	m_RenderParams.SetFloat(3, m_fCurrAge);

	m_pShader->Push_RenderParamData(m_RenderParams);
}
