#pragma once
#include "pch.h"

class Widget_ParticleEffectTool;
class ImGuiToolMgr
{
	DECLARE_SINGLETON(ImGuiToolMgr)

public:
	~ImGuiToolMgr();

public:
	void	Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick();
	void	Render();
	
private:
	void	ImGui_BasicWidget();

private:
	/* Basic Variables */
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	HWND					m_hWnd;
	LEVEL					m_eCurrLevel = { LEVEL_END };

	/* ImGui Variables */
	_bool					m_bIsParticleEffectTool_On = { false };
	_bool					m_bIsMeshEffectTool_On = { false };

	/* Widgets */
	shared_ptr<Widget_ParticleEffectTool>	m_pWidget_ParticleEffectTool = { nullptr };
};

