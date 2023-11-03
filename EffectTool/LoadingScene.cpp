#include "pch.h"
#include "LoadingScene.h"

#include "Loader.h"

#include "Camera.h"
#include "GameObject.h"
#include "MeshRenderer.h"

LoadingScene::LoadingScene(shared_ptr<Scene> nextScene)
	:m_pNextScene(nextScene)
{
}

void LoadingScene::Init()
{
	{
		// For. Create Camera (For Watch UI)
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->GetOrAddTransform();
		camera->GetOrAddTransform()->Set_Speed(5.f);
		CameraDesc desc;
		desc.fFOV = XM_PI / 3.f;
		desc.strName = L"UI";
		desc.fSizeX = _float(g_iWinSizeX);
		desc.fSizeY = _float(g_iWinSizeY);
		desc.fNear = 0.1f;
		desc.fFar = 1000.f;
		camera->Add_Component(make_shared<Camera>(desc));
		camera->Get_Camera()->Set_ProjType(ProjectionType::Orthographic);
		camera->Get_Camera()->Set_CullingMaskAll();
		camera->Get_Camera()->Set_CullingMaskLayerOnOff(Layer_UI, false);

		// For. 생성한 Camera를 씬에 추가함
		Add_GameObject(camera);
	}

	// For. Create Background
	// Create_LoadingBG();

	// For. Create Loader 
	m_pLoader = make_shared<Loader>(m_pNextScene);
	(m_pLoader->Init());

	__super::Init();
}

void LoadingScene::Tick()
{
	__super::Tick();
}

void LoadingScene::Late_Tick()
{
	__super::Late_Tick();
}

void LoadingScene::Final_Tick()
{
	__super::Final_Tick();

	if (!m_pLoader->m_bLoadFinished)
		return;

	SCENE.Change_Scene(m_pNextScene);
}

void LoadingScene::Create_LoadingBG()
{
	/* Logo 이미지가 될 GameObject 생성하기 */
	auto LoadingBackGround = make_shared<GameObject>();
	LoadingBackGround->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0, 0, 2, 1));
	LoadingBackGround->GetOrAddTransform()->Scaled(_float3(g_iWinSizeX, g_iWinSizeY, 1.f));

	// For. GameObject에 붙일 Renderer 생성하기 
	shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx"));

	// For. GameObject에 붙일 Renderer에 추가할 Quad 메쉬(rect 버퍼) 생성하기(클론하기)
	auto mesh = RESOURCES.Get<Mesh>(L"Quad");	// Resource manager는 기본적인 프로토타입들을 들고있다고 생각하면됨. Quad : Rect버퍼같은것.  Get으로 하나 클론해온다는 것. 
	
	// For. Renderer에 그리려고하는 Rect 메쉬 세팅해주기
	renderer->Set_Mesh(mesh);					// 그릴 것을 렌더러에 추가하기 

	// For. 
	auto material = make_shared<Material>();
	material->Set_TextureMap(RESOURCES.Load<Texture>(L"Test", L"../Resources/Textures/Test.dds"), TextureMapType::DIFFUSE);
	renderer->Get_RenderParamDesc().SetVec4(0, _float4(1));
	renderer->Set_Material(material);
	renderer->Set_Pass(MeshRenderer::PASS_INFO::Default_UI);

	// For. 생성한 게임 오브젝트에 렌더러 컴포넌트를 붙이기 
	LoadingBackGround->Add_Component(renderer);

	// For. 생성한 게임 오브젝트의 정보 채우기 ( 혹시 해당 게임오브젝트를 꺼내온다거나 할일이 있을 수 있으니 )
	LoadingBackGround->Set_Name(L"LoadingBG");
	LoadingBackGround->Set_LayerIndex(Layer_UI);
	LoadingBackGround->Set_Instancing(false);

	// For. 생성한 게임오브젝트를 씬에 추가하기 
	Add_GameObject(LoadingBackGround);
}

