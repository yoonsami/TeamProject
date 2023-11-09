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
#include "MathUtils.h"
#include "FontRenderer.h"

namespace fs = std::filesystem;
Scene::Scene()
{

}

Scene::~Scene()
{
}

void Scene::Init()
{
	SSAO_MakeOffsetVector();

	SSAO_MakeFrustumFarCorners();

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
	Render_MotionBlur();
	Render_Deferred();
	Render_DefferedBlur();
	if(GAMEINSTANCE.g_SSAOData.g_bSSAO_On)
	{
		Render_SSAO();
		Render_SSAOBlur(3);
	}
	Render_Lights();
	//Render_BlurEffect();
	Render_LightFinal();
	Render_MotionBlurFinal();
	Render_Forward();
	Render_BloomMap();
	Render_BloomMapScaling();
	Render_OutLine();
	Render_Distortion();
	Render_Distortion_Final();
	Render_LensFlare();
	Render_FXAA();
	Render_Aberration();

	Render_Debug();

	Render_UI();
	//Render_ToneMapping();

	Render_BackBuffer();
}

HRESULT Scene::Load_Scene()
{
	return S_OK;
}

void Scene::Add_GameObject(shared_ptr<GameObject> object, _bool staticFlag)
{
	m_GameObjects.push_back(object);

	if (object->Get_Camera())
		m_Cameras.push_back(object);

	if (object->Get_Light())
		m_Lights.push_back(object);

	if (object->Get_LayerIndex() == Layer_UI)
		m_UI.push_back(object);

	if (staticFlag)
		m_StaticObject.push_back(object);
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
	{
		auto findit = find(m_StaticObject.begin(), m_StaticObject.end(), object);
		if (findit != m_StaticObject.end())
			m_StaticObject.erase(findit);
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

_bool Scene::Is_Static(shared_ptr<GameObject> obj)
{
	for (auto& staticObj : m_StaticObject)
	{
		if (staticObj == obj)
			return true;
	}
	return false;
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
	//wstring finalPath = m_strSceneDataPath + sceneName + L".Scene";

	//shared_ptr<FileUtils> file = make_shared<FileUtils>();
	//file->Open(finalPath, FileMode::Read);


	//Set_Name(Utils::ToWString(file->Read<string>()));

	//auto& objects = Get_Objects();
	//_uint count = file->Read<_uint>();
	//


	//for (_uint i = 0; i < count; ++i)
	//{
	//	shared_ptr<GameObject> obj = make_shared<GameObject>();
	//	obj->Set_Name(Utils::ToWString(file->Read<string>()));

	//	obj->Set_LayerIndex(file->Read<_uchar>());
	//	obj->Get_CullPos() = file->Read<_float3>();
	//	obj->Set_CullRadius(file->Read<_float>());
	//	obj->Set_FrustumCulled(file->Read<_bool>());
	//	obj->Set_DrawShadow(file->Read<_bool>());
	//	//Transform
	//	if (file->Read<_bool>())
	//	{
	//		obj->GetOrAddTransform()->Set_WorldMat(file->Read<_float4x4>());
	//		obj->GetOrAddTransform()->Set_Speed(file->Read<_float>());
	//	}
	//	//MeshRenderer
	//	if (file->Read<_bool>())
	//	{
	//		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx"));
	//		shared_ptr<Mesh> mesh = RESOURCES.Get<Mesh>(Utils::ToWString(file->Read<string>()));

	//		renderer->Set_Mesh(mesh);

	//		shared_ptr<Material> material = make_shared<Material>();

	//		if (file->Read<_bool>())
	//		{
	//			wstring path = Utils::ToWString(file->Read<string>());
	//			wstring key = fs::path(path).filename();
	//			Utils::DetachExt(key);
	//			material->Set_TextureMap(RESOURCES.GetOrAddTexture(key, path),TextureMapType::DIFFUSE);
	//		}
	//		if (file->Read<_bool>())
	//		{
	//			wstring path = Utils::ToWString(file->Read<string>());
	//			wstring key = fs::path(path).filename();
	//			Utils::DetachExt(key);
	//			material->Set_TextureMap(RESOURCES.GetOrAddTexture(key, path), TextureMapType::NORMAL);
	//		}
	//		if (file->Read<_bool>())
	//		{
	//			wstring path = Utils::ToWString(file->Read<string>());
	//			wstring key = fs::path(path).filename();
	//			Utils::DetachExt(key);
	//			material->Set_TextureMap(RESOURCES.GetOrAddTexture(key, path), TextureMapType::SPECULAR);
	//		}

	//		material->Get_MaterialDesc() = file->Read<MaterialDesc>();

	//		renderer->Set_Material(material);

	//		obj->Add_Component(renderer);

	//	}
	//	//ModelRenderer
	//	if (file->Read<_bool>())
	//	{
	//		shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(RESOURCES.Get<Shader>(L"Shader_Model.fx"));
	//		shared_ptr<Model> model = RESOURCES.Get<Model>(Utils::ToWString(file->Read<string>()));
	//		assert(model != nullptr);
	//		renderer->Set_Model(model);

	//		renderer->Set_CullMode(file->Read<_bool>());

	//		obj->Add_Component(renderer);
	//	}
	//	//Camera
	//	if (file->Read<_bool>())
	//	{
	//		shared_ptr<Camera> camera = make_shared<Camera>();
	//		CameraDesc desc;

	//		desc.eType = file->Read<ProjectionType>();
	//		desc.strName = Utils::ToWString(file->Read<string>());
	//		desc.fFOV = file->Read<_float>();
	//		desc.fNear = file->Read<_float>();
	//		desc.fFar = file->Read<_float>();
	//		camera->Get_CameraDesc() = desc;
	//		obj->Add_Component(camera);
	//	}
	//	//Animator
	//	if (file->Read<_bool>())
	//	{
	//		wstring modelTag = Utils::ToWString(file->Read<string>());

	//		shared_ptr<ModelAnimator> renderer = make_shared<ModelAnimator>(RESOURCES.Get<Shader>(L"Shader_Model.fx"));
	//		shared_ptr<Model> model = RESOURCES.Get<Model>(Utils::ToWString(file->Read<string>()));
	//		assert(model != nullptr);
	//		renderer->Set_Model(model);

	//		obj->Add_Component(renderer);
	//	}
	//	//Light
	//	if (file->Read<_bool>())
	//	{
	//		shared_ptr<Light> light = make_shared<Light>();
	//		auto lightInfo = file->Read<LightInfo>();
	//		lightInfo.color.specular = _float4(0.f);
	//		light->Set_Color(lightInfo.color);
	//		light->Set_LightRange(lightInfo.range);
	//		light->Set_LightType(static_cast<LIGHT_TYPE>(lightInfo.lightType));
	//		light->Set_LightAngle(lightInfo.angle);
	//		

	//		obj->Add_Component(light);
	//	}
	//	//collider
	//	if (file->Read<_bool>())
	//	{
	//		ColliderType eType = file->Read<ColliderType>();
	//		_float3 offset = file->Read<_float3>();
	//		switch (eType)
	//		{
	//		case ColliderType::Sphere:
	//		{
	//			shared_ptr<SphereCollider> collider = make_shared<SphereCollider>(file->Read<_float>());
	//			obj->Add_Component(collider);
	//		}
	//		break;
	//		case ColliderType::AABB:
	//		{
	//			shared_ptr<AABBBoxCollider> collider = make_shared<AABBBoxCollider>(file->Read<_float3>());
	//			obj->Add_Component(collider);
	//		}
	//		break;
	//		case ColliderType::OBB:
	//		{
	//			_float3 extend = file->Read<_float3>();
	//			shared_ptr<OBBBoxCollider> collider = make_shared<OBBBoxCollider>(extend);
	//			obj->Add_Component(collider);
	//			_float3 colliderCenter = file->Read<_float3>();
	//			if (file->Read<_bool>())
	//			{
	//				_float3 vObjPos = obj->Get_Transform()->Get_State(Transform_State::POS).xyz();
	//				auto rigidBody = make_shared<RigidBody>();
	//				obj->Add_Component(rigidBody);
	//				rigidBody->Create_CapsuleRigidBody(colliderCenter, extend.x, (colliderCenter - vObjPos).y * 2.f);
	//			}
	//		}
	//		break;
	//		case ColliderType::Mesh:
	//		{
	//			shared_ptr<MeshCollider> collider = make_shared<MeshCollider>(Utils::ToWString(file->Read<string>()), obj->GetOrAddTransform()->Get_WorldMatrix());
	//			obj->Add_Component(collider);
	//			auto rigidBody = make_shared<RigidBody>();
	//			rigidBody->Create_RigidBody(collider);
	//			obj->Add_Component(rigidBody);
	//		}
	//		break;
	//		default:
	//			assert(false);
	//			break;
	//		}
	//		obj->Get_Collider()->Set_Offset(offset);
	//	}

	//	if (obj->Get_Name() == L"ToolCamera")
	//		continue;
	//	m_fLoadPercent = clamp(m_fLoadPercent += count / 50.f, 0.f, 100.f);
	//	Add_GameObject(obj);
	//}

}

void Scene::Load_UIFile(const wstring& strDataFilePath)
{
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(strDataFilePath, FileMode::Read);

	_uint iSize = file->Read<_uint>();
	for (_uint i = 0; i < iSize; ++i)
	{
		auto UiObject = make_shared<GameObject>();

		wstring strObjectName = Utils::ToWString(file->Read<string>());
		UiObject->Set_Name(strObjectName);

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx"));
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");
		renderer->Set_Mesh(mesh);

		auto material = make_shared<Material>();
		for (_uint i = 0; i < MAX_TEXTURE_MAP_COUONT; ++i)
		{
			_bool bIsUseTexture = file->Read<_bool>();

			if (true == bIsUseTexture)
				material->Set_TextureMap(RESOURCES.Get<Texture>(Utils::ToWString(file->Read<string>())), static_cast<TextureMapType>(i));
		}

		for (_uint i = 0; i < MAX_SUB_SRV_COUNT; ++i)
		{
			_bool bIsUseSubmap = file->Read<_bool>();

			if (true == bIsUseSubmap)
				material->Set_SubMap(i, RESOURCES.Get<Texture>(Utils::ToWString(file->Read<string>())));
		}
		renderer->Set_Material(material);

		_float4x4 matWorld = file->Read<_float4x4>();
		UiObject->GetOrAddTransform()->Set_WorldMat(matWorld);

		_uint iPass = file->Read<_uint>();
		renderer->Set_Pass(iPass);

		RenderParams tagParam = file->Read<RenderParams>();
		renderer->Get_RenderParamDesc() = tagParam;
		UiObject->Add_Component(renderer);

		_bool bIsUseBaseUi = file->Read<_bool>();
		if (true == bIsUseBaseUi)
		{
			auto BaseUi = make_shared<BaseUI>();
			BaseUI::BASEUIDESC tagBaseDesc = file->Read<BaseUI::BASEUIDESC>();
			BaseUi->Get_Desc() = tagBaseDesc;
			UiObject->Add_Component(BaseUi);
		}

		_bool bIsUseFont = file->Read<_bool>();
		if (true == bIsUseFont)
		{
			wstring wstrText = Utils::ToWstringUtf8(file->Read<string>());
			auto pFontRenderer = make_shared<FontRenderer>(wstrText);

			wstring strFont = Utils::ToWString(file->Read<string>());
			Color vecColor = file->Read<Color>();
			_float fSize = file->Read<_float>();
			pFontRenderer->Set_Font(RESOURCES.Get<CustomFont>(strFont), vecColor, fSize);

			UiObject->Add_Component(pFontRenderer);
		}

		UiObject->Set_LayerIndex(Layer_UI);
		UiObject->Set_Instancing(false);

		_bool bIsStatic = file->Read<_bool>();
		Add_GameObject(UiObject, bIsStatic);
	}
}

void Scene::Load_MapFile(const wstring& _mapFileName)
{
	// 세이브 파일 이름으로 로드하기
	wstring strFilePath = L"..\\Resources\\Data\\";
	strFilePath += _mapFileName + L".dat";
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(strFilePath, FileMode::Read);

	// 스카이박스 텍스쳐 추가하기
	{
		wstring path = L"..\\Resources\\Textures\\MapObject\\SkyBox\\";

		for (auto& entry : fs::recursive_directory_iterator(path))
		{
			// 파일의 이름을 가져옴
			wstring fileName = entry.path().filename().wstring();
			WCHAR szTempName[MAX_PATH];
			lstrcpy(szTempName, fileName.c_str());

			wstring noExtName = szTempName;
			Utils::DetachExt(noExtName);
			RESOURCES.Load<Texture>(noExtName, path + szTempName);
		}
	}

	// 스카이박스 텍스쳐 이름 가져와서 적용하기
	wstring strSkyBoxTextureName = Utils::ToWString(file->Read<string>());
	vector<shared_ptr<Material>>& Mats = Get_GameObject(L"SkyBase")->Get_ModelRenderer()->Get_Model()->Get_Materials();
	Mats[0]->Set_TextureMap(RESOURCES.Get<Texture>(strSkyBoxTextureName), TextureMapType::DIFFUSE);

	// 광원 정보 가져와서 방향성광원 적용 및 점광원 생성하기
	// 방향성광원
	 // 포지션
	shared_ptr<GameObject> DirectionalLightObject = Get_Light();
	_float4 DirLightPos = _float4{ 0.f, 0.f, 0.f, 1.f };
	file->Read<_float4>(DirLightPos);
	DirectionalLightObject->Get_Transform()->Set_State(Transform_State::POS, DirLightPos);
	// 보는방향
	_float3 DirLightLookDir = _float3{ 0.f, 0.f, 0.f };
	file->Read<_float3>(DirLightLookDir);
	DirectionalLightObject->Get_Transform()->Set_LookDir(DirLightLookDir);
	// 색깔
	LightColor DirLightColor;
	file->Read<LightColor>(DirLightColor);
	DirectionalLightObject->Get_Light()->Set_Ambient(DirLightColor.ambient);
	DirectionalLightObject->Get_Light()->Set_Diffuse(DirLightColor.diffuse);
	DirectionalLightObject->Get_Light()->Set_Specular(DirLightColor.specular);
	DirectionalLightObject->Get_Light()->Set_Emissive(DirLightColor.emissive);

	// 점광원정보 가져오고 불러오기
	_int iNumPointLight = file->Read<_int>();
	for (_int i = 0; i < iNumPointLight; ++i)
	{
		LightInfo loadPointLightInfo;
		file->Read<_float4>(loadPointLightInfo.vPosition);
		file->Read<LightColor>(loadPointLightInfo.color);
		file->Read<_float>(loadPointLightInfo.range);

		shared_ptr<GameObject> PointLight = make_shared<GameObject>();
		PointLight->Set_Name(L"PointLight");
		PointLight->GetOrAddTransform()->Set_State(Transform_State::POS, loadPointLightInfo.vPosition);
		{
			// LightComponent 생성 후 세팅
			shared_ptr<Light> lightCom = make_shared<Light>();
			lightCom->Set_Ambient(loadPointLightInfo.color.ambient);
			lightCom->Set_Diffuse(loadPointLightInfo.color.diffuse);
			lightCom->Set_Specular(loadPointLightInfo.color.specular);
			lightCom->Set_Emissive(loadPointLightInfo.color.emissive);
			lightCom->Set_LightType(LIGHT_TYPE::POINT_LIGHT);
			lightCom->Set_LightRange(loadPointLightInfo.range);

			PointLight->Add_Component(lightCom);
		}
		Add_GameObject(PointLight);
	}

	// 오브젝트 개수 불러오기
	_int iNumObjects = file->Read<_int>();

	for (_int i = 0; i < iNumObjects; ++i)
	{
		// 맵오브젝트에 필요한 모든정보
		string strName = "";
		_float fUVWeight = { 1.f }; // UV비율 : 기본1배
		_bool bShadow = false;
		_bool bBlur = false;
		// Component
		   // Transform
		_bool bTransform = true;
		_float4x4 WorldMatrix = XMMatrixIdentity();
		// Collider
		_bool bCollider = false;
		//ColliderType ColliderType = ColliderType::OBB;
		// 0:Sphere 1:AABB 2:OBB 3:Mesh
		_int iColliderType = static_cast<_int>(ColliderType::OBB);
		// ColliderDesc
		_float3 ColliderOffset = _float3{ 0.f, 0.f, 0.f };
		_float ColRadius = { 0.f };
		_float3 ColBoundingSize = _float3{ 0.f, 0.f, 0.f };
		string ColModelName = "";
		// Culling
		_float3 CullPos = _float3{ 0.f, 0.f, 0.f };
		_float CullRadius = { 0.f };

		wstring strObjectName = Utils::ToWString(file->Read<string>());
		strName = Utils::ToString(strObjectName);

		file->Read<_float>(fUVWeight);
		file->Read<_bool>(bShadow);
		file->Read<_bool>(bBlur);
		file->Read<_bool>(bTransform);
		if (bTransform)
			file->Read<_float4x4>(WorldMatrix);
		file->Read<_bool>(bCollider);
		if (bCollider)
		{
			file->Read<_int>(iColliderType);
			file->Read<_float3>(ColliderOffset);
			switch (static_cast<ColliderType>(iColliderType))
			{
			case ColliderType::Sphere:
				file->Read<_float>(ColRadius);
				break;
			case ColliderType::AABB:
				file->Read<_float3>(ColBoundingSize);
				break;
			case ColliderType::OBB:
				file->Read<_float3>(ColBoundingSize);
				break;
			case ColliderType::Mesh:
				//file->Read<string>(ColModelName);
				break;
			default:
				break;
			}
		}
		file->Read<_float3>(CullPos);
		file->Read<_float>(CullRadius);

		// 오브젝트 틀 생성
		shared_ptr<GameObject> CreateObject = make_shared<GameObject>();
		CreateObject->Set_Name(strObjectName);
		CreateObject->Set_DrawShadow(bShadow);
		CreateObject->Set_Blur(bBlur);
		CreateObject->Set_CullPos(CullPos);
		CreateObject->Set_CullRadius(CullRadius);
		// 이름을 사용하여 모델생성
		// 고유번호를 제거하여 모델명을 얻어옴
		_int iPureNameSize = 0;
		while (strName[iPureNameSize] != '-' && iPureNameSize < strName.size())
		{
			++iPureNameSize;
		}
		string strModelName = strName.substr(0, iPureNameSize);
		// 콜라이더에서 사용하는 모델명도 저장
		ColModelName = strModelName;
		// 모델생성
		shared_ptr<Model> model = RESOURCES.Get<Model>(Utils::ToWString(strModelName));
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
		shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
		CreateObject->Add_Component(renderer);
		renderer->Set_Model(model);
		renderer->Set_PassType(ModelRenderer::PASS_MAPOBJECT);
		renderer->SetFloat(3, fUVWeight);
		// 트랜스폼 생성
		if (bTransform)
		{
			CreateObject->Add_Component(make_shared<Transform>());
			CreateObject->Get_Transform()->Set_WorldMat(WorldMatrix);
		}
		// 콜라이더 생성
		if (bCollider)
		{
			switch (static_cast<ColliderType>(iColliderType))
			{
			case ColliderType::Sphere:
			{
				shared_ptr<SphereCollider> pCollider = make_shared<SphereCollider>(ColRadius);
				pCollider->Set_Offset(ColliderOffset);
				CreateObject->Add_Component(pCollider);
				//pCollider->Set_CollisionGroup(MAPObject);
				pCollider->Set_Activate(true);
				break;
			}
			case ColliderType::AABB:
			{
				shared_ptr<AABBBoxCollider> pCollider = make_shared<AABBBoxCollider>(ColBoundingSize);
				pCollider->Set_Offset(ColliderOffset);
				CreateObject->Add_Component(pCollider);
				//pCollider->Set_CollisionGroup(MAPObject);
				pCollider->Set_Activate(true);
				break;
			}
			case ColliderType::OBB:
			{
				shared_ptr<OBBBoxCollider> pCollider = make_shared<OBBBoxCollider>(ColBoundingSize);
				pCollider->Set_Offset(ColliderOffset);
				CreateObject->Add_Component(pCollider);
				//pCollider->Set_CollisionGroup(MAPObject);
				pCollider->Set_Activate(true);
				_float3 vObjPos = CreateObject->Get_Transform()->Get_State(Transform_State::POS).xyz();
				auto rigidBody = make_shared<RigidBody>();
				CreateObject->Add_Component(rigidBody);
				rigidBody->Create_CapsuleRigidBody(CreateObject->Get_CullPos() + _float3::Up * CreateObject->Get_CullRadius(), CreateObject->Get_CullRadius(), CreateObject->Get_CullRadius() * 2.f);

				break;
			}
			case ColliderType::Mesh:
			{
				shared_ptr<MeshCollider> pCollider = make_shared<MeshCollider>(Utils::ToWString(ColModelName));
				pCollider->Set_Offset(ColliderOffset);
				CreateObject->Add_Component(pCollider);
				//pCollider->Set_CollisionGroup(MAPObject);
				pCollider->Set_Activate(true);
				auto rigidBody = make_shared<RigidBody>();
				rigidBody->Create_RigidBody(pCollider, CreateObject->GetOrAddTransform()->Get_WorldMatrix());
				CreateObject->Add_Component(rigidBody);
				break;
			}
			default:
				break;
			}
		}
		

		Add_GameObject(CreateObject);
	}

}

void Scene::PickUI()
{
	if (!KEYTAP(KEY_TYPE::LBUTTON))
		return;

	if (!Get_UICamera())
		return;

	POINT screenPt = INPUT.GetMousePosToPoint();

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

void Scene::Render_MotionBlur()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::MOTIONBLUR)->OMSetRenderTargets();

	if (Get_MainCamera())
	{
		shared_ptr<Camera> mainCamera = Get_MainCamera()->Get_Camera();
		mainCamera->Render_MotionBlur();
	}
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

void Scene::Render_DefferedBlur()
{
	/*float a[25] = {
	1.0f / 273.0f,  4.0f / 273.0f,  7.0f / 273.0f,  4.0f / 273.0f,  1.0f / 273.0f,
	4.0f / 273.0f,  16.0f / 273.0f, 26.0f / 273.0f, 16.0f / 273.0f, 4.0f / 273.0f,
	7.0f / 273.0f,  26.0f / 273.0f, 41.0f / 273.0f, 26.0f / 273.0f, 7.0f / 273.0f,
	4.0f / 273.0f,  16.0f / 273.0f, 26.0f / 273.0f, 16.0f / 273.0f, 4.0f / 273.0f,
	1.0f / 273.0f,  4.0f / 273.0f,  7.0f / 273.0f,  4.0f / 273.0f,  1.0f / 273.0f
	};*/
	for (_uchar i = 0; i < 2; ++i)
	{
		RENDER_TARGET_GROUP_TYPE eType = static_cast<RENDER_TARGET_GROUP_TYPE>(static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURSMALLER0) + i);
		GRAPHICS.Get_RTGroup(eType)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BlurSmaller" + to_wstring(i));
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");
		//material->Get_Shader()->GetScalar("GaussianWeight")->SetFloatArray(a, 0, 25);
		//material->Get_Shader()->GetScalar("DownScalePower")->SetFloat(m_fDownScalePower);
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
	//	material->Get_Shader()->GetScalar("UpScalePower")->SetFloat(m_fUpScalePower);

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

void Scene::Render_SSAO()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::SSAO)->OMSetRenderTargets();

	auto material = RESOURCES.Get<Material>(L"SSAO");
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");

	material->Push_SubMapData();
	static const _float4x4 T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	_float4x4 matViewToTexSpace =  Camera::Get_Proj() * T;
	material->Get_Shader()->GetMatrix("gViewToTexSpace")->SetMatrix((_float*)&matViewToTexSpace);
	material->Get_Shader()->GetVector("gOffsetVectors")->SetFloatVectorArray((_float*)m_vOffsets, 0, 14);
	material->Get_Shader()->GetVector("gFrustumCorners")->SetFloatVectorArray((_float*)m_vFrustumFarCorner, 0, 4);
	material->Get_Shader()->GetScalar("gOcclusionRadius")->SetFloat(GAMEINSTANCE.g_SSAOData.g_fOcclusionRadius);
	material->Get_Shader()->GetScalar("gOcclusionFadeStart")->SetFloat(GAMEINSTANCE.g_SSAOData.g_OcclusionFadeStart);
	material->Get_Shader()->GetScalar("gOcclusionFadeEnd")->SetFloat(GAMEINSTANCE.g_SSAOData.g_OcclusionFadeEnd);


	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);


}

void Scene::Render_SSAOBlur(_uint blurCount)
{
	for (_uchar i = 0; i < 3; ++i)
	{
		RENDER_TARGET_GROUP_TYPE eType = static_cast<RENDER_TARGET_GROUP_TYPE>(static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::SSAODOWNSCALE0) + i);
		GRAPHICS.Get_RTGroup(eType)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"SSAODownScale" + to_wstring(i));
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");
		//material->Get_Shader()->GetScalar("GaussianWeight")->SetFloatArray(a, 0, 25);
		//material->Get_Shader()->GetScalar("DownScalePower")->SetFloat(m_fDownScalePower);
		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 2, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}

	for (_uchar i = 0; i < 3; ++i)
	{
		RENDER_TARGET_GROUP_TYPE eType = static_cast<RENDER_TARGET_GROUP_TYPE>(static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::SSAOUPSCALE0) + i);
		GRAPHICS.Get_RTGroup(eType)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"SSAOUpScale" + to_wstring(i));
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");
		//	material->Get_Shader()->GetScalar("UpScalePower")->SetFloat(m_fUpScalePower);

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (i != 2)
			material->Get_Shader()->DrawIndexed(0, 2, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
		else
			material->Get_Shader()->DrawIndexed(0, 1, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);

	}
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

void Scene::Render_LightFinal()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::FINAL)->OMSetRenderTargets();

	auto material = RESOURCES.Get<Material>(L"LightFinal");
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");

	material->Get_Shader()->GetScalar("g_gamma")->SetFloat(GAMEINSTANCE.g_fGamma);
	material->Get_Shader()->GetScalar("g_SSAO_On")->SetBool(GAMEINSTANCE.g_SSAOData.g_bSSAO_On);
	material->Get_Shader()->GetScalar("g_FogOn")->SetBool(GAMEINSTANCE.g_FogData.g_FogOn);
	material->Get_Shader()->GetScalar("gFogStart")->SetFloat(GAMEINSTANCE.g_FogData.gFogStart);
	material->Get_Shader()->GetScalar("gFogRange")->SetFloat(GAMEINSTANCE.g_FogData.gFogRange);
	material->Get_Shader()->GetVector("gFogColor")->SetFloatVector((_float*)&GAMEINSTANCE.g_FogData.gFogColor);



	material->Push_SubMapData();

	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	material->Get_Shader()->DrawIndexed(0,3, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);

	m_wstrFinalRenderTarget = L"FinalTarget";
}

void Scene::Render_OutLine()
{
	if (!GAMEINSTANCE.g_bDrawOutline)
		return;

	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::OUTLINE)->OMSetRenderTargets();

		auto material = RESOURCES.Get<Material>(L"Outline");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}
	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::OUTLINEFINAL)->OMSetRenderTargets();

		auto material = RESOURCES.Get<Material>(L"OutlineFinal");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");

		material->Push_SubMapData();
		material->Set_SubMap(1, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));
		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 1, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);

		m_wstrFinalRenderTarget = L"OutLineFinalTarget";
	}
}

void Scene::Render_MotionBlurFinal()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::MOTIONBLURFINAL)->OMSetRenderTargets();

	auto material = RESOURCES.Get<Material>(L"MotionBlurFinal");
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	material->Set_SubMap(1, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));
	material->Get_Shader()->GetScalar("g_BlurCount")->SetInt(50);

	material->Push_SubMapData();

	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	material->Get_Shader()->DrawIndexed(1, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	m_wstrFinalRenderTarget = L"MotionBlurFinalTarget";
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

void Scene::Render_BloomMap()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLOOMMAP)->OMSetRenderTargets();

	auto material = RESOURCES.Get<Material>(L"BloomTarget");
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	material->Set_SubMap(0, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));
	material->Push_SubMapData();
	material->Get_Shader()->GetScalar("g_BloomMin")->SetFloat(GAMEINSTANCE.g_fBloomMin);
	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
}

void Scene::Render_BloomMapScaling()
{
	for (_uchar i = 0; i < 3; ++i)
	{
		RENDER_TARGET_GROUP_TYPE eType = static_cast<RENDER_TARGET_GROUP_TYPE>(static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLOOMDOWNSCALE0) + i);
		GRAPHICS.Get_RTGroup(eType)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BloomDownScale" + to_wstring(i));
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");
		//material->Get_Shader()->GetScalar("GaussianWeight")->SetFloatArray(a, 0, 25);
		//material->Get_Shader()->GetScalar("DownScalePower")->SetFloat(m_fDownScalePower);
		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 2, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	}

	for (_uchar i = 0; i < 3; ++i)
	{
		RENDER_TARGET_GROUP_TYPE eType = static_cast<RENDER_TARGET_GROUP_TYPE>(static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLOOMUPSCALE0) + i);
		GRAPHICS.Get_RTGroup(eType)->OMSetRenderTargets();
		auto material = RESOURCES.Get<Material>(L"BloomUpScale" + to_wstring(i));
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");
		//	material->Get_Shader()->GetScalar("UpScalePower")->SetFloat(m_fUpScalePower);

		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		if(i != 2)
			material->Get_Shader()->DrawIndexed(0, 2, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
		else
			material->Get_Shader()->DrawIndexed(0, 1, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);

	}

	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::BLOOMFINAL)->OMSetRenderTargets();
		
		auto material = RESOURCES.Get<Material>(L"BloomFinal");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");
		//	material->Get_Shader()->GetScalar("UpScalePower")->SetFloat(m_fUpScalePower);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));
		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 1, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);

		m_wstrFinalRenderTarget = L"BloomFinalTarget";
	}


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
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::DISTORTIONFINAL)->OMSetRenderTargets();
	
	auto material = RESOURCES.Get<Material>(L"Distorted_Final");
	material->Set_SubMap(0, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	
	material->Push_SubMapData();
	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();
	
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	m_wstrFinalRenderTarget = L"DistortionFinalTarget";
}

void Scene::Render_FXAA()
{
	if (!GAMEINSTANCE.g_bFXAAOn)
		return;
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::FXAA)->OMSetRenderTargets();
	
	auto material = RESOURCES.Get<Material>(L"FXAARenderFinal");
	material->Set_SubMap(0, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	
	material->Get_Shader()->GetScalar("Use")->SetBool(GAMEINSTANCE.g_bFXAAOn);
	material->Push_SubMapData();
	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	m_wstrFinalRenderTarget = L"FXAATarget";
}

void Scene::Render_LensFlare()
{
	if (!GAMEINSTANCE.g_bLensFlare)
		return;

	if (!CUR_SCENE->Get_MainCamera())
		return;

	if (!CUR_SCENE->Get_Light())
		return;

	_float3 viewLightPos = _float3::Transform(m_LightParams.lights[0].vPosition.xyz(), Camera::Get_View());
	_float3 projLightPos = _float3::Transform(viewLightPos, Camera::Get_Proj());
	//[-1,1] -> [0, 1920] //[1,-1] -> [0, 1080]
	_float2 proj = { projLightPos.x,projLightPos.y };
	if (!(projLightPos.z <=1.f))
		return;
	proj.x = (proj.x + 1.f) * GRAPHICS.Get_ViewPort().Get_Width() * 0.5f;
	proj.y = (1.f - proj.y) * GRAPHICS.Get_ViewPort().Get_Height() * 0.5f;


	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::LENSFLARE)->OMSetRenderTargets();

	auto material = RESOURCES.Get<Material>(L"LensFlare");
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	material->Set_SubMap(0, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));



	material->Get_Shader()->GetVector("g_LightPos")->SetFloatVector((_float*)&proj);
	material->Get_Shader()->GetVector("col1")->SetFloatVector((_float*)&GAMEINSTANCE.g_testVec1);
	material->Get_Shader()->GetVector("col2")->SetFloatVector((_float*)&GAMEINSTANCE.g_testVec2);
	material->Push_SubMapData();
	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();



	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	material->Get_Shader()->DrawIndexed(0, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);

	//for (_uchar i = 0; i < 3; ++i)
	//{
	//	RENDER_TARGET_GROUP_TYPE eType = static_cast<RENDER_TARGET_GROUP_TYPE>(static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::LENSFLAREDOWNSCALE0) + i);
	//	GRAPHICS.Get_RTGroup(eType)->OMSetRenderTargets();
	//	auto material = RESOURCES.Get<Material>(L"LensFlareDownScale" + to_wstring(i));
	//	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	//	//material->Get_Shader()->GetScalar("GaussianWeight")->SetFloatArray(a, 0, 25);
	//	//material->Get_Shader()->GetScalar("DownScalePower")->SetFloat(m_fDownScalePower);
	//	material->Push_SubMapData();

	//	mesh->Get_VertexBuffer()->Push_Data();
	//	mesh->Get_IndexBuffer()->Push_Data();

	//	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	material->Get_Shader()->DrawIndexed(0, 2, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	//}

	//for (_uchar i = 0; i < 3; ++i)
	//{
	//	RENDER_TARGET_GROUP_TYPE eType = static_cast<RENDER_TARGET_GROUP_TYPE>(static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::LENSFLAREUPSCALE0) + i);
	//	GRAPHICS.Get_RTGroup(eType)->OMSetRenderTargets();
	//	auto material = RESOURCES.Get<Material>(L"LensFlareUpScale" + to_wstring(i));
	//	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	//	//	material->Get_Shader()->GetScalar("UpScalePower")->SetFloat(m_fUpScalePower);

	//	material->Push_SubMapData();

	//	mesh->Get_VertexBuffer()->Push_Data();
	//	mesh->Get_IndexBuffer()->Push_Data();

	//	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	if (i != 2)
	//		material->Get_Shader()->DrawIndexed(0, 2, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	//	else
	//		material->Get_Shader()->DrawIndexed(0, 1, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	//}
	{
		GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::LENSFLAREFINAL)->OMSetRenderTargets();

		auto material = RESOURCES.Get<Material>(L"LensFlareFinal");
		auto mesh = RESOURCES.Get<Mesh>(L"Quad");
		//	material->Get_Shader()->GetScalar("UpScalePower")->SetFloat(m_fUpScalePower);
		material->Set_SubMap(0, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));
		material->Push_SubMapData();

		mesh->Get_VertexBuffer()->Push_Data();
		mesh->Get_IndexBuffer()->Push_Data();

		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		material->Get_Shader()->DrawIndexed(0, 1, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);

		m_wstrFinalRenderTarget = L"LensFlareFinalTarget";
	}

}

void Scene::Render_Aberration()
{
	if (!GAMEINSTANCE.g_bAberrationOn)
		return;



	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::ABERRATION)->OMSetRenderTargets();

	auto material = RESOURCES.Get<Material>(L"AberrationFinal");
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");
	material->Set_SubMap(0, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));

	material->Get_Shader()->GetScalar("g_AberrationPower")->SetFloat(GAMEINSTANCE.g_fAberrationPower);

	material->Push_SubMapData();
	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	material->Get_Shader()->DrawIndexed(1, 0, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);
	m_wstrFinalRenderTarget = L"AberrationTarget";
}

void Scene::Render_Debug()
{
	if (m_bRenderDebug)
	{
		for (auto& gameObject : m_GameObjects)
			if (gameObject->Get_Collider() && gameObject->Get_Collider()->Get_ColliderType() != ColliderType::Mesh)
				gameObject->Get_Collider()->Render();
	}
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

void Scene::Render_ToneMapping()
{
	GRAPHICS.Get_RTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets();

	auto material = RESOURCES.Get<Material>(L"BackBufferRenderFinal");
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");

	material->Set_SubMap(0, RESOURCES.Get<Texture>(m_wstrFinalRenderTarget));



	material->Get_Shader()->GetScalar("g_brightness")->SetFloat(GAMEINSTANCE.g_fBrightness);
	material->Get_Shader()->GetScalar("g_contrast")->SetFloat(GAMEINSTANCE.g_fContrast);
	material->Get_Shader()->GetScalar("g_max_white")->SetFloat(GAMEINSTANCE.g_fMaxWhite);
	material->Get_Shader()->GetScalar("g_gamma")->SetFloat(GAMEINSTANCE.g_fGamma);


	material->Push_SubMapData();
	mesh->Get_VertexBuffer()->Push_Data();
	mesh->Get_IndexBuffer()->Push_Data();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (GAMEINSTANCE.g_iTMIndex > 3) GAMEINSTANCE.g_iTMIndex = 3;

	material->Get_Shader()->DrawIndexed(0, GAMEINSTANCE.g_iTMIndex, mesh->Get_IndexBuffer()->Get_IndicesNum(), 0, 0);


}

void Scene::Render_BackBuffer()
{

}

void Scene::SSAO_MakeOffsetVector()
{
	m_vOffsets[0] = _float4(+1.0f, +1.0f, +1.0f, 0.0f);
	m_vOffsets[1] = _float4(-1.0f, -1.0f, -1.0f, 0.0f);

	m_vOffsets[2] = _float4(-1.0f, +1.0f, +1.0f, 0.0f);
	m_vOffsets[3] = _float4(+1.0f, -1.0f, -1.0f, 0.0f);

	m_vOffsets[4] = _float4(+1.0f, +1.0f, -1.0f, 0.0f);
	m_vOffsets[5] = _float4(-1.0f, -1.0f, +1.0f, 0.0f);

	m_vOffsets[6] = _float4(-1.0f, +1.0f, -1.0f, 0.0f);
	m_vOffsets[7] = _float4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	m_vOffsets[8] = _float4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_vOffsets[9] = _float4(+1.0f, 0.0f, 0.0f, 0.0f);

	m_vOffsets[10] = _float4(0.0f, -1.0f, 0.0f, 0.0f);
	m_vOffsets[11] = _float4(0.0f, +1.0f, 0.0f, 0.0f);

	m_vOffsets[12] = _float4(0.0f, 0.0f, -1.0f, 0.0f);
	m_vOffsets[13] = _float4(0.0f, 0.0f, +1.0f, 0.0f);

	for (_int i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		_float s = MathUtils::Get_RandomFloat(0.25f, 1.0f);

		XMVECTOR v = s * ::XMVector4Normalize(::XMLoadFloat4(&m_vOffsets[i]));

		::XMStoreFloat4(&m_vOffsets[i], v);
	}
}

void Scene::SSAO_MakeFrustumFarCorners()
{
	_float aspect = (_float)GRAPHICS.Get_ViewPort().Get_Width() / (_float)GRAPHICS.Get_ViewPort().Get_Height();

	_float farZ = 1000.f;

	_float halfHeight = farZ * tanf(0.5f * XM_PI / 3.f);
	_float halfWidth = aspect * halfHeight;

	if (Get_MainCamera())
	{
		farZ = Get_MainCamera()->Get_Camera()->Get_CameraDesc().fFar;
		halfHeight = farZ * tanf(0.5f * Get_MainCamera()->Get_Camera()->Get_CameraDesc().fFOV);
		halfWidth = aspect * halfHeight;
	}

	m_vFrustumFarCorner[0] = _float4(-halfWidth, +halfHeight, farZ, 0.0f);
	m_vFrustumFarCorner[1] = _float4(+halfWidth, +halfHeight, farZ, 0.0f);
	m_vFrustumFarCorner[2] = _float4(+halfWidth, -halfHeight, farZ, 0.0f);
	m_vFrustumFarCorner[3] = _float4(-halfWidth, -halfHeight, farZ, 0.0f);
}
