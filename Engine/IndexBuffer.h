#pragma once

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

public:
	ComPtr<ID3D11Buffer> Get_ComPtr() { return m_IndexBuffer; }
	_uint Get_Stride() { return m_iIndexStride; }
	_uint Get_IndicesNum() { return m_iNumIndices; }
	_uint Get_Offset() { return m_iOffset; }
public:
	HRESULT Create(const vector<_uint>& indices, _uint iOffset = 0);

	void Push_Data()
	{
		CONTEXT->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

private:

	ComPtr<ID3D11Buffer> m_IndexBuffer;

	_uint m_iIndexStride = 0;
	_uint m_iOffset = 0;
	_uint m_iNumIndices = 0;
};
