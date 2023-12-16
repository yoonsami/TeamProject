#include "pch.h"
#include "InstancingMgr.h"

#include "MeshEffect.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "BaseCollider.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "ParticleSystem.h"
#include "StructuredBuffer.h"
#include "WeedGroup.h"
#include "Camera.h"

void InstancingMgr::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	Clear_Data();

	Render_MeshRenderer(gameObjects);

	Render_ModelRenderer(gameObjects);  

	Render_Animator(gameObjects);

	//Render_Effect(gameObjects);

}

void InstancingMgr::Render_Weeds(vector<shared_ptr<GameObject>>& groups)
{
	vector<shared_ptr<GameObject>> gameObjects;

	for (auto& group : groups)
	{
		if(_float3::Transform(group->Get_CullPos(), Camera::Get_View()).LengthSquared() > 100.f * 100.f)
			continue;

		auto& groupWeeds = group->Get_WeedGroup()->Get_Weeds();
		gameObjects.insert(gameObjects.end(), groupWeeds.begin(), groupWeeds.end());

	}

	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
	vector<shared_ptr<GameObject>> nonInstancing;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->Get_MeshRenderer() == nullptr)
			continue;

		//auto frustum = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_Frustum();

		/*	if (frustum.Contain_Sphere(gameObject->Get_CullPos(), gameObject->Get_CullRadius()) == false)
				continue;*/

		if (gameObject->Get_Instancing())
		{
			const InstanceID instanceID = gameObject->Get_MeshRenderer()->Get_InstanceID();
			cache[instanceID].push_back(gameObject);
		}
		else
			nonInstancing.push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		// sorting
		vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec.front()->Get_MeshRenderer()->Render();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data;
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				data.renderParam = groups.front()->Get_WeedGroup()->Get_RenderParams();
				Add_Data(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_MeshRenderer()->Render_Instancing(buffer);
		}
	}
	for (auto& gameObject : nonInstancing)
	{
		gameObject->Get_MeshRenderer()->Render();
	}

}

void InstancingMgr::Render_Shadow(vector<shared_ptr<GameObject>>& gameObjects)
{
	Clear_Data();

	Render_ModelRenderer_Shadow(gameObjects);
	Render_Animator_Shadow(gameObjects);

}

void InstancingMgr::Clear_Data()
{
	for (auto& pair : m_Buffers)
	{
		pair.second->Clear_Data();
	}
	m_Buffers.clear();
}

void InstancingMgr::Render_MeshRenderer(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
	vector<shared_ptr<GameObject>> nonInstancing;
	for (auto& gameObject : gameObjects)
	{
		if(gameObject->Get_MeshRenderer() == nullptr)
			continue;

		if (gameObject->Get_Instancing())
		{
			const InstanceID instanceID = gameObject->Get_MeshRenderer()->Get_InstanceID();
			cache[instanceID].push_back(gameObject);
		}
		else
			nonInstancing.push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		// sorting
		vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec.front()->Get_MeshRenderer()->Render();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data;
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				data.renderParam = gameobject->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0];
				Add_Data(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_MeshRenderer()->Render_Instancing(buffer);
		}
	}
	for (auto& gameObject : nonInstancing)
	{
		gameObject->Get_MeshRenderer()->Render();
	}
}

void InstancingMgr::Render_ModelRenderer(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
	vector<shared_ptr<GameObject>> nonInstancing;
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->Get_ModelRenderer() == nullptr)
			continue;
		if (!gameObject->Get_ModelRenderer()->Is_RenderOn())
			continue;
		if (gameObject->Get_Instancing())
		{
			const InstanceID instanceID = gameObject->Get_ModelRenderer()->Get_InstanceID();
			cache[instanceID].push_back(gameObject);
		}
		else
			nonInstancing.push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec.front()->Get_ModelRenderer()->Render();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data{};
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				data.renderParam = gameobject->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[0];
				Add_Data(instanceId, data);
			}
			
			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_ModelRenderer()->Render_Instancing(buffer);
		}
	}
	for (auto& gameObject : nonInstancing)
		gameObject->Get_ModelRenderer()->Render();
}

void InstancingMgr::Render_Animator(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
	vector<shared_ptr<GameObject>> nonInstancing;
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->Get_Animator() == nullptr)
			continue;

		if(!gameObject->Get_Animator()->Is_RenderOn())
			continue;

		if (gameObject->Get_Instancing())
		{
			const InstanceID instanceID = gameObject->Get_Animator()->Get_InstanceID();
			cache[instanceID].push_back(gameObject);
		}
		else
			nonInstancing.push_back(gameObject);
	}


	for (auto& pair : cache)
	{
		shared_ptr<InstanceTweenDesc> tweenDesc = make_shared<InstanceTweenDesc>();
		vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec.front()->Get_Animator()->Render();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data;
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				data.renderParam = gameobject->Get_Animator()->Get_RenderParamDesc().vec4Params[0];
				Add_Data(instanceId, data);

				tweenDesc->tweens[i] = gameobject->Get_Animator()->Get_TweenDesc();
			}
			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_Animator()->Render_Instancing(buffer,tweenDesc);
		}
	}
	for (auto& gameObject : nonInstancing)
		gameObject->Get_Animator()->Render();
}

void InstancingMgr::Render_Collider(vector<shared_ptr<GameObject>>& gameObjects)
{

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->Get_Collider() == nullptr)
			continue;

		gameObject->Get_Collider()->Render();
	}
}

//void InstancingMgr::Render_Effect(vector<shared_ptr<GameObject>>& gameObjects)
//{
//	for (auto& gameObject : gameObjects)
//	{
//		if (gameObject->Get_EffectRenderer() == nullptr)
//			continue;
//
//		gameObject->Get_EffectRenderer()->Render();
//	}
//}

void InstancingMgr::Render_ModelRenderer_Shadow(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;

	vector<shared_ptr<GameObject>> nonInstancing;
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->Get_ModelRenderer() == nullptr)
			continue;
		
		if (!gameObject->Get_ModelRenderer()->Is_RenderOn())
			continue;

		if (gameObject->Get_Instancing())
		{
			const InstanceID instanceID = gameObject->Get_ModelRenderer()->Get_InstanceID();
			cache[instanceID].push_back(gameObject);
		}
		else
			nonInstancing.push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec.front()->Get_ModelRenderer()->Render_Shadow();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data;
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				data.renderParam = gameobject->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[0];
				Add_Data(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_ModelRenderer()->Render_Shadow_Instancing(buffer);
		}
	}
	for (auto& gameObject : nonInstancing)
		gameObject->Get_ModelRenderer()->Render_Shadow();
}

void InstancingMgr::Render_Animator_Shadow(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
	vector<shared_ptr<GameObject>> nonInstancing;
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->Get_Animator() == nullptr)
			continue;

		if (!gameObject->Get_Animator()->Is_RenderOn())
			continue;
		
		if (gameObject->Get_Instancing())
		{
			const InstanceID instanceID = gameObject->Get_Animator()->Get_InstanceID();
			cache[instanceID].push_back(gameObject);
		}
		else
			nonInstancing.push_back(gameObject);
	}


	for (auto& pair : cache)
	{
		shared_ptr<InstanceTweenDesc> tweenDesc = make_shared<InstanceTweenDesc>();

		vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec.front()->Get_Animator()->Render_Shadow();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data;
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				data.renderParam = gameobject->Get_Animator()->Get_RenderParamDesc().vec4Params[0];
				Add_Data(instanceId, data);

				tweenDesc->tweens[i] = gameobject->Get_Animator()->Get_TweenDesc();

			}
			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_Animator()->Render_Shadow_Instancing(buffer, tweenDesc);
		}
	}
	for (auto& gameObject : nonInstancing)
		gameObject->Get_Animator()->Render_Shadow();
}

void InstancingMgr::Render_MeshEffect(list <weak_ptr<GameObject>>& gameObjects)
{
	Clear_Data();
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject.expired())
			continue;

		if (gameObject.lock()->Get_MeshEffect() == nullptr)
			continue;

		const InstanceID instanceID = gameObject.lock()->Get_MeshEffect()->Get_InstanceID();
		cache[instanceID].push_back(gameObject.lock());
	}

	for (auto& pair : cache)
	{
		vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec.front()->Get_MeshEffect()->Render();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			vector<RenderParams> paramInfo(vec.size());
			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data{};
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();

				paramInfo[i] = gameobject->Get_MeshEffect()->Get_RenderParamDesc();
				Add_Data(instanceId, data);
			}

			shared_ptr<StructuredBuffer> pRenderParamBuffer = make_shared<StructuredBuffer>(paramInfo.data(), static_cast<_uint>(sizeof RenderParams), _uint(vec.size()));

			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_MeshEffect()->Render_Instancing(buffer, pRenderParamBuffer);
		}
	}
}

void InstancingMgr::Add_Data(InstanceID instanceId, InstancingData& data)
{
	if (m_Buffers.find(instanceId) == m_Buffers.end())
		m_Buffers[instanceId] = make_shared<InstancingBuffer>();

	m_Buffers[instanceId]->Add_Data(data);

}
