#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::~IndexBuffer()
{
}

HRESULT IndexBuffer::Create(const vector<_uint>& indices, _uint iOffset)
{
	m_iOffset = iOffset;
	{
		m_iIndexStride = sizeof(_uint);
		m_iNumIndices = static_cast<_uint>(indices.size());

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = m_iIndexStride * m_iNumIndices;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = indices.data();

		return DEVICE->CreateBuffer(&desc, &data, m_IndexBuffer.GetAddressOf());
	}
}
