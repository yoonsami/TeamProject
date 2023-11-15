#include "pch.h"
#include "GachaScene.h"

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
#include "UiHpBarFlow.h"
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
namespace fs = std::filesystem;

GachaScene::GachaScene()
{
}

GachaScene::~GachaScene()
{
}

void GachaScene::Init()
{
	Load_Scene();
	__super::Init();
}

void GachaScene::Tick()
{
	__super::Tick();
}

void GachaScene::Late_Tick()
{
	__super::Late_Tick();
}

void GachaScene::Final_Tick()
{
	__super::Final_Tick();
}

HRESULT GachaScene::Load_Scene()
{
	Load_SkyBox();
	Load_Player();
	Load_Light();
	Load_Camera();
	Load_Monster(1);
	Load_Boss_Mir();

	return S_OK;
}

void GachaScene::Load_SkyBox()
{
	shared_ptr<GameObject> sky = make_shared<GameObject>();
	sky->GetOrAddTransform();
	sky->Add_Component(make_shared<ModelRenderer>(RESOURCES.Get<Shader>(L"SkyBox.fx")));
	sky->Get_ModelRenderer()->Set_Model(RESOURCES.Get<Model>(L"SkyBox"));
	sky->Set_Name(L"SkyBase");
	Add_GameObject(sky);

	// 텍스쳐 정보는 LoadMapFile에서 재변경
}

void GachaScene::Load_Player()
{
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
		}
		ObjPlayer->Set_Name(L"Player");
	
		ObjPlayer->Set_DrawShadow(true);
		ObjPlayer->Set_ObjectGroup(OBJ_PLAYER);
		Add_GameObject(ObjPlayer);

	
	}


}

void GachaScene::Load_Camera()
{
	{
		//GameObj for Camera Create
		shared_ptr<GameObject> camera = make_shared<GameObject>();

		// Transform Component 
		camera->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(_float3(2.f), 1.f));
		camera->GetOrAddTransform()->LookAt(_float4(0.f, 0.f, 0.f, 1.f));
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

		Add_GameObject(camera);
	}
}

void GachaScene::Load_Monster(_uint iCnt)
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

void GachaScene::Load_Boss_Mir()
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

void GachaScene::Load_Light()
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
}
