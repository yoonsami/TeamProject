#include "pch.h"
#include "DemoScene.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"
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
DemoScene::DemoScene()
{
}

DemoScene::~DemoScene()
{
}

void DemoScene::Init()
{
	__super::Init();
	COLLISION.Check_Group(_int(CollisionGroup::Player), _int(CollisionGroup::Monster));
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
}

HRESULT DemoScene::Load_Scene()
{
	RESOURCES.CreateModel(L"..\\Resources\\Models\\");
	Load_Spear_Ace();
	Load_Light();
	Load_Camera();


	return S_OK;
}

void DemoScene::Load_Spear_Ace()
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
	}

}

void DemoScene::Load_Kyle()
{
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
		desc.strName = L"Player_Cam";
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
	
	//	camera->Add_Component(make_shared<DemoCameraScript1>());
	//	camera->Add_Component(make_shared<DemoCameraScript2>());
	
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
