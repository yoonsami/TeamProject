#pragma once
#include "ResourceBase.h"


class Texture : public ResourceBase
{
public:
	Texture();
	~Texture();

	ComPtr<ID3D11ShaderResourceView> Get_SRV() { return m_pShaderResourceView; }
	
	ComPtr<ID3D11Texture2D> Get_Texture2D() { return m_pTexture; }
	ComPtr<ID3D11RenderTargetView> Get_RTV() { return m_pRTV; }
	ComPtr<ID3D11DepthStencilView> Get_DST() { return m_pDSV; }
	ComPtr<ID3D11UnorderedAccessView> Get_UAV() { return m_pUAV; }

	virtual void Load(const wstring& path) override;

	void Create(DXGI_FORMAT format, _uint width, _uint height, _uint BindFlags, _float4 clearColor);
	void CreateFromResource(ComPtr<ID3D11Texture2D> tex2D);
	void CreateShadowDST(_float _width, _float _height);
	_float2 Get_Size() { return m_Size; }

	const ScratchImage& Get_Info() { return m_Img; }
	wstring& Get_FilePath() { return m_strFilePath; }

private:
	_float2 m_Size = { 0.f,0.f };
	ScratchImage m_Img{};

private:
	wstring m_strFilePath = L"";
	ComPtr<ID3D11Texture2D> m_pTexture;
	D3D11_TEXTURE2D_DESC m_TextureDesc{};
	ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	ComPtr<ID3D11RenderTargetView> m_pRTV;
	ComPtr<ID3D11DepthStencilView> m_pDSV;
	ComPtr<ID3D11UnorderedAccessView> m_pUAV;
};
