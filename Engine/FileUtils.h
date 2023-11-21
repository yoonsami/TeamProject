#pragma once

enum class FileMode : _uchar
{
	Write,
	Read
};

class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	void Open(wstring filePath, FileMode mode);

	template<typename T>
	void Write(const T& data)
	{
		DWORD numOfByte = 0;
		(::WriteFile(m_Handle, &data, sizeof(T), (LPDWORD)&numOfByte, nullptr));
	}

	template<>
	void Write<string>(const string& data)
	{
		return Write(data);
	}
	template<>
	void Write<wstring>(const wstring& data)
	{
		return Write(data);
	}

	void Write(void* data, _uint dataSize);
	void Write(const string& data);
	void Write(const wstring& data);

	template<typename T>
	void Read(OUT T& data)
	{
		DWORD numOfByte = 0;
		(::ReadFile(m_Handle, &data, sizeof(T), (LPDWORD)&numOfByte, nullptr));
	}

	template<typename T>
	T Read()
	{
		T data;
		Read(data);
		return data;
	}
	void Read(void** data, _uint dataSize);
	void Read(OUT string& data);


private:
	HANDLE m_Handle = INVALID_HANDLE_VALUE;
};

