#include "pch.h"
#include "MainApp.h"

/* Scenes */
#include "LoadingScene.h"
#include "DemoScene.h"

#include "ImGuiToolMgr.h"

MainApp::MainApp()
{
}

HRESULT MainApp::Init()
{
	// For. Using Random Functions 
	srand(static_cast<_uint>(time(nullptr)));

	{
		// For. 기본적인 그래픽 정보 채워주기
		GRAPHICDESC			GraphicDesc{};	
		GraphicDesc.hWnd = g_hWnd;
		GraphicDesc.iSizeX = g_iWinSizeX;
		GraphicDesc.iSizeY = g_iWinSizeY;
		GraphicDesc.eWinMode = GRAPHICDESC::WINMODE_WIN;
		GraphicDesc.vClearColor = { 0.5f,0.5f,0.5f,1.f };
		
		// For. 위에서 그래픽정보로 지정한 정보로 엔진 init하기 (엔진헤더와 그 외 프로젝트의 헤더가 각각 그래픽 정보를 보관하는 일이 없도록하기 위함)	
			// GAMEINSTANCE 이런거 다 함수를 디파인한거임. 싱글턴 클래스 바로 사용할 수 있도록 
		if (FAILED(GAMEINSTANCE.Initialize_Engine(LEVEL_END, GraphicDesc)))
		{
			MSG_BOX("Engine Init Failed : Initialize_Engine");
			return E_FAIL;
		}
	}

	// For. 씬 열기 
	Open_Scene();

	// For. Off AASO ( for save frame)


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
	// For. 씬 객체 생성 (스마트 포인터로)
	shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<GranseedScene>());

	// For. 위에서 생성한 씬으로 씬 변경하기 
	SCENE.Change_Scene(scene);

	return S_OK;

}