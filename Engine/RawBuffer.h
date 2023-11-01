#pragma once
class RawBuffer
{
	
public:
	RawBuffer(void* inputData, _uint inputByte, _uint outputByte);
	~RawBuffer();

public:
	void Create_Buffer();
	void Copy_ToInput(void* data);
	void Copy_FromOutput(void* data);
	ComPtr<ID3D11ShaderResourceView> Get_SRV() { return m_pSRV; }
	ComPtr<ID3D11UnorderedAccessView> Get_UAV() { return m_pUAV; }

private:
	void Create_Input();
	void Create_SRV();
	void Create_Output();
	void Create_UAV();
	void Create_Result();

private:
	ComPtr<ID3D11Buffer> m_pInput;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
	ComPtr<ID3D11Buffer> m_pOutput;
	ComPtr<ID3D11UnorderedAccessView> m_pUAV;
	ComPtr<ID3D11Buffer> m_pResult;

 
private:
	void* m_pInputData = nullptr;
	_uint m_iInputByte = 0;
	_uint m_iOutputByte = 0;

};

