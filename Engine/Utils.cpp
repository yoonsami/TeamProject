﻿#include "pch.h"
#include "Utils.h"

#include <random>
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

std::wstring Utils::ToWstringUtf8(string value)
{
	_int wideStrSize = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, NULL, 0);
	wstring wstr;
	
	if (wideStrSize > 0) {
		wstr.resize(wideStrSize - 1);
		MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, &wstr[0], wideStrSize);
	}

	return wstr;
}

std::string Utils::TostringUtf8(wstring value)
{
	_int strSize = WideCharToMultiByte(CP_UTF8, 0, value.c_str(), -1, NULL, 0, NULL, NULL);
	string str;

	if (strSize > 0) {
		str.resize(strSize - 1);
		WideCharToMultiByte(CP_UTF8, 0, value.c_str(), -1, &str[0], strSize, NULL, NULL);
	}

	return str;
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

void Utils::ToUpperString(OUT string& str)
{
	for (char& c : str) {
		if (std::isalpha(c)) { 
			c = std::toupper(c);
		}
	}
}

void Utils::ToUpperString(OUT wstring& str)
{
	for (wchar_t& c : str) {
		if (std::isalpha(c)) {
			c = std::toupper(c);
		}
	}
}

void Utils::ToLowerString(OUT string& str)
{
	for (char& c : str) {
		if (std::isalpha(c)) { 
			c = std::tolower(c);
		}
	}
}

void Utils::ToLowerString(OUT wstring& str)
{
	for (wchar_t& c : str) {
		if (std::isalpha(c)) { 
			c = std::tolower(c);
		}
	}
}

float Utils::Random_In_Range(float fMin, float fMax)
{
	random_device rd;
	mt19937_64 dre = mt19937_64(rd());

	uniform_real_distribution<float> random = uniform_real_distribution<float>(fMin, fMax);

	return random(rd);
}

void Utils::Spline(const float* key, int num, int dim, float t, float* v)
{
	static signed char coefs[16] = {
		-1, 2,-1, 0,
		 3,-5, 0, 2,
		-3, 4, 1, 0,
		 1,-1, 0, 0 };

	const int size = dim + 1;

	// find key
	int k = 0; while (key[k * size] < t) k++;

	// interpolant
	const float h = (t - key[(k - 1) * size]) / (key[k * size] - key[(k - 1) * size]);

	// init result
	for (int i = 0; i < dim; i++) v[i] = 0.0f;

	// add basis functions
	for (int i = 0; i < 4; i++)
	{
		int kn = k + i - 2; if (kn < 0) kn = 0; else if (kn > (num - 1)) kn = num - 1;
		const signed char* co = coefs + 4 * i;
		const float b = 0.5f * (((co[0] * h + co[1]) * h + co[2]) * h + co[3]);
		for (int j = 0; j < dim; j++) v[j] += b * key[kn * size + j + 1];
	}
}

_float4x4 Utils::m_matPivot = _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI);

const wstring Utils::m_strModelPath =  L"../Resources/Models/";;

const wstring Utils::m_strTexturePath = L"../Resources/Textures/";

const wstring Utils::m_strSceneDataPath = L"../Resources/Scene/";

const std::wstring Utils::m_strSoundDataPath = L"../Resources/Sound/";
