#include "pch.h"
#include "TextureBuffer.h"

TextureBuffer::TextureBuffer(ComPtr<ID3D11Texture2D> src)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	m_iWidth = srcDesc.Width;
	m_iHeight = srcDesc.Height;
	m_iArraySize = srcDesc.ArraySize;
	m_eFormat = srcDesc.Format;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof desc);
	desc.Width = m_iWidth;
	desc.Height = m_iHeight;
	desc.ArraySize = m_iArraySize;
	desc.Format = m_eFormat;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	(DEVICE->CreateTexture2D(&desc, NULL, m_pInput.GetAddressOf()));

	CONTEXT->CopyResource(m_pInput.Get(), src.Get());

	Create_Buffer();
}

TextureBuffer::~TextureBuffer()
{
}

void TextureBuffer::Create_Buffer()
{
	Create_SRV();
	Create_Output();
	Create_UAV();
	Create_Result();
}

void TextureBuffer::Create_SRV()
{
	D3D11_TEXTURE2D_DESC desc;
	m_pInput->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof srvDesc);

	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = m_iArraySize;

	(DEVICE->CreateShaderResourceView(m_pInput.Get(), &srvDesc, m_pSRV.GetAddressOf()));


}

void TextureBuffer::Create_Output()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof desc);
	desc.Width = m_iWidth;
	desc.Height = m_iHeight;
	desc.ArraySize = m_iArraySize;
	desc.Format = m_eFormat;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	(DEVICE->CreateTexture2D(&desc, nullptr, m_pOutput.GetAddressOf()));
}

void TextureBuffer::Create_UAV()
{
	D3D11_TEXTURE2D_DESC desc;
	m_pOutput->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof uavDesc);
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.ArraySize = m_iArraySize;

	(DEVICE->CreateUnorderedAccessView(m_pOutput.Get(), &uavDesc, m_pUAV.GetAddressOf()));
}

void TextureBuffer::Create_Result()
{
	D3D11_TEXTURE2D_DESC desc;
	m_pOutput->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof srvDesc);
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = m_iArraySize;

	(DEVICE->CreateShaderResourceView(m_pOutput.Get(), &srvDesc, m_pOutputSRV.GetAddressOf()));

	
}
