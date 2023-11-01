#include "pch.h"
#include "RawBuffer.h"

RawBuffer::RawBuffer(void* inputData, _uint inputByte, _uint outputByte)
	:m_pInputData(inputData), m_iInputByte(inputByte),m_iOutputByte(outputByte)
{
	Create_Buffer();
}

RawBuffer::~RawBuffer()
{
}

void RawBuffer::Create_Buffer()
{
	Create_Input();
	Create_SRV();
	Create_Output();
	Create_UAV();
	Create_Result();
}

void RawBuffer::Copy_ToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	CONTEXT->Map(m_pInput.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

	memcpy(subResource.pData, data, m_iInputByte);

	CONTEXT->Unmap(m_pInput.Get(), 0);
}

void RawBuffer::Copy_FromOutput(void* data)
{
	CONTEXT->CopyResource(m_pResult.Get(), m_pOutput.Get());

	D3D11_MAPPED_SUBRESOURCE subResource;

	CONTEXT->Map(m_pResult.Get(), 0, D3D11_MAP_READ, 0, &subResource);

	memcpy(data, subResource.pData, m_iOutputByte);

	CONTEXT->Unmap(m_pResult.Get(), 0);
}

void RawBuffer::Create_Input()
{
	if (m_iInputByte == 0)
		return;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof desc);
	desc.ByteWidth = m_iInputByte;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResource = {0};
	subResource.pSysMem = m_pInputData;

	if (m_pInputData != nullptr)
		(DEVICE->CreateBuffer(&desc, &subResource, m_pInput.GetAddressOf()));
	else
		(DEVICE->CreateBuffer(&desc, nullptr, m_pInput.GetAddressOf()));
}

void RawBuffer::Create_SRV()
{
	if (m_iInputByte == 0)
		return;

	D3D11_BUFFER_DESC desc;
	m_pInput->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof srvDesc);
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	srvDesc.BufferEx.NumElements = desc.ByteWidth / 4;

	(DEVICE->CreateShaderResourceView(m_pInput.Get(), &srvDesc, m_pSRV.GetAddressOf()));
}

void RawBuffer::Create_Output()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof desc);
	desc.ByteWidth = m_iOutputByte;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	(DEVICE->CreateBuffer(&desc, NULL, m_pOutput.GetAddressOf()));
}

void RawBuffer::Create_UAV()
{
	D3D11_BUFFER_DESC desc;
	m_pOutput->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof uavDesc);
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = desc.ByteWidth / 4;

	(DEVICE->CreateUnorderedAccessView(m_pOutput.Get(), &uavDesc, m_pUAV.GetAddressOf()));
}

void RawBuffer::Create_Result()
{
	D3D11_BUFFER_DESC desc;
	m_pOutput->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = D3D11_USAGE_DEFAULT;
	desc.MiscFlags = 0;
	
	(DEVICE->CreateBuffer(&desc, nullptr, m_pResult.GetAddressOf()));
}
