#include "pch.h"
#include "Utils.h"
#include <filesystem>

namespace fs = std::filesystem;

bool Utils::StartsWith(string str, string comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

bool Utils::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

std::wstring Utils::ToWString(string s)
{
	_int len;
	_int slength = static_cast<_int>(s.length()) + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring ret(buf);
	delete[] buf;
	return ret;
}

std::string Utils::ToString(wstring s)
{
	_int len;
	_int slength = static_cast<_int>(s.length()) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	string r(len, '\0');
	::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

void Utils::ChangeExt(OUT string& srcPath, const string& ext)
{
	string preExt = fs::path(srcPath).extension().string();

	int extIndex = int(srcPath.rfind(preExt));
	srcPath = srcPath.substr(0, extIndex) + ext;
}

void Utils::ChangeExt(OUT wstring& srcPath, const wstring& ext)
{
	wstring preExt = fs::path(srcPath).extension().wstring();

	int extIndex = int(srcPath.rfind(preExt));
	srcPath = srcPath.substr(0, extIndex) + ext;
}

void Utils::DetachExt(OUT string& path)
{
	string preExt = fs::path(path).extension().string();

	int extIndex = int(path.rfind(preExt));
	path = path.substr(0, extIndex);
}

void Utils::DetachExt(OUT wstring& path)
{
	wstring preExt = fs::path(path).extension().wstring();

	int extIndex = int(path.rfind(preExt));
	path = path.substr(0, extIndex);
}

void Utils::Replace(OUT string& str, string comp, string rep)
{
	string temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

void Utils::Replace(OUT wstring& str, wstring comp, wstring rep)
{
	wstring temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

const _float4x4& Utils::Get_PivotMatrix()
{
	return m_matPivot;
}

_float4x4 Utils::m_matPivot = _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI);

const wstring Utils::m_strModelPath =  L"../Resources/Models/";;

const wstring Utils::m_strTexturePath = L"../Resources/Textures/";

const wstring Utils::m_strSceneDataPath = L"../Resources/Scene/";

const std::wstring Utils::m_strSoundDataPath = L"../Resources/Sound/";
