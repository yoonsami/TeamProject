#include "pch.h"
#include "MeshEffect.h"

#include "Model.h"
#include "ModelMesh.h"
#include "Utils.h"
#include "Camera.h"

MeshEffect::MeshEffect(shared_ptr<Shader> shader)
	: Component(COMPONENT_TYPE::MeshEffect)
	, m_pShader(shader)
{

}

MeshEffect::~MeshEffect()
{
}

void MeshEffect::Init(void* pArg)
{
	// For. Setting basic info  
	DESC* pDesc = (DESC*)pArg;
	m_tDesc = *pDesc;

	Update_Desc();
}

void MeshEffect::Tick()
{
	if (m_bIsPlayFinished)
		return;
}

void MeshEffect::Final_Tick()
{
	if (m_bIsPlayFinished && m_bIsAlwaysShowFirstTick)
		return;

	m_fCurrAge += fDT;

	if (m_fCurrAge >= m_tDesc.fDuration)
	{
		if (!m_bIsImmortal)
		{
			CUR_SCENE->Remove_GameObject(Get_Owner());
			return;
		}
		else
			m_bIsPlayFinished = true;

		if (m_bIsPlayLoop)
		{
			Update_Desc();
			m_fCurrAge = 0.f;

			m_bIsPlayFinished = true;
		}
	}
}

void MeshEffect::Render()
{
	if (m_pModel == nullptr)
		return;

	// Bones
	shared_ptr<BoneDesc> boneDesc = make_shared<BoneDesc>();

	const _uint boneCount = m_pModel->Get_BoneCount();

	for (_uint i = 0; i < boneCount; ++i)
	{
		shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(i);
		boneDesc->transform[i] = bone->transform * Utils::m_matPivot;
	}
	m_pShader->Push_BoneData(*boneDesc);

	// For. Bind data to shader 
	auto world = Get_Transform()->Get_WorldMatrix();
	m_pShader->Push_TransformData(TransformDesc{ world });

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());
	Bind_RenderParams_ToShader();

	// For. Draw meshes 
	const auto& meshes = m_pModel->Get_Meshes();
	for (auto& mesh : meshes)
	{
		// For. Material tick 
		if (!mesh->material.expired())
		{
			mesh->vertexBuffer->Push_Data();
			mesh->indexBuffer->Push_Data();
		}
		m_pMaterial->Tick();

		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		// For. Setting Context Topology
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// For. Draw call
		m_pShader->DrawIndexed(0, 0, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
	}
}

void MeshEffect::Update_Desc()
{
	if (m_bIsAlwaysShowFirstTick)
		m_bIsPlayFinished = true;

	Init_RenderParams();

	m_vCurrTexUVOffset_Opacity = m_tDesc.vTiling_Opacity;
	m_vCurrTexUVOffset_Gra = m_tDesc.vTiling_Gra;
	m_vCurrTexUVOffset_Overlay = m_tDesc.vTiling_Overlay;
	m_vCurrTexUVOffset_Dissolve = m_tDesc.vTiling_Dissolve;
	m_vCurrTexUVOffset_Distortion = m_tDesc.vTiling_Distortion;

	// For. Model Components
	m_pModel = RESOURCES.Get<Model>(Utils::ToWString(m_tDesc.strVfxMesh));

	// For. Material Components
	m_pMaterial = make_shared<Material>();
	m_pMaterial->Set_Shader(m_pShader);
	wstring wstrKey = Utils::ToWString(m_tDesc.strDiffuseTexture);
	wstring wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
	if (TEXT("None") != wstrKey)
		m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::DIFFUSE);
	wstrKey = Utils::ToWString(m_tDesc.strOpacityTexture);
	wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
	if (TEXT("None") != wstrKey)
		m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::OPACITY);
	wstrKey = Utils::ToWString(m_tDesc.strNormalTexture);
	wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
	if (TEXT("None") != wstrKey)
		m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::NORMAL);
	wstrKey = Utils::ToWString(m_tDesc.strDissolveTexture);
	wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
	if (TEXT("None") != wstrKey)
		m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::DISSOLVE);
	wstrKey = Utils::ToWString(m_tDesc.strDistortionTexture);
	wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
	if (TEXT("None") != wstrKey)
		m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::DISTORTION);
	wstrKey = Utils::ToWString(m_tDesc.strGraTexture);
	wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
	if (TEXT("None") != wstrKey)
		m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE7);	// Gradation
	wstrKey = Utils::ToWString(m_tDesc.strOverlayTexture);
	wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
	if (TEXT("None") != wstrKey)
		m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE8);	// Overlay
	wstrKey = Utils::ToWString(m_tDesc.strBlendTexture);
	wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
	if (TEXT("None") != wstrKey)
		m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE9);	// Blend
}

void MeshEffect::Init_RenderParams()
{	
	m_RenderParams.SetInt(0, m_tDesc.iSamplerType);
	m_RenderParams.SetInt(1, m_tDesc.bUseFadeOut);
	m_RenderParams.SetInt(2, m_tDesc.bIsOverlayOn);

	m_RenderParams.SetFloat(0, m_fCurrAge / m_tDesc.fDuration);
	m_RenderParams.SetFloat(1, m_tDesc.fAlphaGraIntensity);
	m_RenderParams.SetFloat(2, m_tDesc.fContrast);
	
	m_RenderParams.SetVec2(0, m_vCurrTexUVOffset_Opacity);

	_float4 vUVOffset = _float4(m_vCurrTexUVOffset_Gra.x, m_vCurrTexUVOffset_Gra.y, m_vCurrTexUVOffset_Overlay.x, m_vCurrTexUVOffset_Overlay.y);
	m_RenderParams.SetVec4(0, vUVOffset);
	vUVOffset = _float4(m_vCurrTexUVOffset_Dissolve.x, m_vCurrTexUVOffset_Dissolve.y, m_vCurrTexUVOffset_Distortion.x, m_vCurrTexUVOffset_Distortion.y);
	m_RenderParams.SetVec4(1, vUVOffset);

	_float4x4 mColor = _float4x4(m_tDesc.BaseColor_Diffuse,
							     m_tDesc.BaseColor_AlphaGra,
							     m_tDesc.BaseColor_Gra,
							     m_tDesc.BaseColor_Overlay);
	m_RenderParams.SetMatrix(0, mColor);
}

void MeshEffect::Bind_RenderParams_ToShader()
{
	// For. Update LifeTimeRatio
	_float fLifeTimeRatio = m_fCurrAge / m_tDesc.fDuration;
	m_RenderParams.SetFloat(0, fLifeTimeRatio);

	// For. Update ChangingColor
	Color vFinalDiffuseColor = XMVectorLerp(m_tDesc.BaseColor_Diffuse, m_tDesc.DestColor_Diffuse, fLifeTimeRatio);
	Color vFinalAlphaGraColor = XMVectorLerp(m_tDesc.BaseColor_AlphaGra, m_tDesc.DestColor_AlphaGra, fLifeTimeRatio);
	Color vFinalGraColor = XMVectorLerp(m_tDesc.BaseColor_Gra, m_tDesc.DestColor_Gra, fLifeTimeRatio);
	_float4x4 mColor = _float4x4(vFinalDiffuseColor,
								 vFinalAlphaGraColor,
								 vFinalGraColor,
								 m_tDesc.BaseColor_Overlay);
	m_RenderParams.SetMatrix(0, mColor);

	// For. Update UV Offset
	m_vCurrTexUVOffset_Opacity	+= m_tDesc.vUVSpeed_Opacity * fDT;
	m_vCurrTexUVOffset_Gra		+= m_tDesc.vUVSpeed_Gra * fDT;
	m_vCurrTexUVOffset_Overlay	+= m_tDesc.vUVSpeed_Overlay * fDT;
	m_vCurrTexUVOffset_Dissolve += m_tDesc.vUVSpeed_Dissolve * fDT;
	m_vCurrTexUVOffset_Distortion += m_tDesc.vUVSpeed_Distortion * fDT;
	m_RenderParams.SetVec2(0, m_vCurrTexUVOffset_Opacity);
	_float4 vUVOffset = _float4(m_vCurrTexUVOffset_Gra.x, m_vCurrTexUVOffset_Gra.y, m_vCurrTexUVOffset_Overlay.x, m_vCurrTexUVOffset_Overlay.y);
	m_RenderParams.SetVec4(0, vUVOffset);
	vUVOffset = _float4(m_vCurrTexUVOffset_Dissolve.x, m_vCurrTexUVOffset_Dissolve.y, m_vCurrTexUVOffset_Distortion.x, m_vCurrTexUVOffset_Distortion.y);
	m_RenderParams.SetVec4(1, vUVOffset);

	// For. Bind Data 
	m_pShader->Push_RenderParamData(m_RenderParams);
}
