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
	HRESULT Render_End();

private:
	_uint	m_iWINCX		= { 0 };
	_uint	m_iWINCY		= { 0 };
	Color	m_vClearColor	= { 0.5f,0.5f,0.5f,1.f };

public:
	static _float g_fBrightness;
	static _float g_fContrast;
	static _float g_Saturation;
	static _float g_fShadowBias;
	static _float g_fMaxWhite;
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
		_float g_fogStart = 15.f;
		_float g_fogEnd = 150.f;
		_int g_fogMode = 0;
		_float g_fogDensity = 1.f;
		Color g_fogColor = Color(1.f);
	};
	static FogData g_FogData;

	static _bool g_bFXAAOn;
	static _bool g_bAberrationOn;
	static _float g_fAberrationPower;
	static _bool g_bLensFlare;

	static _float3 g_testVec1;
	static _float3 g_testVec2;

	static _bool g_bDrawOutline;

	struct MotionBlurData
	{
		_bool g_bMotionBlurOn = false;
		int g_iBlurCount = 0;
	};
	static MotionBlurData g_MotionBlurData;

	struct BloomData
	{
		_bool g_BloomOn = false;
		_float g_BloomMin = 0.99f;
	};
	static BloomData g_BloomData;


	struct LIGHTPowerData
	{
		_float g_specularPower = 10.f;
		_float g_rimPower = 10.f;
	};
	static LIGHTPowerData g_LightPowerData;


	static _bool g_bPBR_On;
	static _float g_lightAttenuation;
	static _float g_ambientRatio;

	struct DOFData
	{
		_bool g_bDOF_On = false;
		_float g_FocusDepth = 1.f;
		_float g_DOFRange = 5.f;
	};
	static DOFData g_DOFData;
};

