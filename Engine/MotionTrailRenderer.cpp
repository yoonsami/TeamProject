#include "pch.h"
#include "MotionTrailRenderer.h"
#include "Camera.h"
#include "Model.h"
#include "ModelMesh.h"
#include "ModelAnimator.h"
#include "Utils.h"

MotionTrailRenderer::MotionTrailRenderer(shared_ptr<Shader> shader, shared_ptr<ModelAnimator> animtor)
    : Component(COMPONENT_TYPE::MotionTrailRenderer)
	, m_pShader(shader)
	, m_TweenDesc(animtor->Get_TweenDesc())
	, m_preTweenDesc(animtor->Get_preTweenDesc())
	, m_pModel(animtor->Get_Model())
{
	m_iPass = PS_ANIM;
}

MotionTrailRenderer::MotionTrailRenderer(shared_ptr<Shader> shader, const TweenDesc& desc, shared_ptr<Model> model)
	: Component(COMPONENT_TYPE::MotionTrailRenderer)
	, m_pShader(shader)
	, m_TweenDesc(desc)
	, m_preTweenDesc(desc)
	, m_pModel(model)
{
	m_iPass = PS_ANIM;
}

MotionTrailRenderer::MotionTrailRenderer(shared_ptr<Shader> shader, shared_ptr<Model> model)
	: Component(COMPONENT_TYPE::MotionTrailRenderer)
	, m_pShader(shader)
	, m_pModel(model)
{
	m_iPass = PS_NONANIM;
}

MotionTrailRenderer::~MotionTrailRenderer()
{
}

void MotionTrailRenderer::Tick()
{
}

void MotionTrailRenderer::Render()
{
	if (!m_pModel || ! m_pShader)
		return;

	m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());
	m_pShader->Push_RenderParamData(m_RenderParams);

	
	if (m_fTrailScale == 1.f)
	{
		auto& world = Get_Transform()->Get_WorldMatrix();
		auto& preWorld = Get_Transform()->Get_preWorldMatrix();
		m_pShader->Push_TransformData(TransformDesc{ world,preWorld });
	}
	else
	{
		Get_Transform()->Scaled(_float3(m_fTrailScale));
		_float4x4 matWorld = Get_Transform()->Get_WorldMatrix();
		Get_Transform()->Scaled(_float3(1.f, 1.f, 1.f));
		m_pShader->Push_TransformData(TransformDesc{ matWorld, matWorld });
	}

	if(m_iPass == PS_ANIM)
	{
		m_pShader->Push_TweenData({ m_TweenDesc,m_preTweenDesc });

		m_pShader->GetSRV("TransformMap")->SetResource(m_pModel->Get_TransformSRV().Get());

		auto& desc = m_pModel->Get_AnimAddonDesc();

		m_pShader->Push_AnimAddonData(desc);
	}
	else if (m_iPass == PS_NONANIM)
	{
		// Bones
		shared_ptr<BoneDesc> boneDesc = make_shared<BoneDesc>();

		const _uint boneCount = m_pModel->Get_BoneCount();

		for (_uint i = 0; i < boneCount; ++i)
		{
			shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(i);
			boneDesc->transform[i] = bone->transform * Utils::m_matPivot;
		}
		m_pShader->Push_BoneData(*boneDesc);
	}

	const auto& meshes = m_pModel->Get_Meshes();

	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();

		m_pShader->DrawIndexed(3, m_iPass, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
	}
}

void MotionTrailRenderer::Set_Scale(_float fScale)
{
	m_fTrailScale = fScale;
}

/*void MotionTrailRenderer::Render_Instancing(shared_ptr<class InstancingBuffer>& buffer, shared_ptr<InstanceTweenDesc> desc, shared_ptr<InstanceRenderParamDesc> renderParamDesc)
{
	if (!m_pModel)
		return;

	{
		m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());
	}

	m_pShader->Push_InstanceTweenData(*desc);
	m_pShader->Push_InstanceRenderParamData(*renderParamDesc);
	m_pShader->GetSRV("TransformMap")->SetResource(m_pModel->Get_TransformSRV().Get());

	auto& animAddonDesc = m_pModel->Get_AnimAddonDesc();

	m_pShader->Push_AnimAddonData(animAddonDesc);

	const auto& meshes = m_pModel->Get_Meshes();

	for (auto& mesh : meshes)
	{
		if (!mesh->material.expired())
			mesh->material.lock()->Tick();

		m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->Push_Data();
		mesh->indexBuffer->Push_Data();

		buffer->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pShader->DrawIndexedInstanced(3, PS_ANIMINSTANCING + 1, mesh->indexBuffer->Get_IndicesNum(), buffer->Get_Count());
	}
}

InstanceID MotionTrailRenderer::Get_InstanceID()
{
	return make_pair(_ulonglong(m_pModel.get()), _ulonglong(m_pShader.get()));
}*/
