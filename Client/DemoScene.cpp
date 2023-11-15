#include "pch.h"
#include "DemoScene.h"

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
#include "UiGachaController.h"
#include "Boss_Mir_FSM.h"
#include "DemoAnimationController1.h"
#include "UiCardDeckController.h"
#include "MainUiController.h"
#include "UiCardDeckInvenChange.h"


#include <filesystem>
#include "Player_FSM.h"
#include "GachaScene.h"
namespace fs = std::filesystem;

DemoScene::DemoScene()
{
}

DemoScene::~DemoScene()
{
}

void DemoScene::Init()
{
	__super::Init();
	COLLISION.Check_Group(_int(CollisionGroup::Player_Attack), _int(CollisionGroup::Monster_Body));
	COLLISION.Check_Group(_int(CollisionGroup::Player_Skill), _int(CollisionGroup::Monster_Body));
	COLLISION.Check_Group(_int(CollisionGroup::Monster_Attack), _int(CollisionGroup::Player_Body));
	COLLISION.Check_Group(_int(CollisionGroup::Monster_Skill), _int(CollisionGroup::Player_Body));
	COLLISION.Check_Group(_int(CollisionGroup::Player_Body), _int(CollisionGroup::MAPObject));
	//m_pGachaScene = make_shared<GachaScene>();
	//m_pGachaScene->Init();
}

void DemoScene::Tick()
{
	__super::Tick();

	/*if (KEYTAP(KEY_TYPE::C))
	{
		Get_GameObject(L"Main_Ui_Controller")->Get_Script<MainUiController>()->Set_MainUI_Render(false);
	}
	if (KEYTAP(KEY_TYPE::V))
	{
		Get_GameObject(L"Main_Ui_Controller")->Get_Script<MainUiController>()->Set_MainUI_Render(true);
	}*/
}

void DemoScene::Late_Tick()
{

	__super::Late_Tick();

	// Test
	{
		shared_ptr<GameObject> player = Get_GameObject(L"Player");
		auto animator = player->Get_Animator();

		if (KEYTAP(KEY_TYPE::X))
		{
			if (m_bRenderDebug)
				m_bRenderDebug = false;
			else
				m_bRenderDebug = true;
		}
		/*{
			auto model = animator->Get_Model();

			if (KEYPUSH(KEY_TYPE::Q))
			{
				for (auto& material : model->Get_Materials())
					material->Get_MaterialDesc().emissive.x += fDT;
			}

			if (KEYPUSH(KEY_TYPE::E))
			{
				for (auto& material : model->Get_Materials())
					material->Get_MaterialDesc().emissive.x -= fDT;
			}
		}*/
		{
			/*if (KEYPUSH(KEY_TYPE::Q))
			{
				animator->Get_RenderParamDesc().vec4Params[0].x += fDT;
			}

			if (KEYPUSH(KEY_TYPE::E))
			{
				animator->Get_RenderParamDesc().vec4Params[0].x -= fDT;
			}*/


		}
	}
}

void DemoScene::Final_Tick()
{
	__super::Final_Tick();
	PHYSX.Tick();
}

HRESULT DemoScene::Load_Scene()
{
	PHYSX.Init();
	RESOURCES.CreateModel(L"..\\Resources\\Models\\");
	RESOURCES.LoadParts();
	Load_SkyBox();
	Load_Player();
	Load_Light();
	Load_Camera();
	Load_MapFile(L"KrisMap");
	Load_Monster(1);
	Load_Boss_Mir();

	Load_Ui();

	return S_OK;
}

void DemoScene::Load_SkyBox()
{
	shared_ptr<GameObject> sky = make_shared<GameObject>();
	sky->GetOrAddTransform();
	sky->Add_Component(make_shared<ModelRenderer>(RESOURCES.Get<Shader>(L"SkyBox.fx")));
	sky->Get_ModelRenderer()->Set_Model(RESOURCES.Get<Model>(L"SkyBox"));
	sky->Set_Name(L"SkyBase");
	Add_GameObject(sky);

	// 텍스쳐 정보는 LoadMapFile에서 재변경
}

void DemoScene::Load_Player()
{
	{
		// Add. Player
		shared_ptr<GameObject> ObjPlayer = make_shared<GameObject>();
		
		ObjPlayer->Add_Component(make_shared<Transform>());
	
		ObjPlayer->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 1.5f, 1.f));
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
		Add_GameObject(ObjPlayer);

	
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
		Add_GameObject(ObjWeapon);

		ObjPlayer->Add_Component(make_shared<HeroChangeScript>());
	
	}

	{
		shared_ptr<GameObject> debugText = make_shared<GameObject>();
		debugText->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(-300.f, 400.f, 5.f, 1.f));
		debugText->Add_Component(make_shared<FontRenderer>(L""));
		debugText->Get_FontRenderer()->Set_Font(RESOURCES.Get<CustomFont>(L"136ex"), Color(0.5f,0.5f,0.5f,1.f), 1.f);
		debugText->Set_LayerIndex(Layer_UI);
		debugText->Add_Component(make_shared<ObjectTransformDebug>());
		debugText->Get_Script<ObjectTransformDebug>()->Set_Target(Get_GameObject(L"Player"));
		Add_GameObject(debugText);
	}
}

void DemoScene::Load_Camera()
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

		// MonoBehaviour(Component �� ������ �ƴѰ͵�) �߰�
		// �Ϻη� ��� ��������
		auto pPlayer = Get_GameObject(L"Player");
		camera->Add_Component(make_shared<MainCameraScript>(pPlayer));

		Add_GameObject(camera);
	
		//Setting Camera
		pPlayer->Get_FSM()->Set_Camera(camera);
	}
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();

		// Transform Component �߰�
		camera->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));

		// ī�޶� Component ���� 
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

void DemoScene::Load_Monster(_uint iCnt)
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
					shared_ptr<Model> model = RESOURCES.Get<Model>(L"Silversword_Soldier");
					animator->Set_Model(model);
				}

				ObjMonster->Add_Component(animator);
				ObjMonster->Add_Component(make_shared<Silversword_Soldier_FSM>());
				auto pPlayer = Get_GameObject(L"Player");
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

void DemoScene::Load_Boss_Mir()
{
	// Add. Monster
	shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();

	ObjMonster->Add_Component(make_shared<Transform>());

	ObjMonster->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 30.f, 1.f));
	{
		shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

		shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
		{
			shared_ptr<Model> model = RESOURCES.Get<Model>(L"Boss_Mir");
			animator->Set_Model(model);
		}

		ObjMonster->Add_Component(animator);
		ObjMonster->Add_Component(make_shared<Boss_Mir_FSM>());
		auto pPlayer = Get_GameObject(L"Player");
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

void DemoScene::Load_Light()
{
	shared_ptr<GameObject> lightObj = make_shared<GameObject>();
	lightObj->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(50.f, 30.f, 20.f, 1.f));
	lightObj->GetOrAddTransform()->LookAt(_float4(0.f,0.f,0.f,1.f));
	{
		shared_ptr<Light> lightCom = make_shared<Light>();
		lightCom->Set_Diffuse(Color(0.7f));
		lightCom->Set_Ambient(Color(0.4f));
		lightCom->Set_Specular(Color(0.f));
		lightCom->Set_Emissive(Color(1.f));
		lightCom->Set_LightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		lightObj->Add_Component(lightCom);
		
	}

	Add_GameObject(lightObj);

	// 방향성광원정보 LoadMap에서 재설정
}

void DemoScene::Load_Ui()
{
	wstring assetPath = L"..\\Resources\\Textures\\UITexture\\Main\\";

	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		wstring filePath = entry.path().wstring();
		wstring fileName = entry.path().filename().wstring();
		Utils::DetachExt(fileName);
		RESOURCES.Load<Texture>(fileName, filePath);
	}

	Load_UIFile(L"..\\Resources\\UIData\\UI_Main.dat");
	Load_UIFile(L"..\\Resources\\UIData\\UI_Main_Button.dat");
	Load_UIFile(L"..\\Resources\\UIData\\UI_Char_Change.dat");
	//Load_UIFile(L"..\\Resources\\UIData\\UI_Gacha.dat");
	Load_UIFile(L"..\\Resources\\UIData\\UI_Card_Deck.dat", false);


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
		Add_GameObject(pObj, true);
	}

	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Card_Deck_Controller");

		auto pScript = make_shared<UiCardDeckController>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj, true);
	}

	{
		auto pObj = Get_GameObject(L"Player");
        auto pScript = make_shared<UiHpBarController>(0);
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj, true);
	}

	{
		auto pObj = make_shared<GameObject>();
		pObj->Set_Name(L"UI_Char_Change");

		auto pScript = make_shared<UiCharChange>();
		pObj->Add_Component(pScript);

		pObj->Set_LayerIndex(Layer_UI);
		Add_GameObject(pObj, true);
	}

	{
		auto pObj = Get_GameObject(L"UI_Main_Button2");
		pObj->Get_Button()->AddOnClickedEvent([]()
			{
				CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Render_On();
			});
	}





	{
		auto pScript = make_shared<UiSkillButtonEffect>();
		Get_GameObject(L"UI_Skill0_Effect")->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		Get_GameObject(L"UI_Skill2_Effect")->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		Get_GameObject(L"UI_Skill3_Effect")->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		Get_GameObject(L"UI_Skill4_Effect")->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		Get_GameObject(L"UI_Skill5_Effect")->Add_Component(pScript);

		pScript = make_shared<UiSkillButtonEffect>();
		Get_GameObject(L"UI_Skill6_Effect")->Add_Component(pScript);
	}

	{
		auto pScript = make_shared<UiCoolEndEffect>();
		Get_GameObject(L"UI_Cool_End2")->Add_Component(pScript);

		pScript = make_shared<UiCoolEndEffect>();
		Get_GameObject(L"UI_Cool_End3")->Add_Component(pScript);

		pScript = make_shared<UiCoolEndEffect>();
		Get_GameObject(L"UI_Cool_End4")->Add_Component(pScript);

		pScript = make_shared<UiCoolEndEffect>();
		Get_GameObject(L"UI_Cool_End5")->Add_Component(pScript);

		pScript = make_shared<UiCoolEndEffect>();
		Get_GameObject(L"UI_Cool_End6")->Add_Component(pScript);
	}

	{
		auto pScript = make_shared<CoolTimeCheckScript>();
		Get_GameObject(L"Player")->Add_Component(pScript);
	}

	

	


	{
		for (_uint i = 0; i < 32; ++i)
		{
			auto pScript = make_shared<UiCardDeckInvenChange>(0);
			wstring strTemp = L"UI_Card_Deck_Inven";
			strTemp += to_wstring(i);
			weak_ptr<GameObject> pObj = Get_UI(strTemp);
			pObj.lock()->Add_Component(pScript);

			pObj.lock()->Get_Button()->AddOnClickedEvent([pObj]()
				{
					CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Click_Deck_Inven(pObj.lock()->Get_Name());
				});
		}

		for (_uint i = 0; i < 32; ++i)
		{
			auto pScript = make_shared<UiCardDeckInvenChange>(1);
			wstring strTemp = L"UI_Card_Deck_Inven_Element";
			strTemp += to_wstring(i);
			Get_GameObject(strTemp)->Add_Component(pScript);
		}

		for (_uint i = 0; i < 32; ++i)
		{
			auto pScript = make_shared<UiCardDeckInvenChange>(2);
			wstring strTemp = L"UI_Card_Deck_InvenBg";
			strTemp += to_wstring(i);
			Get_GameObject(strTemp)->Add_Component(pScript);
		}
	}

	/*shared_ptr<GameObject> obj = make_shared<GameObject>();
	obj->GetOrAddTransform()->Scaled(_float3(10.f));
	{
		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx"));
		shared_ptr<Material> material = make_shared<Material>();
		material->Set_TextureMap(RESOURCES.Get<Texture>(L"SUBSCENEFINALTarget"),TextureMapType::DIFFUSE);
		renderer->Get_RenderParamDesc().vec4Params[0] = _float4(1);
		renderer->Set_Material(material);
		renderer->Set_Mesh(RESOURCES.Get<Mesh>(L"Quad"));
		obj->Add_Component(renderer);
	}
	Add_GameObject(obj);*/

	{
		for (_uint i = 0; i < 3; ++i)
		{
			wstring strTemp = L"UI_Card_Deck";
			strTemp += to_wstring(i);
			weak_ptr<GameObject> pObj = Get_UI(strTemp);

			pObj.lock()->Get_Button()->AddOnClickedEvent([pObj]()
				{
					CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Click_Deck_Select(pObj.lock()->Get_Name());
				});
		}	
		
		for (_uint i = 0; i < 3; ++i)
		{
			wstring strTemp = L"UI_Card_Deck_X";
			strTemp += to_wstring(i);
			weak_ptr<GameObject> pObj = Get_UI(strTemp);

			pObj.lock()->Get_Button()->AddOnClickedEvent([pObj]()
				{
					CUR_SCENE->Get_UI(L"UI_Card_Deck_Controller")->Get_Script<UiCardDeckController>()->Click_Deck_X(pObj.lock()->Get_Name());
				});
		}
	}

}