#pragma once
#include "ImGuiToolMgr.h"

class Widget_EffectMaker_Mesh
{
public:
	Widget_EffectMaker_Mesh();
	~Widget_EffectMaker_Mesh();

public:
	void	Initialize();
	void	Tick();

	void	Set_ImGuiStyle(ImGuiToolMgr::IMGUI_STYLE tDesc) { m_tImGuiStyle = tDesc; }

private:
	/* Initialize List */
	void					Set_Mesh_List();
	void					Set_Texture_List();

	/* ImGui Windows */
	void					ImGui_EffectMaker();

	/* Funtions */
	void					Option_Mesh();
	void					Option_Texture();
	void					Option_Color();

	void					Create();
	void					Save();
	void					Load();

private:
	/* ImGui Window Option */
	ImGuiToolMgr::IMGUI_STYLE		m_tImGuiStyle;

	/* Property */
	char					m_szTag[MAX_PATH] = "-";
	_float					m_fDuration = { 3.f };
	_bool					m_bBlurOn = { false };

	/* Mesh list */
	_uint					m_iNumMeshes = { 0 };
	vector<string>			m_vecMeshes;
	const char**			m_pszMeshes = { nullptr };
	_uint					m_iMesh = { 0 };
	string					m_strMesh = { "None" };

	/* Texture list */
	_uint					m_iNumUniversalTextures = { 0 };
	vector<string>			m_vecUniversalTextures;
	const char**			m_pszUniversalTextures = { nullptr };
	pair<_int, string>		m_pTexture[7] = { {(_int)TextureMapType::END, "None"} };

	/* Diffuse Color */
	_int					m_iStartColorOption = { 0 };
	ImVec4					m_vRangeStartColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4					m_vRangeEndColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	_bool					bGradationOn = { true };
	_float					m_fGradationIntensity = { 0.f };
	ImVec4					m_vGradationColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	_bool					bChangingColorOn = { true };
	_int					m_iChangingColorOption = { 0 };
	ImVec4					m_vDestColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4					m_vDestGradationColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	_bool					m_bUseFadeOut = { true };
};

