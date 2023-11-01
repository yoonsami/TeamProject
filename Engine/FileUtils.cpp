#include "pch.h"
#include "FileUtils.h"

FileUtils::FileUtils()
{
}

FileUtils::~FileUtils()
{
	if (m_Handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
	}
}

void FileUtils::Open(wstring filePath, FileMode mode)
{
	if (mode == FileMode::Write)
	{
		m_Handle = ::CreateFile(
			filePath.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}
	else
	{
		m_Handle = ::CreateFile(
			filePath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}

	if (m_Handle == INVALID_HANDLE_VALUE)
		MSG_BOX("FileOpen Failed");
}

void FileUtils::Write(void* data, _uint dataSize)
{
	DWORD numOfByte = 0;
	(::WriteFile(m_Handle, data, dataSize, (LPDWORD)&numOfByte, nullptr));

}

void FileUtils::Write(const string& data)
{
	_uint size = (_uint)data.size();
	Write(size);

	if (data.size() == 0)
		return;

	Write((void*)data.data(), size);
}

void FileUtils::Read(void** data, _uint dataSize)
{
	DWORD numOfByte = 0;
	(::ReadFile(m_Handle, *data, dataSize, (LPDWORD)&numOfByte, nullptr));

}

void FileUtils::Read(OUT string& data)
{
	_uint size = Read<_uint>();

	if (size == 0)
		return;

	char* tmp = new char[size + 1];
	tmp[size] = 0;
	Read((void**)&tmp, size);
	data = tmp;
	delete[] tmp;
}


