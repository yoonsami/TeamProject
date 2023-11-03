#include "pch.h"
#include "Graphics.h"

HRESULT Graphics::Initialize(GRAPHICDESC desc)
{
	if (FAILED(Ready_DeviceAndSwapChain(desc.hWnd, desc.eWinMode, desc.iSizeX, desc.iSizeY)))
	{
		MSG_BOX("Failed to Ready_DeviceAndSwapChain : Graphics ");
		return E_FAIL;
	}

	Set_ViewPort(desc.iSizeX, desc.iSizeY);
	
	Create_RTGroup();

	return S_OK;
}

HRESULT Graphics::Render_Begin()
{
	if (!m_pContext)
		return E_FAIL;

	Get_RTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets();

	for (_uchar i = 0; i < _uchar(RENDER_TARGET_GROUP_TYPE::END); ++i)
	{
		Get_RTGroup(RENDER_TARGET_GROUP_TYPE(i))->ClearRenderTargetView();
	}

	m_Viewport.RSSetViewport();

	return S_OK;
}

HRESULT Graphics::Render_End()
{
	if (!m_pSwapChain)
		return E_FAIL;


	HRESULT hr = m_pSwapChain->Present(1, 0);
	m_pContext->ClearState();

	return hr;
}

HRESULT Graphics::Ready_DeviceAndSwapChain(HWND hWnd, GRAPHICDESC::WINMODE eWinmode, _uint iWinCX, _uint iWinCY)
{
	DXGI_SWAP_CHAIN_DESC    SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	SwapChainDesc.BufferDesc.Width = iWinCX;
	SwapChainDesc.BufferDesc.Height = iWinCY;
	/*스왑하는 형태*/
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.Windowed = eWinmode;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	_uint iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION, &SwapChainDesc, m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), nullptr, m_pContext.GetAddressOf())))
	{
		MSG_BOX("Failed to D3D11CreateDeviceAndSwapChain");
		return E_FAIL;
	}

	return S_OK;
}

void Graphics::Set_ViewPort(_uint iWinCX, _uint iWinCY)
{
	m_Viewport.Set(_float(iWinCX), _float(iWinCY));
}

void Graphics::Create_RTGroup()
{
	//
	shared_ptr<Texture> dsTexture = RESOURCES.CreateTexture(L"DepthStencil", DXGI_FORMAT_D24_UNORM_S8_UINT, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL, _float4(0.f));

	//SwapChain
	{
		vector<RenderTarget> rtVec(SWAP_CHAIN_BUFFER_COUNT);
		for (_uint i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		{
			wstring name = L"SwapChainTarget_" + to_wstring(i);

			ComPtr<ID3D11Texture2D> resource;
			m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&resource));
			rtVec[i].target = RESOURCES.CreateTextureFromResource(name, resource);
			rtVec[i].clearColor = _float4(0.5f, 0.5f, 0.5f, 1.f);
		}
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	//Shadow
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_SHADOW_GROUP_MEMBER_COUNT);

		rtVec[0].target = RESOURCES.CreateTexture(L"ShadowTarget",
			DXGI_FORMAT_R32_FLOAT, 8192, 8192,
			D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET
			| D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[0].clearColor = _float4(1.f);

		//rtVec[0].target = nullptr;

		/*shared_ptr<Texture> shadowDepthTexture
			= RESOURCES.CreateTexture(L"ShadowDepthStencil",
				DXGI_FORMAT_D32_FLOAT, 8192, 8192,
				D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(1.f));
		rtVec[0].clearColor = _float4(0.f);*/
		shared_ptr<Texture> shadowDepthTexture = RESOURCES.CreateShadowTexture(L"ShadowDepthStencil", 8192, 8192);


		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::SHADOW)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::SHADOW)]->Create(RENDER_TARGET_GROUP_TYPE::SHADOW, rtVec, shadowDepthTexture);
	}

	//Deferred
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);
		rtVec[0].target = RESOURCES.CreateTexture(L"PositionTarget", DXGI_FORMAT_R32G32B32A32_FLOAT, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[0].clearColor = _float4(0.f);

		rtVec[1].target = RESOURCES.CreateTexture(L"NormalTarget", DXGI_FORMAT_R32G32B32A32_FLOAT, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[1].clearColor = _float4(0.f);

		rtVec[2].target = RESOURCES.CreateTexture(L"AmbientTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[2].clearColor = _float4(0.f);

		rtVec[3].target = RESOURCES.CreateTexture(L"DiffuseTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[3].clearColor = _float4(0.f);

		rtVec[4].target = RESOURCES.CreateTexture(L"SpecularTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[4].clearColor = _float4(0.f);

		rtVec[5].target = RESOURCES.CreateTexture(L"EmissiveTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[5].clearColor = _float4(0.f);

		rtVec[6].target = RESOURCES.CreateTexture(L"BlurTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[6].clearColor = _float4(0.f);

		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}

	//BLURSMALLER0
	{
		_float ratio = 0.5f;
		vector<RenderTarget> rtVec(RENDER_TARGET_BLUR_GROUP_MEMBER_COUNT);
		shared_ptr<Texture> dsTexture0 = RESOURCES.CreateTexture(L"BLURSMALLER0DS", DXGI_FORMAT_D24_UNORM_S8_UINT, _uint(m_Viewport.Get_Width() * ratio), _uint(m_Viewport.Get_Height() * ratio), D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL, _float4(0.f));

		rtVec[0].target = RESOURCES.CreateTexture(L"BLURSMALLER0", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width() * ratio), _uint(m_Viewport.Get_Height() * ratio), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[0].clearColor = _float4(0.f);

		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURSMALLER0)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURSMALLER0)]->Create(RENDER_TARGET_GROUP_TYPE::BLURSMALLER0, rtVec, dsTexture0);
	}

	//BLURSMALLER1
	{
		_float ratio = 0.25f;
		vector<RenderTarget> rtVec(RENDER_TARGET_BLUR_GROUP_MEMBER_COUNT);
		shared_ptr<Texture> dsTexture1 = RESOURCES.CreateTexture(L"BLURSMALLER1DS", DXGI_FORMAT_D24_UNORM_S8_UINT, _uint(m_Viewport.Get_Width() * ratio), _uint(m_Viewport.Get_Height() * ratio), D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL, _float4(0.f));

		rtVec[0].target = RESOURCES.CreateTexture(L"BLURSMALLER1", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width() * ratio), _uint(m_Viewport.Get_Height() * ratio), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[0].clearColor = _float4(0.f);

		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURSMALLER1)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURSMALLER1)]->Create(RENDER_TARGET_GROUP_TYPE::BLURSMALLER1, rtVec, dsTexture1);
	}

	//BLURBIGGER0
	{
		_float ratio = 0.5f;
		vector<RenderTarget> rtVec(RENDER_TARGET_BLUR_GROUP_MEMBER_COUNT);
		shared_ptr<Texture> dsTexture0 = RESOURCES.CreateTexture(L"BLURBIGGER0DS", DXGI_FORMAT_D24_UNORM_S8_UINT, _uint(m_Viewport.Get_Width() * ratio), _uint(m_Viewport.Get_Height() * ratio), D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL, _float4(0.f));

		rtVec[0].target = RESOURCES.CreateTexture(L"BLURBIGGER0", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width() * ratio), _uint(m_Viewport.Get_Height() * ratio), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[0].clearColor = _float4(0.f);

		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURBIGGER0)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURBIGGER0)]->Create(RENDER_TARGET_GROUP_TYPE::BLURBIGGER0, rtVec, dsTexture0);
	}

	//BLURBIGGER1
	{
		_float ratio = 1.f;
		vector<RenderTarget> rtVec(RENDER_TARGET_BLUR_GROUP_MEMBER_COUNT);
		shared_ptr<Texture> dsTexture1 = RESOURCES.CreateTexture(L"BLURBIGGER1DS", DXGI_FORMAT_D24_UNORM_S8_UINT, _uint(m_Viewport.Get_Width() * ratio), _uint(m_Viewport.Get_Height() * ratio), D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL, _float4(0.f));

		rtVec[0].target = RESOURCES.CreateTexture(L"BLURBIGGER1", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width() * ratio), _uint(m_Viewport.Get_Height() * ratio), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[0].clearColor = _float4(0.f);

		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURBIGGER1)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::BLURBIGGER1)]->Create(RENDER_TARGET_GROUP_TYPE::BLURBIGGER1, rtVec, dsTexture1);
	}

	//Light
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT);
		rtVec[0].target = RESOURCES.CreateTexture(L"AmbientLightTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[0].clearColor = _float4(0.f);
		rtVec[1].target = RESOURCES.CreateTexture(L"DiffuseLightTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[1].clearColor = _float4(0.f);
		rtVec[2].target = RESOURCES.CreateTexture(L"SpecularLightTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[2].clearColor = _float4(0.f);
		rtVec[3].target = RESOURCES.CreateTexture(L"EmissiveLightTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[3].clearColor = _float4(0.f);

		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::LIGHTING)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::LIGHTING)]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
	}

	//Final
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_FINAL_GROUP_MEMEBER_COUNT);
		rtVec[0].target = RESOURCES.CreateTexture(L"FinalTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[0].clearColor = _float4(0.f);

		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::FINAL)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::FINAL)]->Create(RENDER_TARGET_GROUP_TYPE::FINAL, rtVec, dsTexture);
	}

	//Distortion
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_DISTORTION_GROUP_MEMEBER_COUNT);
		rtVec[0].target = RESOURCES.CreateTexture(L"DistortionTarget", DXGI_FORMAT_R8G8B8A8_UNORM, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[0].clearColor = _float4(0.f);
		rtVec[1].target = RESOURCES.CreateTexture(L"DistortionTarget1", DXGI_FORMAT_R32G32B32A32_FLOAT, _uint(m_Viewport.Get_Width()), _uint(m_Viewport.Get_Height()), D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, _float4(0.f));
		rtVec[1].clearColor = _float4(0.f);
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::DISTORTION)] = make_shared<RenderTargetGroup>();
		m_RTGroup[static_cast<_uchar>(RENDER_TARGET_GROUP_TYPE::DISTORTION)]->Create(RENDER_TARGET_GROUP_TYPE::DISTORTION, rtVec, dsTexture);
	}

}
