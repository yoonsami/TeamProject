#include "pch.h"
#include "InstancingBuffer.h"

InstancingBuffer::InstancingBuffer()
{
	Create_Buffer();
}

InstancingBuffer::~InstancingBuffer()
{
}

void InstancingBuffer::Clear_Data()
{
	m_Data.clear();
}

void InstancingBuffer::Add_Data(InstancingData& data)
{
	m_Data.push_back(data);
}

void InstancingBuffer::Push_Data()
{
	const _uint dataCount = Get_Count();
	if (dataCount > m_iMaxCount)
		Create_Buffer(dataCount);

	D3D11_MAPPED_SUBRESOURCE subResrc;

	CONTEXT->Map(m_pInstanceBuffer->Get_ComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResrc);
	{
		::memcpy(subResrc.pData, m_Data.data(), sizeof(InstancingData) * dataCount);
	}
	CONTEXT->Unmap(m_pInstanceBuffer->Get_ComPtr().Get(), 0);
	
	m_pInstanceBuffer->Push_Data();
}

void InstancingBuffer::Create_Buffer(_uint maxCount)
{
	m_iMaxCount = maxCount;
	m_pInstanceBuffer = make_shared<VertexBuffer>();

	vector<InstancingData> temp(maxCount);
	m_pInstanceBuffer->Create(temp, 1/*slot*/, true);
}