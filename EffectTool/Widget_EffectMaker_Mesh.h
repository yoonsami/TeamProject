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

private:
	/* Initialize List */
	void					Set_Mesh_List();
	void					Set_Texture_List();
	void					Set_FinishedEffect_List();

	/* ImGui Windows */
	void					ImGui_EffectMaker();
	void					ImGui_FinishedEffect();

	/* Funtions */
	void					Option_Property();
	void					Option_Mesh();
	void					Option_Opacity();
	void					Option_Blend();
	void					Option_Diffuse();
	void					Option_Normal();
	void					Option_AlphaGradation();
	void					Option_Gradation();
	void					Option_Overlay();
	void					Option_Dissolve();
	void					Option_Distortion();
	void					Option_ColorEdit();

	void					Create();
	void					Save();
	void					Load();

	void					SubWidget_TextureCombo(_int* iSelected, string* strSelected, string strFilePath, const char* pszWidgetKey);
	void					SubWidget_ImageViewer(string strFileName, string strFilePath, const char* pszWidgetKey);
	void					SubWidget_SettingTexUV(_float* arrTiling, _float* arrTexUVSpeed, const char* pszWidgetKey, const char* pszWidgetKey2);

	Color					ImVec4toColor(ImVec4 imvec);
	ImVec4					ColorToImVec4(Color color);
	_int					GetIndex_FromTexList(string strValue);
	_int					GetIndex_FromMeshList(string strValue);
	_bool					Compare_IsSameColor(ImVec4 color1, ImVec4 color2);
	_bool					Compare_IsSameUVOptionsWithOpacity(_float2 tiling, _float2 UVSpeed);
private:
	/* Effect List */
	_uint					m_iNumFinishedEffects = { 0 };
	vector<string>			m_vecFinishedEffects;
	const char**			m_pszFinishedEffects = { nullptr };
	_int					m_iFinishedObject = { 0 };
	string					m_strFinishedObject = {"None"};

	/* Property */
	char					m_szTag[MAX_PATH] = "-";
	_float					m_fDuration = { 3.f };
	_bool					m_bBlurOn = { false };
	_bool					m_bUseFadeOut = { true };

	/* Mesh list */
	_uint					m_iNumMeshes = { 0 };
	vector<string>			m_vecMeshes;
	const char**			m_pszMeshes = { nullptr };
	_uint					m_iMesh = { 0 };
	string					m_strMesh = { "None" };

	/* Texture type list */
	_int					m_iNumTextureTypes = { 9 };
	const char*				m_pszTextureTypes[9] = {
		"Diffuse", "Normal", "Speculer", "Opacity", "Emissive", "Dissolve","Distortion", "Gradation (tex 7)", "Blend (tex 9)"
	};
	
	/* Texture list */
	_uint					m_iNumUniversalTextures = { 0 };
	vector<string>			m_vecUniversalTextures;
	const char**			m_pszUniversalTextures = { nullptr };

	/* Coloring Options */
	_bool					m_bColorChangingOn = { false };

	/* Diffuse */
	_int					m_iDiffuseOption = { 0 };	// Texture, Custom, Random
	pair<_int, string>		m_DiffuseTexture = { 0, "None" };
	ImVec4					m_vDiffuseColor_BaseStart = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4					m_vDiffuseColor_BaseEnd = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4					m_vDiffuseColor_Dest = ImVec4(1.f, 1.f, 1.f, 1.f);

	/* Alpha Gradation */
	_bool					m_bAlphaGra_On = { false };
	_float					m_fAlphaGraIntensity = { 0.5f };
	ImVec4					m_vAlphaGraColor_Base = ImVec4(1.f, 1.f, 1.f, 1.f);
	_bool					m_bDestSameWithBase_AlphaGra = { false };
	ImVec4					m_vAlphaGraColor_Dest = ImVec4(1.f, 1.f, 1.f, 1.f);

	/* Opacity */
	pair<_int, string>		m_OpacityTexture = { 0, "None" };
	_int					m_iSamplerType = { 1 };
	_float					m_fTiling_Opacity[2] = {0.f, 0.f};
	_float					m_fUVSpeed_Opacity[2] = { 0.f, 0.f };

	/* Gradation by Texture */
	_bool					m_bGra_On = { false };
	pair<_int, string>		m_GraTexture = { 0, "None" };
	ImVec4					m_vGraColor_Base = ImVec4(1.f, 1.f, 1.f, 1.f);
	_bool					m_bUVOptionSameWithOpacity_Gra = { true };
	_float					m_fTiling_Gra[2] = { 0.f, 0.f };
	_float					m_fUVSpeed_Gra[2] = { 0.f, 0.f };
	_bool					m_bDestSameWithBase_Gra = { false };
	ImVec4					m_vGraColor_Dest = ImVec4(1.f, 1.f, 1.f, 1.f);

	/* Overlay */
	_bool					m_bOverlay_On = { false };
	pair<_int, string>		m_OverlayTexture = { 0, "None" };
	ImVec4					m_vOverlayColor_Base = ImVec4(1.f, 1.f, 1.f, 1.f);
	_bool					m_bUVOptionSameWithOpacity_Overlay = { true };
	_float					m_fTiling_Overlay[2] = { 0.f, 0.f };
	_float					m_fUVSpeed_Overlay[2] = { 0.f, 0.f };

	/* Normal */
	_bool					m_bNormal_On = { false };
	pair<_int, string>		m_NormalTexture = { 0, "None" };

	/* Dissolve */
	_bool					m_bDissolve_On = { true };
	pair<_int, string>		m_DissolveTexture = { 0, "None" };
	_bool					m_bUVOptionSameWithOpacity_Dissolve = { true };
	_float					m_fTiling_Dissolve[2] = { 0.f, 0.f };
	_float					m_fUVSpeed_Dissolve[2] = { 0.f, 0.f };
	_bool					m_bDissolveInverse = { false };

	/* Distortion */
	_bool					m_bDistortion_On = { true };
	pair<_int, string>		m_DistortionTexture = { 0, "None" };
	_bool					m_bUVOptionSameWithOpacity_Distortion = { true };
	_float					m_fTiling_Distortion[2] = { 0.f, 0.f };
	_float					m_fUVSpeed_Distortion[2] = { 0.f, 0.f };

	/* Blend */
	_bool					m_bBlend_On = { true };
	pair<_int, string>		m_BlendTexture = { 0, "None" };

	/* Color Edit */
	_float					m_fContrast = { 0.f };

	/* const */
	const string			m_strTexturePath = "../Resources/Textures/Universal/";
	const string			m_strNormalTexturePath = "../Resources/Textures/Universal/Normal/";
	const ImGuiColorEditFlags		ColorEdit_flags = 0 | ImGuiColorEditFlags_AlphaBar;
};

