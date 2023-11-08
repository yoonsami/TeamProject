﻿#include "pch.h"
#include "LogoScene.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"
#include "BaseCollider.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "MainCameraScript.h"
#include "WeaponScript.h"
#include "DemoCameraScript1.h"
#include "DemoCameraScript2.h"
#include "DemoAnimationController1.h"
#include "DemoFSM.h"
#include "FileUtils.h"
#include "Utils.h"
#include "SpearAce_FSM.h"
#include "MeshRenderer.h"
#include "BaseUI.h"
#include "FontRenderer.h"
#include "CustomFont.h"
#include "RigidBody.h"
#include "ObjectTransformDebug.h"
#include "CharacterController.h"
#include "HeroChangeScript.h"
#include "Silversword_Soldier_FSM.h"

#include "MeshCollider.h"
#include "OBBBoxCollider.h"
#include "AABBBoxCollider.h"
#include "SphereCollider.h"
#include "MapObjectScript.h"
#include "Loader.h"
#include "LoadingScene.h"
#include "ModelMesh.h"
#include "Geometry.h"
#include "DemoScene.h"
#include <filesystem>

namespace fs = std::filesystem;

LogoScene::LogoScene()
{
}

LogoScene::~LogoScene()
{
	_uint i = 0;
}

void LogoScene::Init()
{
	__super::Init();
}

void LogoScene::Tick()
{
	__super::Tick();
}

void LogoScene::Late_Tick()
{
	__super::Late_Tick();
}

void LogoScene::Final_Tick()
{
	__super::Final_Tick();
}

HRESULT LogoScene::Load_Scene()
{
	Load_Camera();

	Load_Ui();

	return S_OK;
}

void LogoScene::Load_Camera()
{
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();

		camera->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));

		CameraDesc desc;
		desc.fFOV = XM_PI / 3.f;
		desc.strName = L"UI_Cam";
		desc.fSizeX = _float(g_iWinSizeX);
		desc.fSizeY = _float(g_iWinSizeY);
		desc.fNear = 0.1f;
		desc.fFar = 1000.f;
		shared_ptr<Camera> cameraComponent = make_shared<Camera>(desc);

		camera->Add_Component(cameraComponent);

		camera->Get_Camera()->Set_ProjType(ProjectionType::Orthographic);
		camera->Get_Camera()->Set_CullingMaskAll();
		camera->Get_Camera()->Set_CullingMaskLayerOnOff(Layer_UI, false);

		Add_GameObject(camera);
	}


}

void LogoScene::Load_Ui()
{
	wstring assetPath = L"..\\Resources\\Textures\\UITexture\\Logo\\";

	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		wstring filePath = entry.path().wstring();
		wstring fileName = entry.path().filename().wstring();
		Utils::DetachExt(fileName);
		RESOURCES.Load<Texture>(fileName, filePath);
	}


	Load_UIFile(L"..\\Resources\\UIData\\UI_Logo.dat");

	auto pGameobject = Get_GameObject(L"UI_Start_Button");
	if (nullptr != pGameobject)
	{
		pGameobject->Get_Button()->AddOnClickedEvent([&]()
			{
				SCENE.Change_Scene(make_shared<LoadingScene>(make_shared<DemoScene>()));
			});
	}
}