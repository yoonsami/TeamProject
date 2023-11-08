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
	void					Option_Property();
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

	/* Texture type list */
	_int					m_iNumTextureTypes = { 8 };
	const char*				m_pszTextureTypes[8] = {
		"Diffuse", "Normal", "Speculer", "Opacity", "Emissive", "Dissolve","Distortion", "Additional 1"
	};
	
	/* Texture list */
	_uint					m_iNumUniversalTextures = { 0 };
	vector<string>			m_vecUniversalTextures;
	const char**			m_pszUniversalTextures = { nullptr };
	_int					m_iTexture[8];
	string					m_strTexture[8];


	/* Color 
		(Base, Gradation, OverlayStart, OverlayEnd
		BaseDest, GradationDest, OverlayStartDest, OverlayEndDest)*/
	_int					m_iBaseColorOption = { 0 }; 
	ImVec4					m_vBaseColor_RangeStart = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4					m_vBaseColor_RangeEnd = ImVec4(1.f, 1.f, 1.f, 1.f);

	_bool					m_bGradationOn = { false };
	_float					m_fGradationIntensity = { 0.f };
	ImVec4					m_vGradationColor = ImVec4(186.f / 255.f, 48.f / 255.f, 48.f / 255.f, 1.f);

	_bool					m_bOverlayOn = { false };
	_int					m_iOverlayOption = { 0 };
	ImVec4					m_vOverlayColor_Start = ImVec4(1.f, 1.f, 5.f / 255.f, 1.f);
	ImVec4					m_vOverlayColor_End = ImVec4(186.f / 255.f, 48.f / 255.f, 48.f / 255.f, 1.f);

	_bool					m_bChangingColorOn = { false };

	ImVec4					m_vDestBaseColor = ImVec4(86.f / 255.f, 130.f / 255.f, 184.f / 255.f, 1.f);
	
	ImVec4					m_vDestGradationColor = ImVec4(62.f / 255.f, 20.f / 255.f, 143.f / 255.f, 1.f);

	ImVec4					m_vDestOverlayColor_Start = ImVec4(1.f, 1.f, 5.f / 255.f, 1.f);
	ImVec4					m_vDestOverlayColor_End = ImVec4(186.f / 255.f, 48.f / 255.f, 48.f / 255.f, 1.f);

	_bool					m_bUseFadeOut = { true };
};

