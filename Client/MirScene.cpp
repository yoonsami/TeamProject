﻿#include "pch.h"
#include "MirScene.h"

#include "Utils.h"
#include "Model.h"
#include "Light.h"
#include "Loader.h"
#include "Camera.h"
#include "BaseUI.h"
#include "Geometry.h"
#include "RigidBody.h"
#include "FileUtils.h"
#include "ModelMesh.h"
#include "CustomFont.h"
#include "LoadingScene.h"
#include "BaseCollider.h"
#include "WeaponScript.h"
#include "Player_FSM.h"
#include "SpearAce_FSM.h"
#include "MeshRenderer.h"
#include "FontRenderer.h"
#include "MeshCollider.h"
#include "UiCharChange.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"
#include "OBBBoxCollider.h"
#include "SphereCollider.h"
#include "AABBBoxCollider.h"
#include "UiGachaCardMove.h"
#include "Debug_CreateMotionTrail.h"
#include "CounterMotionTrailScript.h"

#include "UiHpBarController.h"
#include "MapObjectScript.h"
#include "UiCoolEndEffect.h"
#include "MainCameraScript.h"
#include "HeroChangeScript.h"
#include "DemoCameraScript1.h"
#include "DemoCameraScript2.h"
#include "CoolTimeCheckScript.h"
#include "UiSkillButtonEffect.h"
#include "CharacterController.h"
#include "ObjectTransformDebug.h"
#include "Silversword_Soldier_FSM.h"
#include "Succubus_Scythe_FSM.h"
#include "Undead_Priest_FSM.h"
#include "Alpaca_FSM.h"
#include "Wolf_FSM.h"


#include "UiGachaController.h"
#include "Boss_Mir_FSM.h"
#include "Boss_Dellons_FSM.h"
#include "Boss_Spike_FSM.h"
#include "DemoAnimationController1.h"
#include "UiCardDeckController.h"
#include "MainUiController.h"
#include "UiCardDeckInvenChange.h"
#include "UiTargetLockOn.h"
#include "UiMonsterHp.h"
#include "UiDamageCreate.h"
#include "UIBossHpBar.h"
#include "UiComboEffect.h"
#include "UiSkillGauge.h"
#include "UiSettingController.h"
#include "UiQuestController.h"
#include "UiBossDialog.h"
#include "UIInteraction.h"

#include <filesystem>
#include "GachaScene.h"
#include "GranseedScene.h"
#include "KrisScene.h"
#include "UiSettingController.h"
#include "UiQuestController.h"
#include "UiBossDialog.h"
#include "UIInteraction.h"
namespace fs = std::filesystem;

MirScene::MirScene()
{
}

MirScene::~MirScene()
{
}

void MirScene::Init()
{
	__super::Init();
	for (_int i = 0; i < _int(HERO::MAX); ++i)
	{
		auto& tagData = GET_DATA(HERO(i));

		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, -200.f, 0.f, 1.f));
			shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
			animator->Set_Model(RESOURCES.Get<Model>(tagData.ModelTag));
			obj->Add_Component(animator);
			obj->Set_Instancing(false);
			Add_GameObject(obj);
		}

	}

	m_strSceneName = L"MirScene";
}

void MirScene::Tick()
{
	__super::Tick();

	//{
	//	if (KEYPUSH(KEY_TYPE::C))
	//	{
	//		Get_GameObject(L"Main_Ui_Controller")->Get_Script<MainUiController>()->Set_MainUI_Render(true);
	//	}
	//	if (KEYPUSH(KEY_TYPE::V))
	//	{
	//		Get_GameObject(L"Main_Ui_Controller")->Get_Script<MainUiController>()->Set_MainUI_Render(false);
	//	}
	//}
	
}

void MirScene::Late_Tick()
{

	__super::Late_Tick();

	// Test
	{
		if (KEYTAP(KEY_TYPE::X))
		{
			if (m_bRenderDebug)
				m_bRenderDebug = false;
			else
				m_bRenderDebug = true;
		}
	}
}

void MirScene::Final_Tick()
{
	__super::Final_Tick();
	PHYSX.Tick();
	
	if (KEYTAP(KEY_TYPE::TAB))
	{
		/*GachaSceneDesc sceneDesc{ L"YeopoMap",HERO::YEOPO};
			SCENE.Add_SubScene(make_shared<GachaScene>(sceneDesc));
		SCENE.Exchange_Scene();*/

		shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<KrisScene>());
		PHYSX.Set_CharacterControllerNull();

		scene->Set_StaticObjects(m_StaticObject);
		SCENE.Change_Scene(scene);
		g_bCutScene = false;

	}
}

HRESULT MirScene::Load_Scene()
{
	RESOURCES.Delete_NonStaticResources();
	PHYSX.Init();

	//Static
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Hero\\", true);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\VfxMesh\\", true);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\SkyBox\\", true);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Weapon\\", true);

	//Map
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\Mir\\", false);

	//Monster
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Boss_Mir\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Boss_Mir2\\", false);

	auto player = Load_Player();
	Load_Camera(player);
	Load_MapFile(L"MirMap", player);
	Load_Boss_Mir(player);
	//Load_Boss_Giant_Mir(player);

	Load_Ui(player);


	auto pPlayer = Get_GameObject(L"Player");
	if (nullptr != pPlayer)
	{
		for(_uint i = 1 ; i < IDX(HERO::MAX); ++i)
			pPlayer->Get_Script<HeroChangeScript>()->Change_Hero(static_cast<HERO>(i));

		pPlayer->Get_Script<HeroChangeScript>()->Change_Hero(HERO::PLAYER);
	}


	return S_OK;
}

shared_ptr<GameObject> MirScene::Load_Player()
{
	if (CUR_SCENE && typeid(*CUR_SCENE.get()) == typeid(LoadingScene))
	{
		auto pPlayer = static_pointer_cast<LoadingScene>(CUR_SCENE)->Get_StaticObjectFromLoader(L"Player");
		if (pPlayer && pPlayer->Get_CharacterController())
		{
			auto& desc = pPlayer->Get_CharacterController()->Get_CapsuleControllerDesc();
			desc.radius = 0.5f;
			desc.height = 5.f;
			_float3 vPos = pPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz();
			desc.position = { vPos.x, vPos.y, vPos.z };
			pPlayer->Get_CharacterController()->Create_Controller();
			return pPlayer;
		}
	}
	
	{
		// Add. Player
		shared_ptr<GameObject> ObjPlayer = make_shared<GameObject>();
		
		ObjPlayer->Add_Component(make_shared<Transform>());
	
		ObjPlayer->Get_Transform()->Set_State(Transform_State::POS, _float4(-0.f, 0.f, 0.f, 1.f));
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(L"Player");
				model->AddParts(L"Am_Ct_Hat_022", PARTS_INFO::Hair);
				model->AddParts(L"Am_Head_001", PARTS_INFO::Head);
				model->AddParts(L"Am_Ct_Uniform_022", PARTS_INFO::Uniform);
				animator->Set_Model(model);
				
			}

			ObjPlayer->Add_Component(animator);
			ObjPlayer->Add_Component(make_shared<Player_FSM>());
		}
		ObjPlayer->Set_Name(L"Player");
		ObjPlayer->Set_VelocityMap(true);
		ObjPlayer->Add_Component(make_shared<OBBBoxCollider>(_float3{ 0.5f, 0.8f, 0.5f })); //obbcollider
		ObjPlayer->Get_Collider()->Set_CollisionGroup(Player_Body);
		ObjPlayer->Get_Collider()->Set_Activate(true);

		{
			auto controller = make_shared<CharacterController>();
			ObjPlayer->Add_Component(controller);
			auto& desc = controller->Get_CapsuleControllerDesc();
			desc.radius = 0.5f;
			desc.height = 5.f;
			_float3 vPos = ObjPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz();
			desc.position = { vPos.x, vPos.y, vPos.z };
			controller->Create_Controller();
		}
		ObjPlayer->Set_DrawShadow(true);
		ObjPlayer->Set_ObjectGroup(OBJ_PLAYER);

		Add_GameObject(ObjPlayer,true);

		//Add. Player's Weapon
		shared_ptr<GameObject> ObjWeapon = make_shared<GameObject>();

		ObjWeapon->Add_Component(make_shared<Transform>());
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(L"Weapon_Player");
				renderer->Set_Model(model);
			}

			ObjWeapon->Add_Component(renderer);

			WeaponScript::WEAPONDESC desc;
			desc.strBoneName = L"Bip001-Prop1";
			desc.matPivot = _float4x4::CreateRotationX(-XM_PI / 2.f) * _float4x4::CreateRotationZ(XM_PI);
			desc.pWeaponOwner = ObjPlayer;

			ObjWeapon->Add_Component(make_shared<WeaponScript>(desc));
		}
		ObjWeapon->Set_DrawShadow(true);
		ObjWeapon->Set_Name(L"Weapon_Player");
		ObjWeapon->Set_VelocityMap(true);
		Add_GameObject(ObjWeapon,true);

		ObjPlayer->Add_Component(make_shared<HeroChangeScript>());

		return ObjPlayer;
	}
	return nullptr;
}

void MirScene::Load_Camera(shared_ptr<GameObject> pPlayer)
{
	{
		//GameObj for Camera Create
		shared_ptr<GameObject> camera = make_shared<GameObject>();

		// Transform Component 
		camera->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(1.f, 1.f, 1.f, 1.f));

		camera->GetOrAddTransform()->Set_Speed(5.f);

		// Camera Component Add
		CameraDesc desc;
		desc.fFOV = XM_PI / 3.f;
		desc.strName = L"Default";
		desc.fSizeX = _float(g_iWinSizeX);
		desc.fSizeY = _float(g_iWinSizeY);
		desc.fNear = 0.1f;
		desc.fFar = 1000.f;
		shared_ptr<Camera> cameraComponent = make_shared<Camera>(desc);
		
		camera->Add_Component(cameraComponent);

	
		camera->Get_Camera()->Set_ProjType(ProjectionType::Perspective);
		//Layer_UI culling true
		camera->Get_Camera()->Set_CullingMaskLayerOnOff(Layer_UI, true);

		camera->Add_Component(make_shared<MainCameraScript>(pPlayer));
		camera->Get_Script<MainCameraScript>()->Get_MaxDist() = 25.f;
		Add_GameObject(camera);
	
		//Setting Camera
		pPlayer->Get_FSM()->Set_Camera(camera);
	}
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

void MirScene::Load_Monster(_uint iCnt, const wstring& strMonsterTag, shared_ptr<GameObject> pPlayer)
{
	{
		for (_uint i = 0; i < iCnt; i++)
		{
			// Add. Monster
			shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();

			ObjMonster->Add_Component(make_shared<Transform>());

			ObjMonster->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, (rand() % 5) + 30.f, 1.f));
			{
				shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

				shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
				{
					shared_ptr<Model> model = RESOURCES.Get<Model>(strMonsterTag);
					animator->Set_Model(model);
				}

				ObjMonster->Add_Component(animator);

				if (strMonsterTag == L"Silversword_Soldier")
					ObjMonster->Add_Component(make_shared<Silversword_Soldier_FSM>());
				else if (strMonsterTag == L"Succubus_Scythe")
					ObjMonster->Add_Component(make_shared<Succubus_Scythe_FSM>());
				else if (strMonsterTag == L"Undead_Priest")
					ObjMonster->Add_Component(make_shared<Undead_Priest_FSM>());
				else if (strMonsterTag.find(L"Alpaca") != wstring::npos)
					ObjMonster->Add_Component(make_shared<Alpaca_FSM>());
				else if (strMonsterTag == L"Wolf")
					ObjMonster->Add_Component(make_shared<Wolf_FSM>());

				ObjMonster->Get_FSM()->Set_Target(pPlayer);
			}
			ObjMonster->Add_Component(make_shared<OBBBoxCollider>(_float3{ 0.5f, 0.7f, 0.5f })); //obbcollider
			ObjMonster->Get_Collider()->Set_CollisionGroup(Monster_Body);
			ObjMonster->Get_Collider()->Set_Activate(true);

			wstring strMonsterName = (L"Monster") + to_wstring(i);
			ObjMonster->Set_Name(strMonsterName);
			{
				auto controller = make_shared<CharacterController>();
				ObjMonster->Add_Component(controller);
				auto& desc = controller->Get_CapsuleControllerDesc();
				desc.radius = 0.5f;
				desc.height = 5.f;
				_float3 vPos = ObjMonster->Get_Transform()->Get_State(Transform_State::POS).xyz();
				desc.position = { vPos.x, vPos.y, vPos.z };
				controller->Create_Controller();
			}
			ObjMonster->Set_ObjectGroup(OBJ_MONSTER);

			Add_GameObject(ObjMonster);
		}
	}
}

void MirScene::Load_Boss_Mir(shared_ptr<GameObject> pPlayer)
{
	// Add. Monster
	shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();

	ObjMonster->Add_Component(make_shared<Transform>());

	ObjMonster->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

		shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
		{
			shared_ptr<Model> model = RESOURCES.Get<Model>(L"Boss_Mir");
			animator->Set_Model(model);
		}

		ObjMonster->Add_Component(animator);
		ObjMonster->Add_Component(make_shared<Boss_Mir_FSM>());
		ObjMonster->Get_FSM()->Set_Target(pPlayer);
	}
	ObjMonster->Add_Component(make_shared<OBBBoxCollider>(_float3{ 2.f, 4.f, 6.f })); //obbcollider
	ObjMonster->Get_Collider()->Set_CollisionGroup(Monster_Body);
	ObjMonster->Get_Collider()->Set_Activate(true);

	ObjMonster->Add_Component(make_shared<CounterMotionTrailScript>());

	wstring strMonsterName = (L"Boss_Mir");
	ObjMonster->Set_Name(strMonsterName);
	{
		auto controller = make_shared<CharacterController>();
		ObjMonster->Add_Component(controller);

		auto& desc = controller->Get_CapsuleControllerDesc();
		desc.radius = 4.5f;
		desc.height = 5.f;
		desc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
		
		_float3 vPos = ObjMonster->Get_Transform()->Get_State(Transform_State::POS).xyz() +
					   ObjMonster->Get_Transform()->Get_State(Transform_State::LOOK);
		desc.position = { vPos.x, vPos.y, vPos.z };
		controller->Create_Controller();
		controller->Get_Actor()->setStepOffset(0.1f);

	}
	ObjMonster->Set_ObjectGroup(OBJ_MONSTER);
	ObjMonster->Set_VelocityMap(true);
	Add_GameObject(ObjMonster);
}

void MirScene::Load_Boss_Giant_Mir(shared_ptr<GameObject> pPlayer)
{
	// Add. Monster
	shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();

	ObjMonster->Add_Component(make_shared<Transform>());

	ObjMonster->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

		shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
		{
			shared_ptr<Model> model = RESOURCES.Get<Model>(L"Mir_02");
			animator->Set_Model(model);
		}

		ObjMonster->Add_Component(animator);
		//ObjMonster->Add_Component(make_shared<Boss_Mir_FSM>());
		//ObjMonster->Get_FSM()->Set_Target(pPlayer);
	}
	ObjMonster->Add_Component(make_shared<OBBBoxCollider>(_float3{ 2.f, 4.f, 6.f })); //obbcollider
	ObjMonster->Get_Collider()->Set_CollisionGroup(Monster_Body);
	ObjMonster->Get_Collider()->Set_Activate(true);

	//ObjMonster->Add_Component(make_shared<CounterMotionTrailScript>());

	wstring strMonsterName = (L"Giant_Mir");
	ObjMonster->Set_Name(strMonsterName);
	{
		auto controller = make_shared<CharacterController>();
		ObjMonster->Add_Component(controller);

		auto& desc = controller->Get_CapsuleControllerDesc();
		desc.radius = 4.5f;
		desc.height = 5.f;
		desc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;

		_float3 vPos = ObjMonster->Get_Transform()->Get_State(Transform_State::POS).xyz() +
			ObjMonster->Get_Transform()->Get_State(Transform_State::LOOK);
		desc.position = { vPos.x, vPos.y, vPos.z };
		controller->Create_Controller();
		controller->Get_Actor()->setStepOffset(0.1f);

	}
	ObjMonster->Set_ObjectGroup(OBJ_MONSTER);
	ObjMonster->Set_VelocityMap(true);
	Add_GameObject(ObjMonster);
}

void MirScene::Load_Boss_Dellons(shared_ptr<GameObject> pPlayer)
{
	{
		// Add. Boss_Dellons
		shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();

		ObjMonster->Add_Component(make_shared<Transform>());

		ObjMonster->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 30.f, 1.f));
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(L"Dellons");
				animator->Set_Model(model);
			}

			ObjMonster->Add_Component(animator);
			ObjMonster->Add_Component(make_shared<Boss_Dellons_FSM>());
			
			ObjMonster->Get_FSM()->Set_Target(pPlayer);
		}
		ObjMonster->Set_Name(L"Boss_Dellons");
		ObjMonster->Set_VelocityMap(true);
		ObjMonster->Add_Component(make_shared<OBBBoxCollider>(_float3{ 0.5f, 0.8f, 0.5f })); //obbcollider
		ObjMonster->Get_Collider()->Set_CollisionGroup(Monster_Body);
		ObjMonster->Get_Collider()->Set_Activate(true);

		{
			auto controller = make_shared<CharacterController>();
			ObjMonster->Add_Component(controller);
			auto& desc = controller->Get_CapsuleControllerDesc();
			desc.radius = 0.5f;
			desc.height = 5.f;
			_float3 vPos = ObjMonster->Get_Transform()->Get_State(Transform_State::POS).xyz();
			desc.position = { vPos.x, vPos.y, vPos.z };
			controller->Create_Controller();
		}
		ObjMonster->Set_DrawShadow(true);
		ObjMonster->Set_ObjectGroup(OBJ_MONSTER);
		Add_GameObject(ObjMonster);


		//Add. Player's Weapon
		shared_ptr<GameObject> ObjWeapon = make_shared<GameObject>();

		ObjWeapon->Add_Component(make_shared<Transform>());
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(L"Weapon_Dellons");
				renderer->Set_Model(model);
			}

			ObjWeapon->Add_Component(renderer);

			WeaponScript::WEAPONDESC desc;
			desc.strBoneName = L"Bip001-Prop1";
			desc.matPivot = _float4x4::CreateRotationX(-XM_PI / 2.f) * _float4x4::CreateRotationZ(XM_PI);
			desc.pWeaponOwner = ObjMonster;

			ObjWeapon->Add_Component(make_shared<WeaponScript>(desc));
		}
		ObjWeapon->Set_DrawShadow(true);
		ObjWeapon->Set_Name(L"Weapon_Boss_Dellons");
		ObjWeapon->Set_VelocityMap(true);
		Add_GameObject(ObjWeapon);

	}
}

void MirScene::Load_Boss_Spike(shared_ptr<GameObject> pPlayer)
{
	{
		// Add. Boss_Dellons
		shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();

		ObjMonster->Add_Component(make_shared<Transform>());

		ObjMonster->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 55.f, 1.f));
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(L"Boss_Spike");
				animator->Set_Model(model);
			}

			ObjMonster->Add_Component(animator);
			ObjMonster->Add_Component(make_shared<Boss_Spike_FSM>());

			ObjMonster->Get_FSM()->Set_Target(pPlayer);
		}
		ObjMonster->Set_Name(L"Boss_Spike");
		ObjMonster->Set_VelocityMap(true);
		ObjMonster->Add_Component(make_shared<OBBBoxCollider>(_float3{ 0.5f, 0.8f, 0.5f })); //obbcollider
		ObjMonster->Get_Collider()->Set_CollisionGroup(Monster_Body);
		ObjMonster->Get_Collider()->Set_Activate(true);

		ObjMonster->Add_Component(make_shared<CounterMotionTrailScript>());

		{
			auto controller = make_shared<CharacterController>();
			ObjMonster->Add_Component(controller);
			auto& desc = controller->Get_CapsuleControllerDesc();
			desc.radius = 0.5f;
			desc.height = 5.f;
			_float3 vPos = ObjMonster->Get_Transform()->Get_State(Transform_State::POS).xyz();
			desc.position = { vPos.x, vPos.y, vPos.z };
			controller->Create_Controller();
		}
		ObjMonster->Set_DrawShadow(true);
		ObjMonster->Set_ObjectGroup(OBJ_MONSTER);
		Add_GameObject(ObjMonster);


		//Add. Player's Weapon
		shared_ptr<GameObject> ObjWeapon = make_shared<GameObject>();

		ObjWeapon->Add_Component(make_shared<Transform>());
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(L"Weapon_Spike");
				renderer->Set_Model(model);
			}

			ObjWeapon->Add_Component(renderer);

			WeaponScript::WEAPONDESC desc;
			desc.strBoneName = L"Bip001-Prop1";
			desc.matPivot = _float4x4::CreateRotationX(-XM_PI / 2.f) * _float4x4::CreateRotationZ(XM_PI);
			desc.pWeaponOwner = ObjMonster;

			ObjWeapon->Add_Component(make_shared<WeaponScript>(desc));
		}
		ObjWeapon->Set_DrawShadow(true);
		ObjWeapon->Set_Name(L"Weapon_Boss_Spike");
		ObjWeapon->Set_VelocityMap(true);
		Add_GameObject(ObjWeapon);
	}
}

void MirScene::Load_Ui(shared_ptr<GameObject> pPlayer)
{
	list<shared_ptr<GameObject>>& tmp = static_pointer_cast<LoadingScene>(CUR_SCENE)->Get_StaticObjectsFromLoader();
	Load_UIFile(L"..\\Resources\\UIData\\UI_Main.dat", tmp);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Main_Button.dat", tmp);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Char_Change.dat", tmp);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Card_Deck.dat", tmp, false);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Target_LockOn.dat", tmp, false);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Cur_Quest.dat", tmp, false);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Setting.dat", tmp, false);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Controller.dat", tmp, false);


	{
		weak_ptr<GameObject> pObj = Get_UI(L"Main_UI_Controller");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<MainUiController>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Gacha_Controller");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<UiGachaController>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Card_Deck_Controller");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<UiCardDeckController>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Damage_Controller");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<UiDamageCreate>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		auto& pObj = pPlayer;
		if (nullptr != pObj)
		{
			auto pScript = make_shared<UiHpBarController>(0);
			pObj->Add_Component(pScript);
		}
	}



	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Target_LockOn");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<UiTargetLockOn>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Setting_Controller");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<UiSettingController>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Dialog_Controller");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<UiQuestController>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Boss_Dialog");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<UiBossDialog>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Interaction");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<UIInteraction>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		auto pObj = Get_UI(L"UI_Combo_Effect");
		if (nullptr != pObj)
		{
			auto pScript = make_shared<UiComboEffect>();
			pObj->Add_Component(pScript);
		}
	}
	{
		auto pObj = Get_UI(L"UI_Skill_Use_Gauge");
		if (nullptr != pObj)
		{
			auto pScript = make_shared<UiSkillGauge>();
			pObj->Add_Component(pScript);
		}
	}


	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Card_Deck_Exit");
		if (false == pObj.expired())
		{
			pObj.lock()->Get_Button()->AddOnClickedEvent([]()
				{
					CUR_SCENE->Get_GameObject(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Render_Off();
				});
		}
	}


	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Char_Change");
		if (false == pObj.expired())
		{
			auto pScript = make_shared<UiCharChange>();
			pObj.lock()->Add_Component(pScript);
		}
	}

	{
		auto pObj = Get_UI(L"UI_Main_Button2");
		if (nullptr != pObj)
		{
			pObj->Get_Button()->AddOnClickedEvent([]()
				{
					CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Render_On();
				});
		}
	}

	{
		auto pObj = Get_UI(L"UI_Main_Button3");
		if (nullptr != pObj)
		{
			pObj->Get_Button()->AddOnClickedEvent([]()
				{
					CUR_SCENE->Get_UI(L"UI_Setting_Controller")->Get_Script<UiSettingController>()->Set_Render(true);
				});
		}
	}


	{
		auto pScript = make_shared<UiSkillButtonEffect>();
		auto pObj = Get_UI(L"UI_Skill0_Effect");
		if (nullptr != pObj)
			pObj->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		pObj = Get_UI(L"UI_Skill2_Effect");
		if (nullptr != pObj)
			pObj->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		pObj = Get_UI(L"UI_Skill3_Effect");
		if (nullptr != pObj)
			pObj->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		pObj = Get_UI(L"UI_Skill4_Effect");
		if (nullptr != pObj)
			pObj->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		pObj = Get_UI(L"UI_Skill5_Effect");
		if (nullptr != pObj)
			pObj->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		pObj = Get_UI(L"UI_Skill6_Effect");
		if (nullptr != pObj)
			pObj->Add_Component(pScript);

	}

	{
		for (_uint i = 2; i < 7; ++i)
		{
			auto pScript = make_shared<UiCoolEndEffect>();
			wstring strTemp = L"UI_Cool_End";
			strTemp += to_wstring(i);
			auto pObj = Get_UI(strTemp);
			if (nullptr != pObj)
				pObj->Add_Component(pScript);
		}
	}

	{
		auto pScript = make_shared<CoolTimeCheckScript>();
		auto& pObj = pPlayer;
		if (nullptr != pObj) {
			pObj->Add_Component(pScript);
		}

	}

	{
		for (_uint i = 0; i < 32; ++i)
		{
			auto pScript = make_shared<UiCardDeckInvenChange>(0);
			wstring strTemp = L"UI_Card_Deck_Inven";
			strTemp += to_wstring(i);
			weak_ptr<GameObject> pObj = Get_UI(strTemp);
			if (true != pObj.expired())
			{
				pObj.lock()->Add_Component(pScript);
				pObj.lock()->Get_Button()->AddOnClickedEvent([pObj]()
					{
						CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Click_Deck_Inven(pObj.lock()->Get_Name());
					});
			}
		}

		for (_uint i = 0; i < 32; ++i)
		{
			auto pScript = make_shared<UiCardDeckInvenChange>(1);
			wstring strTemp = L"UI_Card_Deck_Inven_Element";
			strTemp += to_wstring(i);
			auto pObj = Get_UI(strTemp);
			if (nullptr != pObj)
				pObj->Add_Component(pScript);
		}

		for (_uint i = 0; i < 32; ++i)
		{
			auto pScript = make_shared<UiCardDeckInvenChange>(2);
			wstring strTemp = L"UI_Card_Deck_InvenBg";
			strTemp += to_wstring(i);
			auto pObj = Get_UI(strTemp);
			if (nullptr != pObj)
				pObj->Add_Component(pScript);
		}
	}

	{
		for (_uint i = 0; i < 3; ++i)
		{
			wstring strTemp = L"UI_Card_Deck";
			strTemp += to_wstring(i);
			weak_ptr<GameObject> pObj = Get_UI(strTemp);
			if (true != pObj.expired())
			{
				pObj.lock()->Get_Button()->AddOnClickedEvent([pObj]()
					{
						CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Click_Deck_Select(pObj.lock()->Get_Name());
					});
			}
		}

		for (_uint i = 0; i < 3; ++i)
		{
			wstring strTemp = L"UI_Card_Deck_X";
			strTemp += to_wstring(i);
			weak_ptr<GameObject> pObj = Get_UI(strTemp);
			if (true != pObj.expired())
			{
				pObj.lock()->Get_Button()->AddOnClickedEvent([pObj]()
					{
						CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Click_Deck_X(pObj.lock()->Get_Name());
					});
			}
		}
	}

}
