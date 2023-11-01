#include "pch.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "ModelMesh.h"

ModelRenderer::ModelRenderer(shared_ptr<Shader> shader)
	:Component(COMPONENT_TYPE::ModelRenderer)
	, m_pShader(shader)
{

}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::Render()
{
	if (m_pModel == nullptr)
		return;

	// Set VP
	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());

	// Transform
	auto& world = Get_Transform()->Get_WorldMatrix();
	m_pShader->Push_TransformData(TransformDesc{ world });

	// Set Light
	auto& LightParam = CUR_SCENE->Get_LightParams();
	m_pShader->Push_LightData(LightParam);

	m_pShader->Push_RenderParamData(m_RenderParams);

	// Bones
	shared_ptr<BoneDesc> boneDesc = make_shared<BoneDesc>();

	const _uint boneCount = m_pModel->Get_BoneCount();

	for (_uint i = 0; i < boneCount; ++i)
	{
		shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(i);
		boneDesc->transform[i] = bone->transform * _float4x4::CreateRotationY(XM_PI);
	}
	m_pShader->Push_BoneData(*boneDesc);

	m_pShader->GetVector("g_UVSliding")->SetFloatVector((_float*)(&m_vUvSilding));

	const auto& meshes = m_pModel->Get_Meshes();
	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();
		_float4 lineColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pShader->GetVector("g_LineColor")->SetFloatVector((_float*)(&lineColor));
		m_pShader->GetScalar("g_LineThickness")->SetFloat(Model::m_fOutlineThickness);
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (mesh->material.lock()->Has_Outline())
		{
			
			m_pShader->DrawIndexed(1, PS_NONANIM, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
			
		}
		
		if (m_bCullNone)
		{
			m_pShader->DrawIndexed(0, PS_NONANIM_CULLNONE, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
		}
		else
		{
			
			m_pShader->DrawIndexed(0, PS_NONANIM, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
		}
		
	}
}

void ModelRenderer::Render_Skybox()
{
	if (m_pModel == nullptr)
		return;

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());

	shared_ptr<BoneDesc> boneDesc = make_shared<BoneDesc>();
	const _uint boneCount = m_pModel->Get_BoneCount();
	_float4 tmp = _float4(0.f);

	for (_uint i = 0; i < boneCount; ++i)
	{
		shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(i);
		boneDesc->transform[i] = bone->transform * _float4x4::CreateScale(0.0001f) * _float4x4::CreateRotationY(XM_PI);
	}
	m_pShader->Push_BoneData(*boneDesc);

	m_pShader->GetVector("g_UVSliding")->SetFloatVector((_float*)(&m_vUvSilding));
	// Transform
	m_pShader->Push_TransformData(TransformDesc{ _float4x4::Identity });

	const auto& meshes = m_pModel->Get_Meshes();

	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		// BoneIndex
		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pShader->DrawIndexed(0, m_iPass, mesh->indexBuffer->Get_IndicesNum(), 0, 0);

	}
}

void ModelRenderer::Render_Shadow()
{
	if (m_pModel == nullptr)
		return;

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());

	// Bones
	shared_ptr<BoneDesc> boneDesc = make_shared<BoneDesc>();

	const _uint boneCount = m_pModel->Get_BoneCount();

	for (_uint i = 0; i < boneCount; ++i)
	{
		shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(i);
		boneDesc->transform[i] = bone->transform *  _float4x4::CreateRotationY(XM_PI);
	}
	m_pShader->Push_BoneData(*boneDesc);

	// Transform
	auto world = Get_Transform()->Get_WorldMatrix();
	m_pShader->Push_TransformData(TransformDesc{ world });

	const auto& meshes = m_pModel->Get_Meshes();
	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		// BoneIndex
		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pShader->DrawIndexed(0, PS_NONANIM_SHADOW, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
	}
}

void ModelRenderer::Render_Shadow_Instancing(shared_ptr<InstancingBuffer>& buffer, shared_ptr<InstanceRenderParamDesc> renderParamDesc)
{
	if (m_pModel == nullptr)
		return;

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());

	m_pShader->Push_InstanceRenderParamData(*renderParamDesc);
	// Bones
	shared_ptr<BoneDesc> boneDesc = make_shared<BoneDesc>();

	const _uint boneCount = m_pModel->Get_BoneCount();

	for (_uint i = 0; i < boneCount; ++i)
	{
		shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(i);
		boneDesc->transform[i] = (bone->transform) * _float4x4::CreateRotationY(XM_PI);
	}
	m_pShader->Push_BoneData(*boneDesc);

	const auto& meshes = m_pModel->Get_Meshes();
	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		// BoneIndex
		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();

		buffer->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pShader->DrawIndexedInstanced(0, PS_NONANIM_SHADOW_INSTANCE, mesh->indexBuffer->Get_IndicesNum(), buffer->Get_Count());
	}
}

void ModelRenderer::Set_Model(shared_ptr<Model> model)
{
	m_pModel = model;

	const auto& materials = m_pModel->Get_Materials();

	for (auto& material : materials)
	{
		material->Set_Shader(m_pShader);
	}

}

void ModelRenderer::Render_Instancing(shared_ptr<class InstancingBuffer>& buffer, shared_ptr<InstanceRenderParamDesc> renderParamDesc)
{
	if (m_pModel == nullptr)
		return;

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());

	m_pShader->Push_InstanceRenderParamData(*renderParamDesc);
	// Bones

	shared_ptr<BoneDesc> boneDesc = make_shared<BoneDesc>();
	const _uint boneCount = m_pModel->Get_BoneCount();

	for (_uint i = 0; i < boneCount; ++i)
	{
		shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(i);
		boneDesc->transform[i] = (bone->transform)  * _float4x4::CreateRotationY(XM_PI);
	}
	m_pShader->Push_BoneData(*boneDesc);
	m_pShader->GetVector("g_UVSliding")->SetFloatVector((_float*)(&m_vUvSilding));
	const auto& meshes = m_pModel->Get_Meshes();
	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		// BoneIndex
		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();

		buffer->Push_Data();


		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_float4 lineColor = _float4(0.f, 0.f, 0.f, 1.f);
		m_pShader->GetVector("g_LineColor")->SetFloatVector((_float*)(&lineColor));
		m_pShader->GetScalar("g_LineThickness")->SetFloat(m_pModel->m_fOutlineThickness);

		if (mesh->material.lock()->Has_Outline())
		{
			m_pShader->DrawIndexedInstanced(1, PS_NONANIMINSTANCE, mesh->indexBuffer->Get_IndicesNum(), buffer->Get_Count());
		}


		if (m_bCullNone)
			m_pShader->DrawIndexedInstanced(0, PS_NONANIM_CULLNONE_INSTANCE, mesh->indexBuffer->Get_IndicesNum(), buffer->Get_Count());
		else
			m_pShader->DrawIndexedInstanced(0, PS_NONANIMINSTANCE, mesh->indexBuffer->Get_IndicesNum(), buffer->Get_Count());


	}
}

InstanceID ModelRenderer::Get_InstanceID()
{
	return make_pair((_ulonglong)m_pModel.get(), (_ulonglong)m_pShader.get());
}
