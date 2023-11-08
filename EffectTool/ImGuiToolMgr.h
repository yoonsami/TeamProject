#pragma once
#include "pch.h"

class Widget_EffectMaker_Mesh;
class Widget_ParticleMaker_Instancing;
class ImGuiToolMgr
{
	DECLARE_SINGLETON(ImGuiToolMgr)

public:
	typedef struct tagImGuiStyle
	{
		_float		fWindowBgAlpha = { 0.5f };
		_int		iTheme = { 0 };
		_float		fSeparatorTextPadding = 1.2f;
		_float		fFontSize = { 1.f };
	}IMGUI_STYLE;

public:
	~ImGuiToolMgr();

public:
	void	Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick();
	void	Render();

private:
	void	ImGui_BasicWidget();

	void	Option_ToolSelector();
	void	Option_StyleEditor();

private:
	/* Basic Variables */
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	HWND					m_hWnd;
	LEVEL					m_eCurrLevel = { LEVEL_END };

	/* ImGui Variables */
	_bool					m_bIsParticleMaker_Instancing_On = { false };
	_bool					m_bIsEffectMaker_Mesh_On = { false };

	/* Widgets */
	shared_ptr<Widget_ParticleMaker_Instancing>	m_pWidget_ParticleMaker_Instancing = { nullptr };
	shared_ptr<Widget_EffectMaker_Mesh>			m_pWidget_EffectMaker_Mesh = { nullptr };

	/* ImGui Window Option */
	IMGUI_STYLE				m_tImGuiStyle;
};

