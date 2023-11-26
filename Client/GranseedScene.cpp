#include "pch.h"
#include "GranseedScene.h"

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
#include "GroupEffectData.h"
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

#include "MeshEffectData.h"
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
#include "UIInteraction.h"
#include "UiQuestController.h"
#include "UiSettingController.h"

#include <filesystem>
#include "GachaScene.h"
#include "KrisScene.h"
#include "MirScene.h"
#include "AttackColliderInfoScript.h"
#include "Shane_FSM.h"
#include "NPC_FSM.h"
#include "GranseedGuard01_FSM.h"
namespace fs = std::filesystem;

GranseedScene::GranseedScene()
{
}

GranseedScene::~GranseedScene()
{
}

void GranseedScene::Init()
{
	__super::Init();

	auto pPlayer = Get_GameObject(L"Player");
	if (nullptr != pPlayer)
	{
		for (_uint i = 1; i < IDX(HERO::MAX); ++i)
			pPlayer->Get_Script<HeroChangeScript>()->Change_Hero(static_cast<HERO>(i));

		pPlayer->Get_Script<HeroChangeScript>()->Change_Hero(HERO::PLAYER);
	}

	for(_int i =0; i< _int(HERO::MAX); ++i)
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

void GranseedScene::Tick()
{
	__super::Tick();

	if (KEYTAP(KEY_TYPE::C))
	{
		auto pObj = Get_UI(L"UI_Interaction");
		if(pObj)
		pObj->Get_Script<UIInteraction>()->Create_Interaction(NPCTYPE::QUEST);
	}
	if (KEYTAP(KEY_TYPE::V))
	{
		auto pObj = Get_UI(L"UI_Interaction");
		pObj->Get_Script<UIInteraction>()->Remove_Interaction();
	}
	if (KEYTAP(KEY_TYPE::Z))
	{
		auto pObj = Get_UI(L"UI_Dialog_Controller");
		pObj->Get_Script<UiQuestController>()->Change_Value();
	}
}

void GranseedScene::Late_Tick()
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

void GranseedScene::Final_Tick()
{
	__super::Final_Tick();
	PHYSX.Tick();
	
	if (KEYTAP(KEY_TYPE::TAB)) 
	{
		/*GachaSceneDesc sceneDesc{ L"YeopoMap",HERO::YEOPO};
			SCENE.Add_SubScene(make_shared<GachaScene>(sceneDesc));
		SCENE.Exchange_Scene();*/

		shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<MirScene>());
		scene->Set_StaticObjects(m_StaticObject);
		PHYSX.Set_CharacterControllerNull();
		SCENE.Change_Scene(scene);
		g_bCutScene = false;
	}
}

HRESULT GranseedScene::Load_Scene()
{
	RESOURCES.Delete_NonStaticResources();
	PHYSX.Init();

	//Static
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Hero\\", true);

	RESOURCES.CreateModel(L"..\\Resources\\Models\\VfxMesh\\", true);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\SkyBox\\", true);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Weapon\\", true);

	//Map
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\Granseed\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\Character\\Npc\\Granseed\\", false);
	// Gacha
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\Kyle\\", false);
	RESOURCES.CreateModel(L"..\\Resources\\Models\\MapObject\\Yeopo\\", false);

	auto pPlayer = Load_Player();
	Load_Camera(pPlayer);
	Load_MapFile(L"GranseedMap", pPlayer);

	Load_Ui(pPlayer);
	Load_NPC(L"GranseedMap");
	return S_OK;
}

shared_ptr<GameObject> GranseedScene::Load_Player()
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

			{
				shared_ptr<GameObject> attackCollider = make_shared<GameObject>();
				attackCollider->GetOrAddTransform();
				attackCollider->Add_Component(make_shared<SphereCollider>(1.f));
				attackCollider->Get_Collider()->Set_CollisionGroup(Player_Attack);

				ObjPlayer->Get_FSM()->Set_AttackCollider(attackCollider) ;

				Add_GameObject(attackCollider,true);
				attackCollider->Get_Collider()->Set_Activate(false);

				attackCollider->Add_Component(make_shared<AttackColliderInfoScript>());
				attackCollider->Set_Name(L"Player_AttackCollider");
				attackCollider->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(ObjPlayer);

			}
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
		ObjPlayer->Get_FSM()->Set_Weapon(ObjWeapon);
		ObjPlayer->Add_Component(make_shared<HeroChangeScript>());
		return ObjPlayer;
	}
	return nullptr;
}

void GranseedScene::Load_Camera(shared_ptr<GameObject> pPlayer)
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

void GranseedScene::Load_Ui(shared_ptr<GameObject> pPlayer)
{
	/*wstring assetPath = L"..\\Resources\\Textures\\UITexture\\Main\\";

	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		wstring filePath = entry.path().wstring();
		wstring fileName = entry.path().filename().wstring();
		Utils::DetachExt(fileName);
		RESOURCES.Load<Texture>(fileName, filePath);
	}*/
	list<shared_ptr<GameObject>>& tmp = static_pointer_cast<LoadingScene>(CUR_SCENE)->Get_StaticObjectsFromLoader();
	Load_UIFile(L"..\\Resources\\UIData\\UI_Main.dat", tmp);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Main_Button.dat", tmp);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Char_Change.dat", tmp);
	//Load_UIFile(L"..\\Resources\\UIData\\UI_Gacha.dat");
	Load_UIFile(L"..\\Resources\\UIData\\UI_Card_Deck.dat", tmp, false);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Target_LockOn.dat", tmp, false);
	//Load_UIFile(L"..\\Resources\\UIData\\UI_MonsterHp.dat", tmp);
	//Load_UIFile(L"..\\Resources\\UIData\\UI_Mouse.dat");
	Load_UIFile(L"..\\Resources\\UIData\\UI_Cur_Quest.dat", tmp, false);
	Load_UIFile(L"..\\Resources\\UIData\\UI_Setting.dat", tmp, false);


	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"Main_Ui_Controller");

		auto pScript = make_shared<MainUiController>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj, true);
	}

	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Gacha_Controller");

		auto pScript = make_shared<UiGachaController>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj);
	}

	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Card_Deck_Controller");

		auto pScript = make_shared<UiCardDeckController>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj);
	}

	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Damage_Controller");

		auto pScript = make_shared<UiDamageCreate>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj);
	}

	{
		auto& pObj = pPlayer;
		if (nullptr != pObj)
		{
			auto pScript = make_shared<UiHpBarController>(0);
			pObj->Add_Component(pScript);
		}
	}

	/*{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Monster_Hp");

		auto pScript = make_shared<UiMonsterHp>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj);
	}*/

	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Target_LockOn");

		auto pScript = make_shared<UiTargetLockOn>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj);
	}

	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Setting_Controller");

		auto pScript = make_shared<UiSettingController>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj);
	}

	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Dialog_Controller");

		auto pScript = make_shared<UiQuestController>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj);
	}
	
	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Interaction");

		auto pScript = make_shared<UIInteraction>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj);
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
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Char_Change");

		auto pScript = make_shared<UiCharChange>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj);
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

void GranseedScene::Load_Debug()
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

void GranseedScene::Load_NPC(const wstring& dataFileName)
{
	wstring strFilePath = L"..\\Resources\\MapData\\" + dataFileName + L".subdata";
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(strFilePath, FileMode::Read);

	_int count = file->Read<_int>();
	for (_int i = 0; i < count; ++i)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->GetOrAddTransform();
		obj->Set_Name(Utils::ToWString(file->Read<string>()));

		string modelTag = file->Read<string>();
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
		shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
		animator->Set_Model(RESOURCES.Get<Model>(Utils::ToWString(modelTag)));
		obj->Add_Component(animator);

		animator->Set_CurrentAnim(file->Read<_uint>(), true, 1.f);
		obj->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(file->Read<_float3>(), 1.f));
		obj->GetOrAddTransform()->Set_Quaternion(file->Read<Quaternion>());

		_bool isMoving = file->Read<_bool>();
		_int iFSMIndex = file->Read<_int>();
		_float3 vMinPos = file->Read<_float3>();
		_float3 vMaxPos = file->Read<_float3>();

		if (isMoving)
		{
			switch (iFSMIndex)
			{
			case NPC_FSM::POTION:

				break;
			case NPC_FSM::GUARD1:
			{
				auto fsm = make_shared<GranseedGuard01_FSM>();
				fsm->Set_MinMovePos(vMinPos);
				fsm->Set_MaxMovePos(vMaxPos);
				obj->Add_Component(fsm);
			}

				break;
			case NPC_FSM::GUARD2:

				break;
			case NPC_FSM::TRAVELER:

				break;
			}
			{
				// Potion_FSM
			}



		}

		Add_GameObject(obj);
	}
}
