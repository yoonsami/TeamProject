#pragma once
class TextureBuffer
{
public:
	TextureBuffer(ComPtr<ID3D11Texture2D> src);
	~TextureBuffer();

public:
	void Create_Buffer();

private: 
	void Create_SRV();
	void Create_Output();
	void Create_UAV();
	void Create_Result();

public:
	ComPtr<ID3D11ShaderResourceView> Get_SRV() { return m_pSRV; }
	ComPtr<ID3D11UnorderedAccessView> Get_UAV() { return m_pUAV; }
	_uint Get_Width() { return m_iWidth; }
	_uint Get_Height() { return m_iHeight; }
	_uint Get_ArraySize() { return m_iArraySize; }

	ComPtr<ID3D11Texture2D> Get_Output() { return (ID3D11Texture2D*)m_pOutputSRV.Get(); }
	ComPtr<ID3D11ShaderResourceView> Get_OutputSRV() { return m_pOutputSRV; }

private:
	ComPtr<ID3D11Texture2D> m_pInput;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
	ComPtr<ID3D11Texture2D> m_pOutput;
	ComPtr<ID3D11UnorderedAccessView> m_pUAV;

private:
	_uint m_iWidth = 0;
	_uint m_iHeight = 0;
	_uint m_iArraySize = 0;
	DXGI_FORMAT m_eFormat{};
	ComPtr<ID3D11ShaderResourceView> m_pOutputSRV;
};

