#include "pch.h"
#include "DemoScene.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "MainCameraScript.h"
#include "DemoCameraScript1.h"
#include "DemoCameraScript2.h"
#include "DemoAnimationController1.h"
#include "DemoFSM.h"
#include "FileUtils.h"
#include "Utils.h"
#include "SpearAce_FSM.h"
#include "MeshRenderer.h"
#include "BaseUI.h"

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
	Load_DemoModel();
	Load_Light();
	Load_Camera();
	Load_Ui();

	return S_OK;
}

void DemoScene::Load_DemoModel()
{
	/*{
		shared_ptr<FileUtils> file = make_shared<FileUtils>();
		file->Open(L"", FileMode::Write);

		file->Write<_uint>(3);
		file->Write<string>(Utils::ToString(L"AWSDF"));
	}
	{
		shared_ptr<FileUtils> file = make_shared<FileUtils>();
		file->Open(L"", FileMode::Read);

		_uint a = file->Read<_uint>();
		wstring name = Utils::ToWString(file->Read<string>());
	}*/



	{

		shared_ptr<GameObject> testObj = make_shared<GameObject>();
		// Transform Component

		testObj->Add_Component(make_shared<Transform>());
		//testObj->GetOrAddTransform();

		testObj->Get_Transform()->Set_State(Transform_State::POS, _float4(3.f, 0.f, 3.f, 1.f));
		{
			shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

			shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
			{
				shared_ptr<Model> model = RESOURCES.Get<Model>(L"Spear_Ace");
				animator->Set_Model(model);
			}

			testObj->Add_Component(animator);
		}

		{
			shared_ptr<SpearAce_FSM> fsm = make_shared<SpearAce_FSM>();
			testObj->Add_Component(fsm);
		}
		testObj->Set_Name(L"Player");

		Add_GameObject(testObj);
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
		camera->Add_Component(make_shared<DemoCameraScript1>());
		camera->Add_Component(make_shared<DemoCameraScript2>());
	
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

void DemoScene::Load_Ui()
{
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(L"..\\Resources\\UI\\asdfasdf.dat", FileMode::Read);

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
		material->Set_TextureMap(RESOURCES.Get<Texture>(Utils::ToWString(file->Read<string>())), TextureMapType::DIFFUSE);
		//material->Set_TextureMap(RESOURCES.Get<Texture>(Utils::ToWString(file->Read<string>())), TextureMapType::DIFFUSE);

		UiObject->GetOrAddTransform()->Set_WorldMat(file->Read<_float4x4>());
		_uint iIndex = file->Read<_uint>();
		renderer->Set_Pass(iIndex);
		renderer->Get_RenderParamDesc() = file->Read<RenderParams>();
		renderer->Set_Material(material);
		UiObject->Add_Component(renderer);

		_bool bIsUseBaseUi = file->Read<_bool>();
		if (true == bIsUseBaseUi)
		{
			auto BaseUi = make_shared<BaseUI>();
			BaseUi->Get_Desc() = file->Read<BaseUI::BASEUIDESC>();
		}

		UiObject->Set_LayerIndex(Layer_UI);
		UiObject->Set_Instancing(false);
		Add_GameObject(UiObject);
	}
}
