#pragma once

class  ConstantBuffer
{
public:
	ConstantBuffer();
	virtual ~ConstantBuffer();

	ComPtr<ID3D11Buffer> Get_ComPtr() { return m_pConstantBuffer; }

	template<typename T>
	HRESULT Create()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, m_pConstantBuffer.GetAddressOf())))
			return E_FAIL;

		return S_OK;
	}
	
	template<typename T>
	void CopyData(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		CONTEXT->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		::memcpy(subResource.pData, &data, sizeof(data));
		CONTEXT->Unmap(m_pConstantBuffer.Get(), 0);
	}

private:

	ComPtr<ID3D11Buffer> m_pConstantBuffer;
};
