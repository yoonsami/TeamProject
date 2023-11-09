#include "pch.h"
#include "MeshEffect.h"

#include "Model.h"
#include "Utils.h"
#include "Camera.h"
#include "ModelMesh.h"

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

	m_vCurrTexUVOffset = m_tDesc.vTexTiling;

	// For. Model Components
	m_pModel = RESOURCES.Get<Model>(Utils::ToWString(m_tDesc.strVfxMesh));

	m_pMaterial = make_shared<Material>();
	m_pMaterial->Set_Shader(m_pShader);

	// For. Material Components
	for (_uint i = 1; i < 8; i++)
	{
		if ("None" != m_tDesc.strTextures[i])
		{
			wstring strPath = TEXT("../Resources/Textures/Universal/") + Utils::ToWString(m_tDesc.strTextures[i]);
			m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(Utils::ToWString(m_tDesc.strTextures[i]), strPath), (TextureMapType)i);
		}
	}
    return S_OK;
}

void MeshEffect::Tick()
{
}

void MeshEffect::Final_Tick()
{
	m_fCurrAge += fDT;

	if (m_fCurrAge >= m_tDesc.fDuration)
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

void MeshEffect::Init_RenderParams()
{	
	m_RenderParams.SetInt(0, m_tDesc.bOverlayOn);	
	m_RenderParams.SetInt(1, m_tDesc.bUseFadeOut);
	m_RenderParams.SetInt(2, m_tDesc.bGradationOn);
	m_RenderParams.SetInt(3, m_tDesc.iSamplerType);

	m_RenderParams.SetFloat(0, m_fCurrAge / m_tDesc.fDuration);
	m_RenderParams.SetFloat(1, m_tDesc.fGradationIntensity);

	m_RenderParams.SetVec2(0, m_vCurrTexUVOffset);

	m_RenderParams.SetVec4(0, m_tDesc.vBaseColor);
	m_RenderParams.SetVec4(1, m_tDesc.vGradationColor);
	m_RenderParams.SetVec4(2, m_tDesc.vOverlayColor_Start);
	m_RenderParams.SetVec4(3, m_tDesc.vOverlayColor_End);
}

void MeshEffect::Bind_RenderParams_ToShader()
{
	_float fLifeTimeRatio = m_fCurrAge / m_tDesc.fDuration;
	Color vFinalBaseColor = XMVectorLerp(m_tDesc.vBaseColor, m_tDesc.vDestBaseColor, fLifeTimeRatio);
	Color vFinalGradationColor = XMVectorLerp(m_tDesc.vGradationColor, m_tDesc.vDestGradationColor, fLifeTimeRatio);
	Color vFinalOverlayColor_Start = XMVectorLerp(m_tDesc.vOverlayColor_Start, m_tDesc.vDestOverlayColor_Start, fLifeTimeRatio);
	Color vFinalOverlayColor_End = XMVectorLerp(m_tDesc.vOverlayColor_End, m_tDesc.vDestOverlayColor_End, fLifeTimeRatio);

	m_RenderParams.SetFloat(0, fLifeTimeRatio);
	m_RenderParams.SetVec4(0, vFinalBaseColor);
	m_RenderParams.SetVec4(1, vFinalGradationColor);
	m_RenderParams.SetVec4(2, vFinalOverlayColor_Start);
	m_RenderParams.SetVec4(3, vFinalOverlayColor_End);

	m_vCurrTexUVOffset += m_tDesc.vTexUVSpeed * fDT;
	m_RenderParams.SetVec2(0, m_vCurrTexUVOffset);

	m_pShader->Push_RenderParamData(m_RenderParams);
}
