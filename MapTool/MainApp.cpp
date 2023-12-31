#include "pch.h"
#include "MainApp.h"
#include "LoadingScene.h"
#include "DemoScene.h"
#include "imgui_Manager.h"

MainApp::MainApp()
{
}

MainApp::~MainApp()
{
}

HRESULT MainApp::Init()
{
    srand(static_cast<_uint>(time(nullptr)));
	{
		GRAPHICDESC			GraphicDesc{};

		GraphicDesc.hWnd = g_hWnd;
		GraphicDesc.iSizeX = g_iWinSizeX;
		GraphicDesc.iSizeY = g_iWinSizeY;
		GraphicDesc.eWinMode = GRAPHICDESC::WINMODE_WIN;
		GraphicDesc.vClearColor = { 0.5f,0.5f,0.5f,1.f };
		if (FAILED(GAMEINSTANCE.Initialize_Engine(LEVEL_END, GraphicDesc)))
		{
			MSG_BOX("Engine Init Failed : Initialize_Engine");
			return E_FAIL;
		}
	}

	Open_Scene();

    return S_OK;
}

void MainApp::Tick()
{
	GAMEINSTANCE.Tick_Engine();
}

HRESULT MainApp::Render()
{
	GAMEINSTANCE.Render_Begin();
	SCENE.Render();
	GAMEINSTANCE.Render_End();
	return S_OK;
}

HRESULT MainApp::Open_Scene()
{
	shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<GranseedScene>());

	SCENE.Change_Scene(scene);

	return S_OK;

}
