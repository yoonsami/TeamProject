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
	/* For. Target Particle Object */
	shared_ptr<GameObject>	m_pTargetParticle = { nullptr };

	/* For. Particle Texture list */
	_uint					m_iNumUniversalTextures = { 0 };
	vector<string>			m_vecUniversalTextures;
	const char**			m_pszUniversalTextures = { nullptr };

	/* For. Select Texture */
	_uint					m_iSelected_Texture_Shape = { 0 };
	string					m_strSelected_Texture_Shape = { "None" };
	_uint					m_iSelected_Texture_Dissolve = { 0 };
	string					m_strSelected_Texture_Dissolve = { "None" };
	_uint					m_iSelected_Texture_Distortion = { 0 };
	string					m_strSelected_Texture_Distortion = { "None" };
	_uint					m_iSelected_Texture_Option1 = { 0 };
	string					m_strSelected_Texture_Option1 = { "None" };
	_uint					m_iSelected_Texture_Option2 = { 0 };
	string					m_strSelected_Texture_Option2 = { "None" };

	/* For. Tag (GameObject's) */
	char					m_pszParticleTag[MAX_PATH] = "-";

	/* For. Diffuse Color */
	_uint					m_iSelected_ColorOption = { 0 };
	ImVec4					m_vColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4					m_vStartColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // For. Gradation start color
	ImVec4					m_vEndColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);// For. Gradation end color

	/* For. Coloring Option */
	_bool					m_bGradationByAlpha_Brighter = { false };
	_bool					m_bGradationByAlpha_Darker = { false };
	_bool					m_bIsAlphaFollowDuration = { true };				// 듀레이션과 현재 시간의 비가 모든 파티클의 알파값에 영향을 준다.
	_bool					m_bDissolveOn = { false };
	_float					m_fDissolveSpeed = { 1.f };
	_bool					m_bDistortionOn = { false };
	_float					m_fDistortionOffset[2] = {0.5f, 0.5f};

	/* For. LifeTime */
	_uint					m_iSelected_LifeTimeOption = { 0 };
	_float					m_iLifeTime[2] = {1.f, 5.f};

	/* For. Speed */
	_uint					m_iSelected_SpeedOption = { 0 };
	_float					m_iSpeed[2] = { 1.f, 5.f };

	/* For. Billbord */
	_uint					m_iSelected_BillbordOption = { 0 };

	/* For. Movement */
	_uint					m_iSelected_MovementOption = { 0 };

	/* For. Implicit Options */
	_float					m_fDuration = { 5.f };
	_bool					m_bIsLoop = { false };

	/* For. Create Particle position */
	_float					m_fCenterPosition[3] = { 0.f, 0.f, 0.f };
	_float					m_fCreateRange[3] = { 1.f, 1.f, 1.f };

	/* For. Scale */
	_uint					m_iSelected_ScaleOption = { 0 };
	_float					m_iScale[2] = { 1.f, 5.f };

	/* For. Rotation (Billbord option에 따라 선택할 수 있는 옵션이 달라진다.) */
	_float					m_fRotationSpeed[3] = { 0.f, 0.f, 0.f };
	_float					m_fRotationDir[3] = { 1.f, 1.f, 1.f };
};

