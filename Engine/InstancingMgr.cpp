#include "pch.h"
#include "InstancingMgr.h"

#include "GameObject.h"
#include "MeshRenderer.h"
#include "BaseCollider.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "ParticleSystem.h"

void InstancingMgr::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	Clear_Data();

	Render_MeshRenderer(gameObjects);

	Render_ModelRenderer(gameObjects);  

	Render_Animator(gameObjects);

	//Render_Effect(gameObjects);

}

void InstancingMgr::Render_Shadow(vector<shared_ptr<GameObject>>& gameObjects)
{
	Clear_Data();

	Render_ModelRenderer_Shadow(gameObjects);
	Render_Animator_Shadow(gameObjects);

}

void InstancingMgr::Render_VelocityMap(vector<shared_ptr<GameObject>>& gameObjects)
{
	Clear_Data();

	Render_ModelRenderer_VelocityMap(gameObjects);
	Render_Animator_VelocityMap(gameObjects);
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
	shared_ptr<InstanceRenderParamDesc> materialDesc = make_shared<InstanceRenderParamDesc>();
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
				data.preWorld = gameobject->Get_Transform()->Get_preWorldMatrix();

				materialDesc->params[i] = gameobject->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[0];
				Add_Data(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_MeshRenderer()->Render_Instancing(buffer, materialDesc);
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
	shared_ptr<InstanceRenderParamDesc> materialDesc = make_shared<InstanceRenderParamDesc>();
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
				data.preWorld = gameobject->Get_Transform()->Get_preWorldMatrix();

				materialDesc->params[i] = gameobject->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[0];
				Add_Data(instanceId, data);
			}
			
			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_ModelRenderer()->Render_Instancing(buffer,materialDesc);
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
		shared_ptr<InstanceRenderParamDesc> materialDesc = make_shared<InstanceRenderParamDesc>();
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
				data.preWorld = gameobject->Get_Transform()->Get_preWorldMatrix();
				Add_Data(instanceId, data);

				tweenDesc->tweens[i] = gameobject->Get_Animator()->Get_TweenDesc();
				tweenDesc->preTweens[i] = gameobject->Get_Animator()->Get_preTweenDesc();
				materialDesc->params[i] = gameobject->Get_Animator()->Get_RenderParamDesc().vec4Params[0];
			}
			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_Animator()->Render_Instancing(buffer,tweenDesc,materialDesc);
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
	shared_ptr<InstanceRenderParamDesc> materialDesc = make_shared<InstanceRenderParamDesc>();

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
				data.preWorld = gameobject->Get_Transform()->Get_preWorldMatrix();

				materialDesc->params[i] = gameobject->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[0];
				Add_Data(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_ModelRenderer()->Render_Shadow_Instancing(buffer, materialDesc);
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
		shared_ptr<InstanceRenderParamDesc> materialDesc = make_shared<InstanceRenderParamDesc>();

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
				data.preWorld = gameobject->Get_Transform()->Get_preWorldMatrix();

				Add_Data(instanceId, data);

				tweenDesc->tweens[i] = gameobject->Get_Animator()->Get_TweenDesc();
				materialDesc->params[i] = gameobject->Get_Animator()->Get_RenderParamDesc().vec4Params[0];
			}
			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_Animator()->Render_Shadow_Instancing(buffer, tweenDesc, materialDesc);
		}
	}
	for (auto& gameObject : nonInstancing)
		gameObject->Get_Animator()->Render_Shadow();
}

void InstancingMgr::Render_ModelRenderer_VelocityMap(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
	shared_ptr<InstanceRenderParamDesc> materialDesc = make_shared<InstanceRenderParamDesc>();

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
			vec.front()->Get_ModelRenderer()->Render_MotionBlur();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data;
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				data.preWorld = gameobject->Get_Transform()->Get_preWorldMatrix();

				materialDesc->params[i] = gameobject->Get_ModelRenderer()->Get_RenderParamDesc().vec4Params[0];
				Add_Data(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_ModelRenderer()->Render_MotionBlur_Instancing(buffer, materialDesc);
		}
	}
	for (auto& gameObject : nonInstancing)
		gameObject->Get_ModelRenderer()->Render_MotionBlur();
}

void InstancingMgr::Render_Animator_VelocityMap(vector<shared_ptr<GameObject>>& gameObjects)
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
		shared_ptr<InstanceRenderParamDesc> materialDesc = make_shared<InstanceRenderParamDesc>();

		vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec.front()->Get_Animator()->Render_MotionBlur();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (size_t i = 0; i < vec.size(); ++i)
			{
				shared_ptr<GameObject>& gameobject = vec[i];
				InstancingData data;
				data.world = gameobject->Get_Transform()->Get_WorldMatrix();
				data.preWorld = gameobject->Get_Transform()->Get_preWorldMatrix();

				Add_Data(instanceId, data);

				tweenDesc->tweens[i] = gameobject->Get_Animator()->Get_TweenDesc();
				materialDesc->params[i] = gameobject->Get_Animator()->Get_RenderParamDesc().vec4Params[0];
			}
			shared_ptr<InstancingBuffer>& buffer = m_Buffers[instanceId];
			vec[0]->Get_Animator()->Render_MotionBlur_Instancing(buffer, tweenDesc, materialDesc);
		}
	}
	for (auto& gameObject : nonInstancing)
		gameObject->Get_Animator()->Render_MotionBlur();
}

void InstancingMgr::Add_Data(InstanceID instanceId, InstancingData& data)
{
	if (m_Buffers.find(instanceId) == m_Buffers.end())
		m_Buffers[instanceId] = make_shared<InstancingBuffer>();

	m_Buffers[instanceId]->Add_Data(data);

}
