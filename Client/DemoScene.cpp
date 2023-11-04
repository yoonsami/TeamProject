#include "pch.h"
#include "DemoScene.h"
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
#include <Utils.h>
#include "SpearAce_FSM.h"
#include "FontRenderer.h"
#include "CustomFont.h"
#include "MeshCollider.h"
#include "RigidBody.h"
#include "ObjectTransformDebug.h"
#include "CharacterController.h"
#include "HeroChangeScript.h"
#include "Silversword_Soldier_FSM.h"
#include "OBBBoxCollider.h"


DemoScene::DemoScene()
{
}

DemoScene::~DemoScene()
{
}

void DemoScene::Init()
{
	__super::Init();
	COLLISION.Check_Group(_int(CollisionGroup::Player_Body), _int(CollisionGroup::Monster_Body));
	COLLISION.Check_Group(_int(CollisionGroup::Player_Attack), _int(CollisionGroup::Monster_Body));
	COLLISION.Check_Group(_int(CollisionGroup::Player_Skill), _int(CollisionGroup::Monster_Body));
	COLLISION.Check_Group(_int(CollisionGroup::Monster_Attack), _int(CollisionGroup::Player_Body));
	COLLISION.Check_Group(_int(CollisionGroup::Monster_Skill), _int(CollisionGroup::Player_Body));
}

void DemoScene::Tick()
{
	__super::Tick();
}

void DemoScene::Late_Tick()
{
	__super::Late_Tick();
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
	Load_Player();
	Load_Light();
	Load_Camera();
	Load_Monster();
	Load_DemoMap();


	return S_OK;
}

void DemoScene::Load_Player()
{
	{
		// Add. Player
		shared_ptr<GameObject> ObjPlayer = make_shared<GameObject>();
		
		ObjPlayer->Add_Component(make_shared<Transform>());
	
		ObjPlayer->Get_Transform()->Set_State(Transform_State::POS, _float4(3.f, 0.f, 3.f, 1.f));
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(L"Spear_Ace");
				animator->Set_Model(model);
			}

			ObjPlayer->Add_Component(animator);
			ObjPlayer->Add_Component(make_shared<SpearAce_FSM>());
		}
		ObjPlayer->Set_Name(L"Player");
		ObjPlayer->Add_Component(make_shared<OBBBoxCollider>(_float3{ 0.5f, 0.7f, 0.5f })); //obbcollider
		ObjPlayer->Get_Collider()->Set_CollisionGroup(Player_Body);
		ObjPlayer->Get_Collider()->Set_Activate(true);
			
		{
			auto controller = make_shared<CharacterController>();
			ObjPlayer->Add_Component(controller);
			auto& desc = controller->Get_ControllerDesc();
			desc.radius = 0.5f;
			desc.height = 5.f;
			_float3 vPos = ObjPlayer->Get_Transform()->Get_State(Transform_State::POS).xyz();
			desc.position = { vPos.x, vPos.y, vPos.z };
			controller->Create_Controller();
		}
		Add_GameObject(ObjPlayer);

	
		//Add. Player's Weapon
		shared_ptr<GameObject> ObjWeapon = make_shared<GameObject>();

		ObjWeapon->Add_Component(make_shared<Transform>());
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(L"Weapon_Spear_Ace");
				renderer->Set_Model(model);
			}

			ObjWeapon->Add_Component(renderer);

			WeaponScript::WEAPONDESC desc;
			desc.strBoneName = L"Bip001-Prop1";
			desc.matPivot = _float4x4::CreateRotationX(-XM_PI / 2.f) * _float4x4::CreateRotationZ(XM_PI);
			desc.pWeaponOwner = ObjPlayer;

			ObjWeapon->Add_Component(make_shared<WeaponScript>(desc));
		}
		
		ObjWeapon->Set_Name(L"Weapon_Spear_Ace");
		Add_GameObject(ObjWeapon);

		ObjPlayer->Add_Component(make_shared<HeroChangeScript>(ObjPlayer));
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

void DemoScene::Load_DemoMap()
{
	auto shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
	vector<wstring> modelName;
	modelName.push_back(L"BG_Wall_1");
	modelName.push_back(L"BG_Wall_2");
	modelName.push_back(L"BG_Wall_3");
	modelName.push_back(L"BG_Wall_4");
	modelName.push_back(L"Building_A");
	modelName.push_back(L"Building_B");
	modelName.push_back(L"Building_C");
	modelName.push_back(L"Ground");
	modelName.push_back(L"Wall");

	for(auto& modelTag: modelName)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->GetOrAddTransform();
		obj->Add_Component(make_shared<ModelRenderer>(shader));
		obj->Get_ModelRenderer()->Set_Model(RESOURCES.Get<Model>(modelTag));
		obj->Set_Name(modelTag);
		Add_GameObject(obj);
	}
	
	{
		auto gameObject = Get_GameObject(L"Wall");
		
		shared_ptr<MeshCollider> collider = make_shared<MeshCollider>(L"Wall_Collider");
		gameObject->Add_Component(collider);
		auto rigidBody = make_shared<RigidBody>();
		rigidBody->Create_RigidBody(collider);
		gameObject->Add_Component(rigidBody);
	}
	{
		auto gameObject = Get_GameObject(L"Ground");

		shared_ptr<MeshCollider> collider = make_shared<MeshCollider>(L"Ground_Collider");
		gameObject->Add_Component(collider);
		auto rigidBody = make_shared<RigidBody>();
		rigidBody->Create_RigidBody(collider);
		gameObject->Add_Component(rigidBody);
	}
}

void DemoScene::Load_Camera()
{
	{
		//GameObj for Camera Create
		shared_ptr<GameObject> camera = make_shared<GameObject>();

		// Transform Component 
		camera->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));

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
		//Layer_UI�� �ִ� ������Ʈ�� �ø��ϰڴ�.
		camera->Get_Camera()->Set_CullingMaskLayerOnOff(Layer_UI, false);

		Add_GameObject(camera);
	}
}

void DemoScene::Load_Monster()
{
	{
		// Add. Player
		shared_ptr<GameObject> ObjMonster = make_shared<GameObject>();

		ObjMonster->Add_Component(make_shared<Transform>());

		ObjMonster->Get_Transform()->Set_State(Transform_State::POS, _float4(6.f, 0.f, 6.f, 1.f));
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

		ObjMonster->Set_Name(L"Monster1");
		{
			auto controller = make_shared<CharacterController>();
			ObjMonster->Add_Component(controller);
			auto& desc = controller->Get_ControllerDesc();
			desc.radius = 0.5f;
			desc.height = 5.f;
			_float3 vPos = ObjMonster->Get_Transform()->Get_State(Transform_State::POS).xyz();
			desc.position = { vPos.x, vPos.y, vPos.z };
			controller->Create_Controller();
		}

		Add_GameObject(ObjMonster);
	}
}

void DemoScene::Load_Light()
{
	shared_ptr<GameObject> lightObj = make_shared<GameObject>();
	lightObj->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, 25.f, 0.f, 1.f));
	lightObj->GetOrAddTransform()->Set_LookDir(_float3(-1.f,-1.f,-1.f));
	{
		shared_ptr<Light> lightCom = make_shared<Light>();
		lightCom->Set_Diffuse(Color(1.f));
		lightCom->Set_Ambient(Color(0.8f));
		lightCom->Set_Specular(Color(0.f));
		lightCom->Set_Emissive(Color(0.f));
		lightCom->Set_LightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		lightObj->Add_Component(lightCom);
		
	}

	Add_GameObject(lightObj);
}
