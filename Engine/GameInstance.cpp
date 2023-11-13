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

HRESULT GameInstance::Render_End()
{
	if (CUR_SCENE)
		CUR_SCENE->Render_ToneMapping();

	GRAPHICS.Render_End();

	return S_OK;
}

_float GameInstance::g_fBrightness = 1.f;

_float GameInstance::g_fContrast = 1.f;

_float GameInstance::g_fShadowBias = _float(6.67628628e-05);

_float GameInstance::g_fMaxWhite = 1.f;

_int GameInstance::g_iTMIndex = 1;

_bool GameInstance::g_bFXAAOn = false;

_bool GameInstance::g_bAberrationOn = false;

_float GameInstance::g_fAberrationPower = 0.f;

_bool GameInstance::g_bLensFlare = false;

_float3 GameInstance::g_testVec1 = _float3(0.2f, 0.1f, 0.f);

_float3 GameInstance::g_testVec2 = _float3(0.9f, 1.f, 1.f);

_bool GameInstance::g_bDrawOutline = false;

_bool GameInstance::g_bPBR_On = true;

_float GameInstance::g_lightAttenuation = 100.f;

_float GameInstance::g_ambientRatio = 0.5f;

GameInstance::LIGHTPowerData GameInstance::g_LightPowerData{};

GameInstance::BloomData GameInstance::g_BloomData{};

GameInstance::MotionBlurData GameInstance::g_MotionBlurData{};

GameInstance::FogData GameInstance::g_FogData{};

GameInstance::SSAOData GameInstance::g_SSAOData{};