#include "pch.h"
#include "MeshEffect.h"

#include "Model.h"
#include "ModelMesh.h"
#include "Utils.h"
#include "MathUtils.h"
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
	MeshEffectData::DESC* pDesc = (MeshEffectData::DESC*)pArg;
	m_tDesc = *pDesc;

	Update_Desc();
}

void MeshEffect::Tick()
{
	// For. Transform
	Translate();
	Scaling();
	Turn();
}

void MeshEffect::Final_Tick()
{
	if (m_bIsPlayFinished)
		return;
	m_fCurrAge += fDT;

	// For. Check is dead 
	if (m_fCurrAge >= m_fDuration)
	{
		CUR_SCENE->Remove_GameObject(Get_Owner());
		return;
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
	_float fNoise = _float(rand() % 11) / 10.f;
	Color vRangStartColor = Color(m_tDesc.vDiffuseColor_BaseStart.x, m_tDesc.vDiffuseColor_BaseStart.y, m_tDesc.vDiffuseColor_BaseStart.z, m_tDesc.vDiffuseColor_BaseStart.w);
	Color vRangEndColor = Color(m_tDesc.vDiffuseColor_BaseEnd.x, m_tDesc.vDiffuseColor_BaseEnd.y, m_tDesc.vDiffuseColor_BaseEnd.z, m_tDesc.vDiffuseColor_BaseEnd.w);
	m_vDiffuseColor_Base = vRangStartColor * fNoise + vRangEndColor * (1.f - fNoise);

	m_fDuration = MathUtils::Get_RandomFloat(m_tDesc.vParticleDuration.x, m_tDesc.vParticleDuration.y);

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

void MeshEffect::InitialTransform(_float3 vParentPos, _float3 vParentScale, _float3 vParentRotation)
{
	// For. Position, Scale, Rotation 
	m_vStartPos += vParentPos;
	m_vStartRotation += vParentRotation;
	
	m_vEndPos += m_vStartPos;

	Get_Transform()->Set_State(Transform_State::POS, _float4(m_vStartPos, 1.f));
	Get_Transform()->Scaled(m_vStartScale);
	Get_Transform()->Set_Rotation(m_vStartRotation);
}

void MeshEffect::Set_TransformDesc(void* pArg)
{
	// For. Setting basic info  
	MeshEffectData::Transform_Desc* pDesc = (MeshEffectData::Transform_Desc*)pArg;

	// For. Initial Transform 
	m_vStartPos = _float3(
		MathUtils::Get_RandomFloat(pDesc->vPosRange.x / 2.f * (-1.f), pDesc->vPosRange.x / 2.f),
		MathUtils::Get_RandomFloat(pDesc->vPosRange.y / 2.f * (-1.f), pDesc->vPosRange.y / 2.f),
		MathUtils::Get_RandomFloat(pDesc->vPosRange.z / 2.f * (-1.f), pDesc->vPosRange.z / 2.f)
	);

	m_vStartScale = _float3(
		MathUtils::Get_RandomFloat(pDesc->vInitScale_Min.x, pDesc->vInitScale_Max.x),
		MathUtils::Get_RandomFloat(pDesc->vInitScale_Min.y, pDesc->vInitScale_Max.y),
		MathUtils::Get_RandomFloat(pDesc->vInitScale_Min.z, pDesc->vInitScale_Max.z)
	);

	m_vStartRotation = _float3(
		MathUtils::Get_RandomFloat(pDesc->vInitRotation_Min.x, pDesc->vInitRotation_Max.x),
		MathUtils::Get_RandomFloat(pDesc->vInitRotation_Min.y, pDesc->vInitRotation_Max.y),
		MathUtils::Get_RandomFloat(pDesc->vInitRotation_Min.z, pDesc->vInitRotation_Max.z)
	);

	// For. Translate
	m_iTranslateOption = pDesc->iTranslateOption;
	m_fTranslateSpeed = pDesc->fTranslateSpeed;
	m_vEndPos = m_vStartPos + _float3(
		MathUtils::Get_RandomFloat(pDesc->vEndPosOffset_Min.x, pDesc->vEndPosOffset_Max.x),
		MathUtils::Get_RandomFloat(pDesc->vEndPosOffset_Min.y, pDesc->vEndPosOffset_Max.y),
		MathUtils::Get_RandomFloat(pDesc->vEndPosOffset_Min.z, pDesc->vEndPosOffset_Max.z)
	);

	m_vEndScale = pDesc->vEndScale;
	m_vEndScale += m_vStartScale;
	
	m_iTurnOption = pDesc->iTurnOption;
	m_fTurnSpeed = pDesc->fTurnSpeed;
	m_vRandomAxis = _float3(
		MathUtils::Get_RandomFloat(pDesc->vRandomAxis_Min.x, pDesc->vRandomAxis_Max.x),
		MathUtils::Get_RandomFloat(pDesc->vRandomAxis_Min.y, pDesc->vRandomAxis_Max.y),
		MathUtils::Get_RandomFloat(pDesc->vRandomAxis_Min.z, pDesc->vRandomAxis_Max.z)
	);
}

void MeshEffect::Translate()
{
	switch (m_iTranslateOption)
	{
	case 0:
		break;
	case 1: // Move to target position
	case 2: // Move to random target position
		Get_Transform()->Set_State(Transform_State::POS, _float4(XMVectorLerp(m_vStartPos, m_vEndPos, m_fCurrAge), 0.f));
		break;
	case 3:
		Get_Transform()->Go_Straight();
		break;
	case 4:
		Get_Transform()->Go_Backward();
		break;
	case 5:
		Get_Transform()->Go_Left();
		break;
	case 6:
		Get_Transform()->Go_Right();
		break;
	case 7:
		// TODO: Spreading dust
		break;
	case 8:
		// TODO: Scattered embers
		break;
	}
}

void MeshEffect::Scaling()
{
	_float3 vScale = XMVectorLerp(m_vStartScale, m_vEndScale, m_fCurrAge);
	Get_Transform()->Scaled(vScale);
}

void MeshEffect::Turn()
{
	if (m_vRandomAxis.x == 0.f && m_vRandomAxis.y == 0.f && m_vRandomAxis.z == 0.f)
		return;
	Get_Transform()->Turn(m_vRandomAxis, m_fTurnSpeed);
}

void MeshEffect::Init_RenderParams()
{	
	m_RenderParams.SetInt(0, m_tDesc.iSamplerType);
	m_RenderParams.SetInt(1, m_tDesc.bUseFadeOut);
	m_RenderParams.SetInt(2, m_tDesc.bIsOverlayOn);
	m_RenderParams.SetInt(3, m_tDesc.bInverseDissolve);

	m_RenderParams.SetFloat(0, m_fCurrAge / m_tDesc.fDuration);
	m_RenderParams.SetFloat(1, m_tDesc.fAlphaGraIntensity);
	m_RenderParams.SetFloat(2, m_tDesc.fContrast);
	
	m_RenderParams.SetVec2(0, m_vCurrTexUVOffset_Opacity);

	_float4 vUVOffset = _float4(m_vCurrTexUVOffset_Gra.x, m_vCurrTexUVOffset_Gra.y, m_vCurrTexUVOffset_Overlay.x, m_vCurrTexUVOffset_Overlay.y);
	m_RenderParams.SetVec4(0, vUVOffset);
	vUVOffset = _float4(m_vCurrTexUVOffset_Dissolve.x, m_vCurrTexUVOffset_Dissolve.y, m_vCurrTexUVOffset_Distortion.x, m_vCurrTexUVOffset_Distortion.y);
	m_RenderParams.SetVec4(1, vUVOffset);

	_float4x4 mColor = _float4x4(m_vDiffuseColor_Base,
							     m_tDesc.vBaseColor_AlphaGra,
							     m_tDesc.vBaseColor_Gra,
							     m_tDesc.vBaseColor_Overlay);
	m_RenderParams.SetMatrix(0, mColor);
}

void MeshEffect::Bind_RenderParams_ToShader()
{
	// For. Update LifeTimeRatio
	_float fLifeTimeRatio = m_fCurrAge / m_tDesc.fDuration;
	m_RenderParams.SetFloat(0, fLifeTimeRatio);

	// For. Update ChangingColor
	Color vFinalDiffuseColor = XMVectorLerp(m_vDiffuseColor_Base, m_tDesc.vDestColor_Diffuse, fLifeTimeRatio);
	Color vFinalAlphaGraColor = XMVectorLerp(m_tDesc.vBaseColor_AlphaGra, m_tDesc.vDestColor_AlphaGra, fLifeTimeRatio);
	Color vFinalGraColor = XMVectorLerp(m_tDesc.vBaseColor_Gra, m_tDesc.vDestColor_Gra, fLifeTimeRatio);
	_float4x4 mColor = _float4x4(vFinalDiffuseColor,
								 vFinalAlphaGraColor,
								 vFinalGraColor,
								 m_tDesc.vBaseColor_Overlay);
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

_float MeshEffect::GetRandomFloatInRange(_float fStart, _float fEnd)
{
	uniform_real_distribution<float>	RandomInRange(fStart, fEnd);
	_float fResult = RandomInRange(m_RandomNumber);
	return fResult;
}
