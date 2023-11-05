#include "pch.h"
#include "DemoScene.h"

// For. Components
#include "Light.h"
#include "Camera.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"

// For. Script
#include "FreeCameraScript.h"

// For. Etc
#include "Model.h"
#include "ImGuiToolMgr.h"

DemoScene::DemoScene()
{
}

void DemoScene::Init()
{
	__super::Init();

	COLLISION.Check_Group(_int(CollisionGroup::Player), _int(CollisionGroup::Monster));

	// For. Initialize ImGui Tool mgr
	IMGUITOOL_MGR.Init(DEVICE.Get(), CONTEXT.Get());
	IMGUITOOL_MGR.Tick();
}

void DemoScene::Tick()
{
	__super::Tick();

	IMGUITOOL_MGR.Tick();
}

void DemoScene::Late_Tick()
{
	__super::Late_Tick();
}

void DemoScene::Final_Tick()
{
	__super::Final_Tick();
}

void DemoScene::Render()
{
	__super::Render();

	IMGUITOOL_MGR.Render();
}

HRESULT DemoScene::Load_Scene()
{
	RESOURCES.CreateModel(L"..\\Resources\\Models\\");
	//Load_DemoModel();
	Load_Light();
	Load_Camera();

	return S_OK;
}

void DemoScene::Load_DemoModel()
{
	// For. 예시 모델이 될 GameObject생성 
	shared_ptr<GameObject> testObj = make_shared<GameObject>();

	// For. GameObject에 Transform Component 추가
		// 둘중 하나 사용. AddComponent 또는 GetOrAddTransform(있으면 반환 없으면 생성후 반환)
	testObj->Add_Component(make_shared<Transform>());
		//testObj->GetOrAddTransform();

	// For. GameObject의 Transform 설정해주기 
	testObj->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
	
	// For. Animator생성할때 필요한 
	shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

	// For. GameObject에 붙일 animator component 생성하기 
	shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
	
	// For. Animator component에 붙일 Model 생성하기 
	shared_ptr<Model> model = RESOURCES.Get<Model>(L"Kyle");

	// For. Animator에 Model 붙이기
	animator->Set_Model(model);
	
	// For. GameObject에 Animator component 붙이기
	testObj->Add_Component(animator);
		//testObj->Add_Component(make_shared<DemoAnimationController1>());

	// For. 씬에 GameObject추가 
	Add_GameObject(testObj);
}

void DemoScene::Load_Light()
{
	// For. 씬에 추가할 빛 역할을 할 GameObject생성 
	shared_ptr<GameObject> lightObj = make_shared<GameObject>();

	// For. GameObject의 Transform 설정 
	lightObj->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, 25.f, 0.f, 1.f));
	lightObj->GetOrAddTransform()->Set_LookDir(_float3(-1.f, -1.f, -1.f));

	// For. GameObject에 붙일 LightComponent 생성
	shared_ptr<Light> lightCom = make_shared<Light>();
	lightCom->Set_Diffuse(Color(1.f));
	lightCom->Set_Ambient(Color(0.8f));
	lightCom->Set_Specular(Color(0.f));
	lightCom->Set_Emissive(Color(0.f));
	lightCom->Set_LightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);

	// For. GameObject에 Light Component 붙이기 
	lightObj->Add_Component(lightCom);

	// For. 씬에 빛 역할을 하는 GameObject 추가 
	Add_GameObject(lightObj);
}

void DemoScene::Load_Camera()
{
	/* UI가 아닌 일반 모델들을 그리는 카메라를 생성해서 씬에 추가하기 */

	// For. 씬에 추가할 Camera가 될 GameObject 생성 
	shared_ptr<GameObject> camera = make_shared<GameObject>();

	// For. 카메라 Transform 세팅 
	camera->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
	camera->GetOrAddTransform()->Set_Speed(5.f);

	// For. GameObject에 붙일 카메라 컴포넌트 생성 
	CameraDesc desc;
	desc.fFOV = XM_PI / 3.f;
	desc.strName = L"Default";
	desc.fSizeX = _float(g_iWinSizeX);
	desc.fSizeY = _float(g_iWinSizeY);
	desc.fNear = 0.1f;
	desc.fFar = 1000.f;
	shared_ptr<Camera> cameraComponent = make_shared<Camera>(desc);

	// For. 생성한 카메라 컴포넌트를 GameObject에 붙이기 
	camera->Add_Component(cameraComponent);

	// For. GameObject의 카메라 컴포넌트가 어떤 투영을 할지 설정하기 
	camera->Get_Camera()->Set_ProjType(ProjectionType::Perspective);

	// For. GameObject의 카메라 컴포넌트가 UI는 그리지 않도록 설정 
	camera->Get_Camera()->Set_CullingMaskLayerOnOff(Layer_UI, true);

	// For. GameObject에 key input에 따른 이동에 관한 스크립트 붙이기 
	camera->Add_Component(make_shared<FreeCameraScript>());

	// For. 씬에 생성한 카메라 추가 
	Add_GameObject(camera);
}