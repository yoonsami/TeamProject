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
		// For. �⺻���� �׷��� ���� ä���ֱ�
		GRAPHICDESC			GraphicDesc{};	
		GraphicDesc.hWnd = g_hWnd;
		GraphicDesc.iSizeX = g_iWinSizeX;
		GraphicDesc.iSizeY = g_iWinSizeY;
		GraphicDesc.eWinMode = GRAPHICDESC::WINMODE_WIN;
		GraphicDesc.vClearColor = { 0.5f,0.5f,0.5f,1.f };
		
		// For. ������ �׷��������� ������ ������ ���� init�ϱ� (��������� �� �� ������Ʈ�� ����� ���� �׷��� ������ �����ϴ� ���� �������ϱ� ����)	
			// GAMEINSTANCE �̷��� �� �Լ��� �������Ѱ���. �̱��� Ŭ���� �ٷ� ����� �� �ֵ��� 
		if (FAILED(GAMEINSTANCE.Initialize_Engine(LEVEL_END, GraphicDesc)))
		{
			MSG_BOX("Engine Init Failed : Initialize_Engine");
			return E_FAIL;
		}
	}

	// For. Initialize ImGui Tool mgr
	IMGUITOOL_MGR.Init(DEVICE.Get(), CONTEXT.Get());

	// For. �� ���� 
	Open_Scene();

	return S_OK;
}

void MainApp::Tick()
{
	GAMEINSTANCE.Tick_Engine();

	IMGUITOOL_MGR.Tick();
}

HRESULT MainApp::Render()
{
	GAMEINSTANCE.Render_Begin();
	SCENE.Render();
	IMGUITOOL_MGR.Render();
	GAMEINSTANCE.Render_End();

	return S_OK;
}

HRESULT MainApp::Open_Scene()
{
	// For. �� ��ü ���� (����Ʈ �����ͷ�)
	shared_ptr<LoadingScene> scene = make_shared<LoadingScene>(make_shared<DemoScene>());

	// For. ������ ������ ������ �� �����ϱ� 
	SCENE.Change_Scene(scene);

	return S_OK;

}