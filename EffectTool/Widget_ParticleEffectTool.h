#pragma once

class Widget_ParticleEffectTool 
{
public:
	Widget_ParticleEffectTool();
	~Widget_ParticleEffectTool();

public:
	void	Initialize();
	void	Tick();

private:
	/* Initialize List */
	void					Set_Texture_List();
	
	/* ImGui Windows */
	void					ImGui_ParticleMaker();

	/*  Funtions */
	void					Option_Textures();
	void					Option_Color();
	void					Option_LifeTime();
	void					Option_Speed();
	void					Option_Billbord();
	void					Create();
	void					Delete();

private:
	shared_ptr<GameObject>	m_pTargetParticle = { nullptr };

	/* For. Particle Texture list */
	_uint					m_iNumUniversalTextures = { 0 };
	vector<string>			m_vecUniversalTextures;
	const char**			m_pszUniversalTextures = { nullptr };

	/* For. Select Texture */
	_uint					m_iSelected_Texture_Shape = { 0 };
	wstring					m_wstrSelected_Texture_Shape;
	_uint					m_iSelected_Texture_Option1 = { 0 };
	wstring					m_wstrSelected_Texture_Option1;
	_uint					m_iSelected_Texture_Option2 = { 0 };
	wstring					m_wstrSelected_Texture_Option2;
	_uint					m_iSelected_Texture_Option3 = { 0 };
	wstring					m_wstrSelected_Texture_Option3;

	/* For. Etc */
	char					m_pszParticleTag[MAX_PATH] = "-";

	/* For. Color */
	_uint					m_iSelected_ColorOption = { 0 };
	ImVec4					m_vColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4					m_vStartColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // For. Gradation start color
	ImVec4					m_vEndColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);// For. Gradation end color

	/* For. LifeTime */
	_uint					m_iSelected_LifeTimeOption = { 0 };
	_float					m_iLifeTime[2] = {1.f, 5.f};

	/* For. Speed */
	_uint					m_iSelected_SpeedOption = { 0 };
	_float					m_iSpeed[2] = { 1.f, 5.f };

	/* For. Billbord */
	_uint					m_iSelected_BillbordOption = { 0 };
};

