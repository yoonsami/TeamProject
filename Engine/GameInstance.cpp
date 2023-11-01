#include "pch.h"
#include "GameInstance.h"

HRESULT GameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHICDESC& GraphicDesc)
{

	m_iWINCX = GraphicDesc.iSizeX;
	m_iWINCY = GraphicDesc.iSizeY;
	m_vClearColor = GraphicDesc.vClearColor;	
	GRAPHICS.Initialize(GraphicDesc);

	TIME.Initialize();
	INPUT.Initialize(GraphicDesc.hWnd);
	SOUND.Initialize();
	RESOURCES.Initialize();

	return S_OK;
}

void GameInstance::Tick_Engine()
{
	INPUT.Tick();
	TIME.Tick();
	SOUND.Tick();

	SCENE.Tick();

	COLLISION.Tick();

	SCENE.Late_Tick();


	SCENE.Final_Tick();

}

HRESULT GameInstance::Render_Begin()
{
	GRAPHICS.Render_Begin();
	return S_OK;
}

HRESULT GameInstance::Render()
{
	Render_Begin();

	SCENE.Render();
	Render_End();
	return S_OK;
}

HRESULT GameInstance::Render_End()
{
	GRAPHICS.Render_End();
	return S_OK;
}

