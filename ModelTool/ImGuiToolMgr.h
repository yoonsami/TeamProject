#pragma once
#include "pch.h"

class Widget_Model_Controller;

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

private:
	/* Basic Variables */
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	HWND					m_hWnd;
	LEVEL					m_eCurrLevel = { LEVEL_END };

	/* ImGui Variables */


	/* Widgets */

	shared_ptr<Widget_Model_Controller>			m_pWidget_Model_Controller = { nullptr };

};

