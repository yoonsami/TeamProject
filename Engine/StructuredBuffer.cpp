#include "pch.h"
#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(void* inputData, _uint inputStride, _uint inputCount, _uint outputStride, _uint outputCount)
	: m_pInputData(inputData)
	, m_iInputStride(inputStride), m_iInputCount(inputCount)
	, m_iOutputStride(outputStride), m_iOutputCount(outputCount)
{
	if (outputStride == 0 || outputCount == 0)
	{
		m_iOutputCount = inputCount;
		m_iOutputStride = inputStride;
	}

	Create_Buffer();
}

StructuredBuffer::~StructuredBuffer()
{
	int a = 0;
}

void StructuredBuffer::Create_Buffer()
{
	Create_Input();
	Create_SRV();
	Create_Output();
	Create_UAV();
	Create_Result();
}

void StructuredBuffer::Copy_ToInput(void* data) 
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	CONTEXT->Map(m_pInput.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

	memcpy(subResource.pData, data, Get_InputByteWidth());

	CONTEXT->Unmap(m_pInput.Get(), 0);
}

void StructuredBuffer::Copy_FromOutput(void* data)
{
	CONTEXT->CopyResource(m_pResult.Get(), m_pOutput.Get());

	D3D11_MAPPED_SUBRESOURCE subResource;

	CONTEXT->Map(m_pResult.Get(), 0, D3D11_MAP_READ, 0, &subResource);

	memcpy(data, subResource.pData, Get_OutputByteWidth());

	CONTEXT->Unmap(m_pResult.Get(), 0);
}

void StructuredBuffer::Create_Input()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof desc);

	desc.ByteWidth = Get_InputByteWidth();
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = m_iInputStride;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = m_pInputData;

	if (m_pInputData)
		(DEVICE->CreateBuffer(&desc, &subResource, m_pInput.GetAddressOf()));
	else
		(DEVICE->CreateBuffer(&desc, nullptr, m_pInput.GetAddressOf()));
}

void StructuredBuffer::Create_SRV()
{
	D3D11_BUFFER_DESC desc;
	m_pInput->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof srvDesc);
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.NumElements = m_iInputCount;

	(DEVICE->CreateShaderResourceView(m_pInput.Get(), &srvDesc, m_pSRV.GetAddressOf()));

}

void StructuredBuffer::Create_Output()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof desc);
	desc.ByteWidth = Get_OutputByteWidth();
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = m_iOutputStride;

	(DEVICE->CreateBuffer(&desc, nullptr, m_pOutput.GetAddressOf()));
}

void StructuredBuffer::Create_UAV()
{
	D3D11_BUFFER_DESC desc;
	m_pOutput->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof uavDesc);
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = m_iOutputCount;

	(DEVICE->CreateUnorderedAccessView(m_pOutput.Get(), &uavDesc, m_pUAV.GetAddressOf()));

}

void StructuredBuffer::Create_Result()
{
	D3D11_BUFFER_DESC desc;
	m_pOutput->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	(DEVICE->CreateBuffer(&desc, nullptr, m_pResult.GetAddressOf()));
}
