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
	for (_uint i = 1; i < 8; i++)
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
	m_RenderParams.SetInt(0, m_tDesc.bOverlayOn);
	m_RenderParams.SetInt(1, m_tDesc.bChangingColorOn);
	m_RenderParams.SetInt(2, m_tDesc.bUseFadeOut);

	m_RenderParams.SetFloat(0, m_fCurrAge / m_tDesc.fDuration);
	m_RenderParams.SetFloat(1, m_tDesc.fGradationIntensity);

	m_RenderParams.SetVec2(0, _float2(m_tDesc.vDestBaseColor.x, m_tDesc.vDestBaseColor.y));
	m_RenderParams.SetVec2(1, _float2(m_tDesc.vDestBaseColor.z, m_tDesc.vDestBaseColor.w));
	m_RenderParams.SetVec2(2, _float2(m_tDesc.vDestGradationColor.x, m_tDesc.vDestGradationColor.y));
	m_RenderParams.SetVec2(3, _float2(m_tDesc.vDestGradationColor.z, m_tDesc.vDestGradationColor.w));

	m_RenderParams.SetVec4(0, m_tDesc.vBaseColor);
	m_RenderParams.SetVec4(1, m_tDesc.vGradationColor);
	m_RenderParams.SetVec4(2, m_tDesc.vOverlayColor_Start);
	m_RenderParams.SetVec4(3, m_tDesc.vOverlayColor_End);
}

void MeshEffect::Bind_RenderParams_ToShader()
{
	m_RenderParams.SetFloat(0, m_fCurrAge / m_tDesc.fDuration);

	m_pShader->Push_RenderParamData(m_RenderParams);
}
