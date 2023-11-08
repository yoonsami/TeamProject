#include "pch.h"
#include "MainApp.h"

#include "Utils.h"
#include "DemoScene.h"
#include "LogoScene.h"
#include "LoadingScene.h"

#include <filesystem>
namespace fs = std::filesystem;

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

	Load_Ui();
	Open_Scene();
#ifdef _DEBUGTOOL
	DEBUGTOOL.Init();
#endif
    return S_OK;
}

void MainApp::Tick()
{
	GAMEINSTANCE.Tick_Engine();
#ifdef _DEBUGTOOL
DEBUGTOOL.Tick();
#endif // _DEBUGTOOL
	Control_Option();
}

HRESULT MainApp::Render()
{
	GAMEINSTANCE.Render_Begin();
	SCENE.Render();
#ifdef _DEBUGTOOL
	DEBUGTOOL.Render();
#endif // _DEBUGTOOL

	GAMEINSTANCE.Render_End();

	return S_OK;
}

HRESULT MainApp::Open_Scene()
{
	shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<LogoScene>());

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
		GAMEINSTANCE.g_fMaxWhite += 0.1f * fDT;
		if (GAMEINSTANCE.g_fMaxWhite > 2.f) GAMEINSTANCE.g_fMaxWhite = 2.f;
	}
	if (KEYPUSH(KEY_TYPE::X))
	{
		GAMEINSTANCE.g_fMaxWhite -= 0.1f * fDT;
		if (GAMEINSTANCE.g_fMaxWhite < 0.f) GAMEINSTANCE.g_fMaxWhite = 0.1f;
	}
	if (KEYTAP(KEY_TYPE::C))
	{
		GAMEINSTANCE.g_iTMIndex++;
		if (GAMEINSTANCE.g_iTMIndex > 3) GAMEINSTANCE.g_iTMIndex %= 3;
	}
	if (KEYTAP(KEY_TYPE::V))
	{
		GAMEINSTANCE.g_iTMIndex--;
		if (GAMEINSTANCE.g_iTMIndex == 0) GAMEINSTANCE.g_iTMIndex = 3;
	}
}

void MainApp::Load_Ui()
{
	wstring assetPath = L"..\\Resources\\Textures\\UITexture\\Loading\\";

	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		wstring filePath = entry.path().wstring();
		wstring fileName = entry.path().filename().wstring();
		Utils::DetachExt(fileName);
		RESOURCES.Load<Texture>(fileName, filePath);
	}
}
