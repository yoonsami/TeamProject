#include "pch.h"
#include "Scene.h"
#include "Utils.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include <filesystem>
#include "FileUtils.h"
#include "AABBBoxCollider.h"
#include "OBBBoxCollider.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "MeshRenderer.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "RigidBody.h"
#include "BaseUI.h"

namespace fs = std::filesystem;
Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	auto objects = m_GameObjects;
	for (auto& object : objects)
	{
		object->Init();
	}
}

void Scene::Tick()
{
	auto objects = m_GameObjects;
	for (auto& object : objects)
	{
		object->Tick();
	}
	PickUI();
}

void Scene::Late_Tick()
{
	auto objects = m_GameObjects;
	for (auto& object : objects)
	{
		object->Late_Tick();
	}
}

void Scene::Final_Tick()
{
	auto objects = m_GameObjects;
	for (auto& object : objects)
	{
		object->Final_Tick();
	}


}

void Scene::Render()
{
	Gather_LightData();
	Sort_GameObjects();
	Render_Shadow();
	Render_Deferred();
	Render_Blur();
	Render_Lights();
	//Render_BlurEffect();
	Render_Final();
	Render_Forward();
	Render_Distortion();
	Render_Distortion_Final();

	//Render_Debug();

	Render_UI();

}

HRESULT Scene::Load_Scene()
{
	return S_OK;
}

void Scene::Add_GameObject(shared_ptr<GameObject> object)
{
	m_GameObjects.push_back(object);

	if (object->Get_Camera())
		m_Cameras.push_back(object);

	if (object->Get_Light())
		m_Lights.push_back(object);

	if (object->Get_LayerIndex() == Layer_UI)
		m_UI.push_back(object);
}

void Scene::Remove_GameObject(shared_ptr<GameObject> object)
{
	{
		auto findit = find(m_GameObjects.begin(), m_GameObjects.end(), object);
		if (findit != m_GameObjects.end())
			m_GameObjects.erase(findit);
	}
	{
		auto findit = find(m_Cameras.begin(), m_Cameras.end(), object);
		if (findit != m_Cameras.end())
			m_Cameras.erase(findit);
	}
	{
		auto findit = find(m_Lights.begin(), m_Lights.end(), object);
		if (findit != m_Lights.end())
			m_Lights.erase(findit);
	}
	{
		auto findit = find(m_UI.begin(), m_UI.end(), object);
		if (findit != m_UI.end())
			m_UI.erase(findit);
	}

}

shared_ptr<GameObject> Scene::Get_Camera(const wstring& cameraName)
{
	for (auto& camera : m_Cameras)
	{
		if (camera->Get_Camera()->Get_CameraName() == cameraName)
			return camera;
	}
	return nullptr;
}

shared_ptr<GameObject> Scene::Get_MainCamera()
{
	for (auto& camera : m_Cameras)
	{
		if (camera->Get_Camera()->Get_ProjType() == ProjectionType::Perspective)
			return camera;
	}
	return nullptr;
}

shared_ptr<GameObject> Scene::Get_UICamera()
{
	for (auto& camera : m_Cameras)
	{
		if (camera->Get_Camera()->Get_ProjType() == ProjectionType::Orthographic)
			return camera;
	}
	return nullptr;
}

shared_ptr<GameObject> Scene::Get_GameObject(const wstring& name)
{
	for (auto& gameObject : m_GameObjects)
	{
		if (gameObject->Get_Name() == name)
			return gameObject;
	}
	return nullptr;
}

shared_ptr<GameObject> Scene::Get_UI(const wstring& strName)
{
	for (auto& gameObject : m_UI)
	{
		if (gameObject->Get_Name() == strName)
			return gameObject;
	}
	return nullptr;
}

void Scene::Swap_Object(const wstring& leftObjName, const wstring& rightObjName)
{
	auto leftIter = find_if(m_GameObjects.begin(), m_GameObjects.end(),
		[&](shared_ptr<GameObject>& obj)
		{
			if (obj->Get_Name() == leftObjName)
				return true;

			return false;
		});
	auto rightIter = find_if(m_GameObjects.begin(), m_GameObjects.end(),
		[&](shared_ptr<GameObject>& obj)
		{
			if (obj->Get_Name() == rightObjName)
				return true;

			return false;
		});

	if (leftIter != m_GameObjects.end() && rightIter != m_GameObjects.end())
	{
		auto temp = *leftIter;
		*leftIter = *rightIter;
		*rightIter = temp;

	}

	//auto LeftObj = Get_GameObject(leftObjName);
	//if (!LeftObj)
	//	return;
	//auto RightObj = Get_GameObject(rightObjName);
	//if (!RightObj)
	//	return;
	//auto temp = LeftObj;
	//LeftObj = RightObj;
	//RightObj = temp;
}

void Scene::Load_SceneFile(const wstring& sceneName)
{
	wstring finalPath = m_strSceneDataPath + sceneName + L".Scene";

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Read);


	Set_Name(Utils::ToWString(file->Read<string>()));

	auto& objects = Get_Objects();
	_uint count = file->Read<_uint>();
	


	for (_uint i = 0; i < count; ++i)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->Set_Name(Utils::ToWString(file->Read<string>()));

		obj->Set_LayerIndex(file->Read<_uchar>());
		obj->Get_CullPos() = file->Read<_float3>();
		obj->Set_CullRadius(file->Read<_float>());
		obj->Set_FrustumCulled(file->Read<_bool>());
		obj->Set_DrawShadow(file->Read<_bool>());
		//Transform
		if (file->Read<_bool>())
		{
			obj->GetOrAddTransform()->Set_WorldMat(file->Read<_float4x4>());
			obj->GetOrAddTransform()->Set_Speed(file->Read<_float>());
		}
		//MeshRenderer
		if (file->Read<_bool>())
		{
			shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx"));
			shared_ptr<Mesh> mesh = RESOURCES.Get<Mesh>(Utils::ToWString(file->Read<string>()));

			renderer->Set_Mesh(mesh);

			shared_ptr<Material> material = make_shared<Material>();

			if (file->Read<_bool>())
			{
				wstring path = Utils::ToWString(file->Read<string>());
				wstring key = fs::path(path).filename();
				Utils::DetachExt(key);
				material->Set_TextureMap(RESOURCES.GetOrAddTexture(key, path),TextureMapType::DIFFUSE);
			}
			if (file->Read<_bool>())
			{
				wstring path = Utils::ToWString(file->Read<string>());
				wstring key = fs::path(path).filename();
				Utils::DetachExt(key);
				material->Set_TextureMap(RESOURCES.GetOrAddTexture(key, path), TextureMapType::NORMAL);
			}
			if (file->Read<_bool>())
			{
				wstring path = Utils::ToWString(file->Read<string>());
				wstring key = fs::path(path).filename();
				Utils::DetachExt(key);
				material->Set_TextureMap(RESOURCES.GetOrAddTexture(key, path), TextureMapType::SPECULAR);
			}

			material->Get_MaterialDesc() = file->Read<MaterialDesc>();

			renderer->Set_Material(material);

			obj->Add_Component(renderer);

		}
		//ModelRenderer
		if (file->Read<_bool>())
		{
			shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(RESOURCES.Get<Shader>(L"Shader_Model.fx"));
			shared_ptr<Model> model = RESOURCES.Get<Model>(Utils::ToWString(file->Read<string>()));
			assert(model != nullptr);
			renderer->Set_Model(model);

			renderer->Set_CullMode(file->Read<_bool>());

			obj->Add_Component(renderer);
		}
		//Camera
		if (file->Read<_bool>())
		{
			shared_ptr<Camera> camera = make_shared<Camera>();
			CameraDesc desc;

			desc.eType = file->Read<ProjectionType>();
			desc.strName = Utils::ToWString(file->Read<string>());
			desc.fFOV = file->Read<_float>();
			desc.fNear = file->Read<_float>();
			desc.fFar = file->Read<_float>();
			camera->Get_CameraDesc() = desc;
			obj->Add_Component(camera);
		}
		//Animator
		if (file->Read<_bool>())
		{
			wstring modelTag = Utils::ToWString(file->Read<string>());

			shared_ptr<ModelAnimator> renderer = make_shared<ModelAnimator>(RESOURCES.Get<Shader>(L"Shader_Model.fx"));
			shared_ptr<Model> model = RESOURCES.Get<Model>(Utils::ToWString(file->Read<string>()));
			assert(model != nullptr);
			renderer->Set_Model(model);

			obj->Add_Component(renderer);
		}
		//Light
		if (file->Read<_bool>())
		{
			shared_ptr<Light> light = make_shared<Light>();
			auto lightInfo = file->Read<LightInfo>();
			lightInfo.color.specular = _float4(0.f);
			light->Set_Color(lightInfo.color);
			light->Set_LightRange(lightInfo.range);
			light->Set_LightType(static_cast<LIGHT_TYPE>(lightInfo.lightType));
			light->Set_LightAngle(lightInfo.angle);
			

			obj->Add_Component(light);
		}
		//collider
		if (file->Read<_bool>())
		{
			ColliderType eType = file->Read<ColliderType>();
			_float3 offset = file->Read<_float3>();
			switch (eType)
			{
			case ColliderType::Sphere:
			{
				shared_ptr<SphereCollider> collider = make_shared<SphereCollider>(file->Read<_float>());
				obj->Add_Component(collider);
			}
			break;
			case ColliderType::AABB:
			{
				shared_ptr<AABBBoxCollider> collider = make_shared<AABBBoxCollider>(file->Read<_float3>());
				obj->Add_Component(collider);
			}
			break;
			case ColliderType::OBB:
			{
				_float3 extend = file->Read<_float3>();
				shared_ptr<OBBBoxCollider> collider = make_shared<OBBBoxCollider>(extend);
				obj->Add_Component(collider);
				_float3 colliderCenter = file->Read<_float3>();
				if (file->Read<_bool>())
				{
					_float3 vObjPos = obj->Get_Transform()->Get_State(Transform_State::POS).xyz();
					auto rigidBody = make_shared<RigidBody>();
					obj->Add_Component(rigidBody);
					rigidBody->Create_CapsuleRigidBody(colliderCenter, extend.x, (colliderCenter - vObjPos).y * 2.f);
				}
			}
			break;
			case ColliderType::Mesh:
			{
				shared_ptr<MeshCollider> collider = make_shared<MeshCollider>(Utils::ToWString(file->Read<string>()), obj->GetOrAddTransform()->Get_WorldMatrix());
				obj->Add_Component(collider);
				auto rigidBody = make_shared<RigidBody>();
				rigidBody->Create_RigidBody(collider);
				obj->Add_Component(rigidBody);
			}
			break;
			default:
				assert(false);
				break;
			}
			obj->Get_Collider()->Set_Offset(offset);
		}

		if (obj->Get_Name() == L"ToolCamera")
			continue;
		m_fLoadPercent = clamp(m_fLoadPercent += count / 50.f, 0.f, 100.f);
		Add_GameObject(obj);
	}

}

void Scene::PickUI()
{
	if (!KEYTAP(KEY_TYPE::LBUTTON))
		return;

	if (!Get_UICamera())
		return;

	POINT screenPt = { LONG(INPUT.GetMousePos().x), LONG(INPUT.GetMousePos().y) };

	shared_ptr<Camera> camera = Get_UICamera()->Get_Camera();

	const auto gameobjects = m_GameObjects;

	for (auto& gameObject : gameobjects)
	{
		if(!gameObject->Get_Button())
			continue;

		if (gameObject->Get_Button()->Picked(screenPt))
			gameObject->Get_Button()->InvokeOnClicked();
	}

}

void Scene::Gather_LightData()
{
	ZeroMemory(&m_LightParams, sizeof(m_LightParams));

	auto& lights = m_Lights;
	for (auto& light : m_Lights)
	{
		const LightInfo& lightInfo = light->Get_Light()->Get_LightInfo();

		light->Get_Light()->Set_LightIndex(m_LightParams.lightCount);

		m_LightParams.lights[m_LightParams.lightCount] = lightInfo;
		m_LightParams.lightCount++;
	}
}

void Scene::Sort_GameObjects()
{
	for (auto& camera : m_Cameras)
	{
		camera->Get_Camera()->Sort_GameObject();
	}
}

void Scene::Render_Shadow()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->OMSetRenderTargets();

	for (auto& light : m_Lights)
	{
		if(light->Get_Light()->Get_LightInfo().lightType != static_cast<_int>(LIGHT_TYPE::DIRECTIONAL_LIGHT))
			continue;

		light->Get_Light()->Render_Shadow();

	}
	//GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->UnBindSRV();
}

void Scene::Render_Deferred()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();

	if(Get_MainCamera())
	{
		shared_ptr<Camera> mainCamera = Get_MainCamera()->Get_Camera();
		mainCamera->Render_Deferred();
	}
	//GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->UnBindSRV();
}

void Scene::Render_Blur()
{
	for (_uchar i = 0; i < 2; ++i)
	{
		RENDER_TARGET_GROUP_TYPE eType = static_cast<RENDER_TARGET_GROUP_TYPE>(static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURSMALLER0) + i);
		GRAPHICS.Get_RTGroup(eType)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurSmaller" + to_wstring(i));
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}

	for (_uchar i = 0; i < 2; ++i)
	{
		RENDER_TARGET_GROUP_TYPE eType = static_cast<RENDER_TARGET_GROUP_TYPE>(static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURBIGGER0) + i);
		GRAPHICS.Get_RTGroup(eType)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurBigger" + to_wstring(i));
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 1, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}


	/*{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLURSMALLER0)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurSmaller0");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}
	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLURSMALLER1)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurSmaller1");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}
	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLURSMALLER2)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurSmaller2");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}
	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLURSMALLER3)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurSmaller3");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}*/
	/*{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLURBIGGER0)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurBigger0");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}
	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLURBIGGER1)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurBigger1");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}
	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLURBIGGER2)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurBigger2");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}
	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLURBIGGER3)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurBigger3");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}*/

}

void Scene::Render_Lights()
{
	if (Get_MainCamera())
	{
		shared_ptr<Camera> mainCamera = Get_MainCamera()->Get_Camera();
		Camera::S_View = mainCamera->Get_ViewMat();
		Camera::S_Proj = mainCamera->Get_ProjMat();
	}

	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	

	for (auto& light : m_Lights)
		light->Get_Light()->Render();



	//GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->UnBindSRV();
}

void Scene::Render_Final()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::FINAL)->OMSetRenderTargets();

	auto material = RESOURCES.Get<Material>(L"Final");
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");

	material->Push_SubMapData();

	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	material->Get_Shader()->DrawIndexed(0,3, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
}

void Scene::Render_Forward()
{
	if (Get_MainCamera())
	{
		shared_ptr<Camera> mainCamera = Get_MainCamera()->Get_Camera();
		Camera::S_View = mainCamera->Get_ViewMat();
		Camera::S_Proj = mainCamera->Get_ProjMat();


		mainCamera->Render_Forward();
	}
	//GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->UnBindSRV();
}

void Scene::Render_Distortion()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::DISTORTION)->OMSetRenderTargets();
	if (Get_MainCamera())
	{
		shared_ptr<Camera> mainCamera = Get_MainCamera()->Get_Camera();
		Camera::S_View = mainCamera->Get_ViewMat();
		Camera::S_Proj = mainCamera->Get_ProjMat();


		mainCamera->Render_DistrotionEffects();
	}
}

void Scene::Render_Distortion_Final()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets();
	
	auto material = RESOURCES.Get<Material>(L"Distorted_Final");
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	
	material->Push_SubMapData();
	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();
	
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	
	
}

void Scene::Render_Debug()
{
	for (auto& gameObject : m_GameObjects)
		if (gameObject->Get_Collider() && gameObject->Get_Collider()->Get_ColliderType() != ColliderType::Mesh)
			gameObject->Get_Collider()->Render();
}

void Scene::Render_UI()
{
	if (Get_UICamera())
	{
		shared_ptr<Camera> uiCamera = Get_UICamera()->Get_Camera();
		Camera::S_View = uiCamera->Get_ViewMat();
		Camera::S_Proj = uiCamera->Get_ProjMat();

		uiCamera->Render_Forward();
	}
}
