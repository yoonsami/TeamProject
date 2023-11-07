#pragma once

class Utils
{
public:
	static bool StartsWith(string str, string comp);
	static bool StartsWith(wstring str, wstring comp);

	static wstring ToWString(string value);
	static string ToString(wstring value);

	static wstring ToWstringUtf8(string value);
	static string TostringUtf8(wstring value);

	static void ChangeExt(OUT string& srcPath, const string& ext);
	static void ChangeExt(OUT wstring& srcPath, const wstring& ext);

	static void DetachExt(OUT string& path);
	static void DetachExt(OUT wstring& path);

	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);

	static _float4x4 m_matPivot;

	static void ToUpperString(OUT string& str);
	static void ToUpperString(OUT wstring& str);

	static void ToLowerString(OUT string& str);
	static void ToLowerString(OUT wstring& str);

	const static wstring m_strModelPath;
	const static wstring m_strTexturePath;
	const static wstring m_strSceneDataPath;
	const static wstring m_strSoundDataPath;

};

