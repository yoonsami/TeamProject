#pragma once
class Widget_EffectMaker_Mesh
{
public:
	Widget_EffectMaker_Mesh();
	~Widget_EffectMaker_Mesh();

public:
	void	Initialize();
	void	Tick();

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
	/* Mesh list */
	_uint					m_iNumMeshes = { 0 };
	vector<string>			m_vecMeshes;
	const char**			m_pszMeshes = { nullptr };
	
	_uint					m_iMeshOption = { 0 };
	string					m_strMeshOption = { "None" };

	/* Texture list */
	_uint					m_iNumUniversalTextures = { 0 };
	vector<string>			m_vecUniversalTextures;
	const char**			m_pszUniversalTextures = { nullptr };

	_uint					m_iTextureOption_Diffuse = { 0 };
	string					m_strTextureOption_Diffuse = { "None" };
	_uint					m_iTextureOption_Dissolve = { 0 };
	string					m_strTextureOption_Dissolve = { "None" };
	_uint					m_iTextureOption_Distortion = { 0 };
	string					m_strTextureOption_Distortion = { "None" };
	_uint					m_iTextureOption_Option1 = { 0 };
	string					m_strTextureOption_Option1 = { "None" };

	/* Diffuse Color */
	_int					m_iSelected_StartColorOption = { 0 };
	_int					m_iSelected_EndColorOption = { 0 };
	_int					m_iSelected_ChangingColorOption = { 0 };
	ImVec4					m_vStartColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4					m_vEndColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4					m_vDestColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	_float					m_fGradationByAlpha_Brighter = { 0.f };
	_float					m_fGradationByAlpha_Darker = { 0.f };

	_bool					m_bUseFadeOut = { true };
};

