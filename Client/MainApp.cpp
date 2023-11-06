#include "pch.h"
#include "MainApp.h"

#include "LoadingScene.h"
#include "DemoScene.h"

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
	Control_Option();
}

HRESULT MainApp::Render()
{
	GAMEINSTANCE.Render();

	return S_OK;
}

HRESULT MainApp::Open_Scene()
{
	shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<DemoScene>());

	SCENE.Change_Scene(scene);

	return S_OK;

}

void MainApp::Control_Option()
{
	/*if (KEYPUSH(KEY_TYPE::Z))
	{
		GAMEINSTANCE.g_fBrightness += 0.5f * fDT;
		if (GAMEINSTANCE.g_fBrightness > 2.f) GAMEINSTANCE.g_fBrightness = 2.f;
	}
	if (KEYPUSH(KEY_TYPE::X))
	{
		GAMEINSTANCE.g_fBrightness -= 0.5f * fDT;
		if (GAMEINSTANCE.g_fBrightness < 0.1f) GAMEINSTANCE.g_fBrightness = 0.1f;
	}*/
	if (KEYPUSH(KEY_TYPE::Z))
	{
		GAMEINSTANCE.g_fShadowBias += 0.0001f * fDT;
		if (GAMEINSTANCE.g_fShadowBias > 0.01f) GAMEINSTANCE.g_fShadowBias = 0.01f;
	}
	if (KEYPUSH(KEY_TYPE::X))
	{
		GAMEINSTANCE.g_fShadowBias -= 0.0001f * fDT;
		if (GAMEINSTANCE.g_fShadowBias < 0.f) GAMEINSTANCE.g_fShadowBias = 0.000001f;
	}
	if (KEYPUSH(KEY_TYPE::C))
	{
		GAMEINSTANCE.g_fContrast += 0.5f * fDT;
		if (GAMEINSTANCE.g_fContrast > 2.f) GAMEINSTANCE.g_fContrast = 2.f;
	}
	if (KEYPUSH(KEY_TYPE::V))
	{
		GAMEINSTANCE.g_fContrast -= 0.5f * fDT;
		if (GAMEINSTANCE.g_fContrast < 0.1f) GAMEINSTANCE.g_fContrast = 0.1f;
	}
}
