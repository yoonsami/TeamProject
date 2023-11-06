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
};

