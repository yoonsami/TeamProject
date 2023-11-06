#pragma once
#include "pch.h"

class Widget_EffectMaker_Mesh;
class Widget_ParticleMaker_Instancing;
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
	_bool					m_bIsParticleMaker_Instancing_On = { false };
	_bool					m_bIsEffectMaker_Mesh_On = { false };

	/* Widgets */
	shared_ptr<Widget_ParticleMaker_Instancing>	m_pWidget_ParticleMaker_Instancing = { nullptr };
	shared_ptr<Widget_EffectMaker_Mesh>			m_pWidget_EffectMaker_Mesh = { nullptr };
};

