#pragma once

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

public:
	ComPtr<ID3D11Buffer>	Get_ComPtr() { return m_VertexBuffer; }
	_uint					Get_Stride() { return m_VertexStride; }
	_uint					Get_Offset() { return m_Offset; }
	_uint					Get_VerticesNum() { return m_NumVertices; }
	_uint					Get_Slot() { return m_iSlot; }

public:
	template<typename T>
	HRESULT Create(const vector<T>& vertices, _uint slot = 0, bool cpuWrite = false, bool gpuWrite = false)
	{
		m_VertexStride = sizeof(T);
		m_NumVertices = static_cast<_uint>(vertices.size());

		m_iSlot = slot;
		m_bCpuWrite = cpuWrite;
		m_bGpuWrite = gpuWrite;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = m_VertexStride * m_NumVertices;

		if (cpuWrite == false && gpuWrite == false)
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE; // CPU READ GPU READ
		}
		else if (cpuWrite == true && gpuWrite == false)
		{
			desc.Usage = D3D11_USAGE_DYNAMIC; // CPU WRITE GPU READ
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else if (cpuWrite == false && gpuWrite == true)
		{
			desc.Usage = D3D11_USAGE_DEFAULT; // CPU READ GPU WRITE
		}
		else
		{
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		}

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data();


		return DEVICE->CreateBuffer(&desc, &data, m_VertexBuffer.GetAddressOf());
	}

	void Push_Data()
	{
		CONTEXT->IASetVertexBuffers(m_iSlot, 1, m_VertexBuffer.GetAddressOf(), &m_VertexStride, &m_Offset);
	}

private:

	ComPtr<ID3D11Buffer> m_VertexBuffer;

	_uint m_VertexStride = 0;
	_uint m_Offset = 0;
	_uint m_NumVertices = 0;

	_uint m_iSlot = 0;
	_bool m_bCpuWrite = false;
	_bool m_bGpuWrite = false;
};