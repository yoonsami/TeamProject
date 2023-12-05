﻿#include "pch.h"
#include "KrisScene.h"

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
#include "UiTargetLockOn.h"
#include "UiMonsterHp.h"
#include "UiDamageCreate.h"
#include "UIBossHpBar.h"
#include "UiComboEffect.h"
#include "UiSkillGauge.h"


#include <filesystem>
#include "GachaScene.h"
#include "GranseedScene.h"
#include "SpikeScene.h"
#include "UiSettingController.h"
#include "UiQuestController.h"
#include "UiBossDialog.h"
#include "UIInteraction.h"
#include "NeutralAlpaca_FSM.h"
#include "UiCostumeController.h"
namespace fs = std::filesystem;

KrisScene::KrisScene()
{
	m_strSceneName = L"KrisScene";
}

KrisScene::~KrisScene()
{
}

void KrisScene::Init()
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


}

void KrisScene::Tick()
{
	__super::Tick();
}

void KrisScene::Late_Tick()
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

void KrisScene::Final_Tick()
{
	__super::Final_Tick();
	PHYSX.Tick();

	if (KEYPUSH(KEY_TYPE::TAB) && KEYPUSH(KEY_TYPE::F4))
	{

		/*GachaSceneDesc sceneDesc{ L"YeopoMap",HERO::YEOPO};
			SCENE.Add_SubScene(make_shared<GachaScene>(sceneDesc));
		SCENE.Exchange_Scene();*/


		shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<SpikeScene>());
		PHYSX.Set_CharacterControllerNull();

		scene->Set_StaticObjects(m_StaticObject);
		SCENE.Change_Scene(scene);
		g_bCutScene = false;
	}
	if (KEYPUSH(KEY_TYPE::TAB) && KEYPUSH(KEY_TYPE::F7))
	{
		/*GachaSceneDesc sceneDesc{ L"YeopoMap",HERO::YEOPO};
			SCENE.Add_SubScene(make_shared<GachaScene>(sceneDesc));
		SCENE.Exchange_Scene();*/

		shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<KrisScene>());
		scene->Set_StaticObjects(m_StaticObject);
		PHYSX.Set_CharacterControllerNull();
		SCENE.Change_Scene(scene);
		g_bCutScene = false;
	}
}

HRESULT KrisScene::Load_Scene()
{
	RESOURCES.Delete_NonStaticResources();
	PHYSX.Init();

	//Static
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Hero\\", true);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\VfxMesh\\", true);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\SkyBox\\", true);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Weapon\\", true);

	//Map
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\Kris\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\Spike\\", false);

	//Monster
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Boss_Spike\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Silversword_Soldier\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Alpaca_White\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Alpaca_Brown\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Alpaca_Black\\", false);

	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Wolf\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Succubus_Scythe\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Monster\\Undead_Priest\\", false);

	auto player = Load_Player();
	Load_Camera(player);
	Load_MapFile(L"KrisMap", player);

	Load_Monster(5, L"Alpaca_White", player);
	Load_Monster(5, L"Alpaca_Brown", player);
	Load_Monster(5, L"Alpaca_Black", player); 
	Load_Monster(5, L"Bad_Alpaca_White", player);
	Load_Monster(5, L"Bad_Alpaca_Brown", player);
	Load_Monster(5, L"Bad_Alpaca_Black", player);
	//Load_Monster(2, L"Wolf", player);

	////Load_Boss_Spike(player);				
	Load_Boss_Dellons(player);				


	Load_Ui(player);
	Load_EventScript();
	return S_OK;
}

shared_ptr<GameObject> KrisScene::Load_Player()
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

		ObjPlayer->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
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

void KrisScene::Load_Camera(shared_ptr<GameObject> pPlayer)
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

void KrisScene::Load_Monster(_uint iCnt, const wstring& strMonsterTag, shared_ptr<GameObject> pPlayer, _bool bCharacterController)
{
	{
		for (_uint i = 0; i < iCnt; i++)
		{
			// Add. Monster
			shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();

			ObjMonster->Add_Component(make_shared<Transform>());

			_float fRan = 0;
			if (rand() % 2 == 0)
				fRan = -1.f;
			else
				fRan = 1.f;

			ObjMonster->Get_Transform()->Set_State(Transform_State::POS, _float4(_float(rand() % 20) * fRan, 0.f, _float(rand() % 15) + 30.f, 1.f));
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
				else if (strMonsterTag.find(L"Bad_Alpaca") != wstring::npos)
					ObjMonster->Add_Component(make_shared<Alpaca_FSM>());
				else if (strMonsterTag.find(L"Alpaca") != wstring::npos)
					ObjMonster->Add_Component(make_shared<NeutralAlpaca_FSM>());
				else if (strMonsterTag == L"Wolf")
					ObjMonster->Add_Component(make_shared<Wolf_FSM>());

				ObjMonster->Get_FSM()->Set_Target(pPlayer);
			}
			ObjMonster->Add_Component(make_shared<OBBBoxCollider>(_float3{ 0.5f, 0.7f, 0.5f })); //obbcollider
			ObjMonster->Get_Collider()->Set_CollisionGroup(Monster_Body);
			ObjMonster->Get_Collider()->Set_Activate(true);

			wstring strMonsterName = strMonsterTag + to_wstring(i);
			ObjMonster->Set_Name(strMonsterName);
			{
				//Alpaca and Wolf don't use character controller
				if (bCharacterController)
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
			}
			ObjMonster->Set_ObjectGroup(OBJ_MONSTER);

			Add_GameObject(ObjMonster);
		}
	}
}

void KrisScene::Load_Boss_Mir(shared_ptr<GameObject> pPlayer)
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

void KrisScene::Load_Boss_Dellons(shared_ptr<GameObject> pPlayer)
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

void KrisScene::Load_Boss_Spike(shared_ptr<GameObject> pPlayer)
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

void KrisScene::Load_Ui(shared_ptr<GameObject> pPlayer)
{
	list<shared_ptr<GameObject>>& tmp = static_pointer_cast<LoadingScene>(CUR_SCENE)->Get_StaticObjectsFromLoader();
	Load_UIFile(L"..\\Resources\\UIData\\UI_Controller.dat", tmp, false);

	Load_UIFile(L"..\\Resources\\UIData\\UI_Main.dat", tmp);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Main_Button.dat", tmp);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Char_Change.dat", tmp);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Card_Deck.dat", tmp, false, false);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Target_LockOn.dat", tmp, false, false);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Cur_Quest.dat", tmp, false, false);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Setting.dat", tmp, false, false);

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
		auto pObj = Get_UI(L"UI_Costume_Controller");
		if (nullptr != pObj)
		{
			auto pScript = make_shared<UiCostumeController>();
			pObj->Add_Component(pScript);
		}
	}

	{
		weak_ptr<GameObject> pObj = Get_UI(L"UI_Card_Deck_Exit");
		if (false == pObj.expired())
		{
			pObj.lock()->Get_Button()->AddOnClickedEvent([]()
				{
					CUR_SCENE->Get_GameObject(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Set_Render(false);
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
		auto pObj = Get_UI(L"UI_Main_Button0");
		if (nullptr != pObj)
		{
			pObj->Get_Button()->AddOnClickedEvent([]()
				{
					CUR_SCENE->Get_UI(L"UI_Setting_Controller")->Get_Script<UiSettingController>()->Set_Render(true);
				});
		}
	}

	{
		auto pObj = Get_UI(L"UI_Main_Button1");
		if (nullptr != pObj)
		{
			pObj->Get_Button()->AddOnClickedEvent([]()
				{
					CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Set_Render(true);
				});
		}
	}

	{
		auto pObj = Get_UI(L"UI_Main_Button2");
		if (nullptr != pObj)
		{
			pObj->Get_Button()->AddOnClickedEvent([]()
				{
					CUR_SCENE->Get_UI(L"UI_Costume_Controller")->Get_Script<UiCostumeController>()->Create_Costume();
				});
		}
	}

	{
		for (_uint i = 0; i < 7; ++i)
		{
			if (1 == i)
				continue;

			wstring strName = L"UI_Skill" + to_wstring(i) + L"_Effect";
			auto pScript = make_shared<UiSkillButtonEffect>();
			auto pObj = Get_UI(strName);
			if (nullptr != pObj)
				pObj->Add_Component(pScript);
		}
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
}

void KrisScene::Load_Debug()
{
	{
		shared_ptr<GameObject> debugText = make_shared<GameObject>();
		debugText->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(-300.f, 400.f, 5.f, 1.f));
		debugText->Add_Component(make_shared<FontRenderer>(L""));
		debugText->Get_FontRenderer()->Set_Font(RESOURCES.Get<CustomFont>(L"136ex"), Color(0.5f, 0.5f, 0.5f, 1.f), 1.f);
		debugText->Set_LayerIndex(Layer_UI);
		debugText->Add_Component(make_shared<ObjectTransformDebug>());
		debugText->Get_Script<ObjectTransformDebug>()->Set_Target(Get_GameObject(L"Boss_Spike"));
		Add_GameObject(debugText);
	}
}

void KrisScene::Load_EventScript()
{
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(1.619f, -0.029f, -27.929f, 1.f));
		obj->Add_Component(make_shared<AABBBoxCollider>(_float3(10.f, 4.f, 4.f)));
		obj->Get_Collider()->Set_Activate(true);
		obj->Get_Collider()->Set_CollisionGroup(MAPObject);
		obj->Set_Name(L"Alpaca_Exit");

		Add_GameObject(obj);

		
	}

}
