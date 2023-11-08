#pragma once


class GameInstance
{
	DECLARE_SINGLETON(GameInstance)

public: //Getter. Setter
	_float2 Get_WindowSize() { return _float2(_float(m_iWINCX), _float(m_iWINCY)); }

public: // for Engine
	HRESULT Initialize_Engine(_uint iNumLevels, const GRAPHICDESC& GraphicDesc);
	void	Tick_Engine();

public: // for Graphics
	HRESULT Render_Begin();
	HRESULT Render();
	HRESULT Render_End();

private:
	_uint	m_iWINCX		= { 0 };
	_uint	m_iWINCY		= { 0 };
	Color	m_vClearColor	= { 0.5f,0.5f,0.5f,1.f };

public:
	static _float g_fBrightness;
	static _float g_fContrast;
	static _float g_fShadowBias;
	static _float g_fMaxWhite;
	static _float g_fGamma;
	static _float g_fBloomMin;
	static _int g_iTMIndex;

	struct SSAOData
	{
		_bool g_bSSAO_On = true;
		_float g_fOcclusionRadius = 0.5f;
		_float g_OcclusionFadeStart = 0.2f;
		_float g_OcclusionFadeEnd = 2.0f;

	};
	static SSAOData g_SSAOData;

	struct FogData
	{
		_bool g_FogOn = false;
		_float  gFogStart = 15.f;
		_float  gFogRange = 150.f;
		Color	gFogColor = Color(1.f);
	};
	static FogData g_FogData;

	static _bool g_bFXAAOn;
	static _bool g_bAberrationOn;
	static _float g_fAberrationPower;
	static _bool g_bLensFlare;
};

