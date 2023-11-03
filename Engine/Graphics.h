#pragma once
#include "Viewport.h"
#include "RenderTargetGroup.h"

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 1
};

class Graphics
{
	DECLARE_SINGLETON(Graphics)

public:
	ComPtr<ID3D11Device>		Get_Device()	{ return m_pDevice; }
	ComPtr<ID3D11DeviceContext> Get_Context()	{ return m_pContext; }

public:
	HRESULT Initialize(GRAPHICDESC desc);
	HRESULT Render_Begin();
	HRESULT Render_End();

	Viewport& Get_ViewPort() { return m_Viewport; }

private:

private:
	ComPtr<IDXGIFactory>			m_pDXGI = nullptr;
	ComPtr<ID3D11Device>			m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>		m_pContext = nullptr;
	ComPtr<IDXGISwapChain>			m_pSwapChain = nullptr;

	Viewport						m_Viewport = {};

private:
	HRESULT Ready_DeviceAndSwapChain(HWND hWnd, GRAPHICDESC::WINMODE eWinmode, _uint iWinCX, _uint iWinCY);

	void Set_ViewPort(_uint iWinCX, _uint iWinCY);


	// RenderTargetGroup
public:
	shared_ptr<RenderTargetGroup> Get_RTGroup(RENDER_TARGET_GROUP_TYPE type) { return m_RTGroup[static_cast<_uint>(type)]; }


private:
	void Create_RTGroup();
	array<shared_ptr<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> m_RTGroup;
};