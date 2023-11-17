#include "pch.h"
#include "Kyle_GachaScene.h"

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
#include "Gacha_FSM.h"
namespace fs = std::filesystem;

Kyle_GachaScene::Kyle_GachaScene(const GachaSceneDesc& desc)
	: m_Desc(desc)
{
}

Kyle_GachaScene::~Kyle_GachaScene()
{
}

void Kyle_GachaScene::Init()
{
	Load_Scene();
	__super::Init();
}

void Kyle_GachaScene::Tick()
{
	__super::Tick();
}

void Kyle_GachaScene::Late_Tick()
{
	__super::Late_Tick();
}

void Kyle_GachaScene::Final_Tick()
{
	__super::Final_Tick();
	if (KEYTAP(KEY_TYPE::ENTER))
	{
		SCENE.End_SubScene();
	}
}

HRESULT Kyle_GachaScene::Load_Scene()
{
	Load_Player();
	Load_Light();
	Load_Camera();
	Load_MapFile(m_Desc.strMapFileName);
	return S_OK;
}

void Kyle_GachaScene::Load_Player()
{

	// Add. Player
	shared_ptr<GameObject> ObjPlayer = make_shared<GameObject>();

	ObjPlayer->Add_Component(make_shared<Transform>());

	ObjPlayer->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
	ObjPlayer->Get_Transform()->LookAt(_float4(0.f, 0.f, -1.f, 1.f));

	wstring modelName = L"";

	ObjPlayer->Set_DrawShadow(true);
	ObjPlayer->Set_ObjectGroup(OBJ_PLAYER);
	Add_GameObject(ObjPlayer);
	Gacha_FSM_Desc desc;
	shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
	shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);

	
	switch (m_Desc.eHeroType)
	{
	case HERO::KYLE:
		modelName = L"Kyle";
		{
			shared_ptr<Model> model = RESOURCES.Get<Model>(modelName);
			animator->Set_Model(model);
			model->Get_MaterialByName(L"mi_Wp_Kyle_Chain")->Get_MaterialDesc().emissive = Color(214.f/255.f,55.f/255.f, 0.0f, 1.f);
			model->Get_MaterialByName(L"mi_Wp_Kyle")->Get_MaterialDesc().emissive = Color(214.f / 255.f, 55.f / 255.f, 0.0f, 1.f);
			ObjPlayer->Add_Component(animator);
		}
		desc.strWeaponName = L"";
		desc.strAnimTag = L"SQ_SpecialHero_Kyle";
		desc.iAnimStopFrame = 134;

		{
			g_fBrightness = 0.f;
			g_fContrast = 1.04f;
			g_Saturation = 1.5;
			g_iTMIndex = 1;
			g_fMaxWhite = 0.9f;
		}
		{
			g_SSAOData.g_bSSAO_On = true;
		}
		{
			g_bFXAAOn = true;
		}
		{
			g_bLensFlare = true;
		}
		{
			g_MotionBlurData.g_bMotionBlurOn = true;
			g_MotionBlurData.g_iBlurCount = 20;
		}
		{
			g_BloomData.g_BloomOn = true;
		}
		{
			g_DOFData.g_bDOF_On = true;
			g_DOFData.g_FocusDepth = 30.f;
			g_DOFData.g_DOFRange = 35.f;
		}

		break;
	case HERO::YEOPO:
		modelName = L"Yeopo";
		{
			shared_ptr<Model> model = RESOURCES.Get<Model>(modelName);
			animator->Set_Model(model);

			ObjPlayer->Add_Component(animator);
		}
		desc.strWeaponName = L"Weapon_Yeopo";
		desc.strAnimTag = L"SQ_SpecialHero_Yeopo";
		desc.iAnimStopFrame = 134;
		{
			shared_ptr<GameObject> ObjVehicle = make_shared<GameObject>();

			ObjVehicle->GetOrAddTransform()->Set_WorldMat(ObjPlayer->Get_Transform()->Get_WorldMatrix());
			{
				shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

				shared_ptr<ModelAnimator> renderer = make_shared<ModelAnimator>(shader);
				{
					shared_ptr<Model> model = RESOURCES.Get<Model>(L"Yeopo_Horse");
					renderer->Set_Model(model);
				}

				ObjVehicle->Add_Component(renderer);
				renderer->Set_CurrentAnim(L"SQ_SpecialHero_Yeopo_Horse", false, 1.f);
			}

			ObjVehicle->Set_Name(L"Yeopo_RedHorse");

			Add_GameObject(ObjVehicle);
		}
		break;
	case HERO::MAX:
		break;
	default:
		break;
	}

	ObjPlayer->Set_Name(modelName);
	ObjPlayer->Add_Component(make_shared<Gacha_FSM>(desc));

	

	
}

void Kyle_GachaScene::Load_Camera()
{
	{
		//GameObj for Camera Create
		shared_ptr<GameObject> camera = make_shared<GameObject>();

		// Transform Component 
		camera->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(_float3(0.f,5.f,5.f), 1.f));
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

void Kyle_GachaScene::Load_Light()
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
