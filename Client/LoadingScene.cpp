#include "pch.h"
#include "LoadingScene.h"

#include "Utils.h"
#include "Loader.h"
#include "Camera.h"
#include "BaseUI.h"
#include "Material.h"
#include "DemoScene.h"
#include "GameObject.h"
#include "CustomFont.h"
#include "MeshRenderer.h"
#include "FontRenderer.h"
#include "UiLoadingScript.h"

#include <filesystem>
namespace fs = std::filesystem;

LoadingScene::LoadingScene(shared_ptr<Scene> nextScene)
    : m_pNextScene(nextScene)
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Init()
{
    {
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
        Add_GameObject(camera);
    }

    Load_Ui();

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
	
    //if (KEYTAP(KEY_TYPE::LBUTTON))
	{
		if (!m_pLoader->m_bLoadFinished)
			return;

		SCENE.Change_Scene(m_pNextScene);
	}
}

void LoadingScene::Load_Ui()
{
    Load_UIFile(L"..\\Resources\\UIData\\UI_Loading.dat");

    auto pGameobject = Get_GameObject(L"UI_Loading_Even");
    pGameobject->Add_Component(make_shared<UiLoadingScript>(true));

    pGameobject = Get_GameObject(L"UI_Loading_Odd");
    pGameobject->Add_Component(make_shared<UiLoadingScript>(false));
}
