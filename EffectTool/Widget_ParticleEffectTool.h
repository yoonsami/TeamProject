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
	void					Option_Etc();
	void					Option_Textures();
	void					Option_Color();
	void					Option_Transform();
	void					Option_LifeTime();
	void					Option_Movement();
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
	_uint					m_iSelected_Texture_Option1 = { 0 };
	string					m_strSelected_Texture_Option1 = { "None" };
	_uint					m_iSelected_Texture_Option2 = { 0 };
	string					m_strSelected_Texture_Option2 = { "None" };

	/* For. Tag (GameObject's) */
	char					m_pszParticleTag[MAX_PATH] = "-";

	/* For. Diffuse Color */
	_uint					m_iSelected_ColorOption = { 0 };
	ImVec4					m_vStartColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // For. Gradation start color
	ImVec4					m_vEndColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);// For. Gradation end color
	ImVec4					m_vDestColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	/* For. Coloring Option */
	_float					m_fGradationByAlpha_Brighter = { false };
	_float					m_fGradationByAlpha_Darker = { false };
	_bool					m_bIsAlphaFollowDuration = { true };				// 듀레이션과 현재 시간의 비가 모든 파티클의 알파값에 영향을 준다.
	
	_int					m_iSelected_DissolveOption = { 0 };
	_float					m_fDissolveSpeed[2] = {1.f, 1.f};
	
	/* For. LifeTime */
	_uint					m_iSelected_LifeTimeOption = { 0 };
	_float					m_fLifeTime[2] = {1.f, 5.f};

	/* For. Speed */
	_uint					m_iSelected_SpeedOption = { 0 };
	_float					m_fSpeed[2] = { 1.f, 5.f };

	/* For. Billbord */
	_uint					m_iSelected_BillbordOption = { 0 };

	/* For. Movement */
	_uint					m_iSelected_MovementOption = { 0 };
	_float					m_vMovementOffsets[4] = {0.f, 0.f, 0.f, 0.f};

	/* For. Implicit Options */
	_uint					m_iParticleCnt[2] = {1, 10};
	_uint					m_iMaxParticle = { 1000 };
	_float					m_fDuration = { 5.f };
	_float					m_fCreateInterval;
	_bool					m_bIsLoop = { false };

	/* For. Create Particle position */
	_float					m_fCenterPosition[3] = { 0.f, 0.f, 0.f };
	_float					m_fCreateRange[3] = { 1.f, 1.f, 1.f };
	_float4					m_fCreateOffset = { 0.f, 0.f, 0.f, 0.f };

	/* For. Scale */
	_float					m_fStartScale[2] = { 1.f, 1.f };	// min, max
	_uint					m_iSelected_ScaleOption = { 0 };
	_float					m_fScaleSpeed[2] = { 1.f, 5.f };

	/* For. Rotation (Billbord option에 따라 선택할 수 있는 옵션이 달라진다.) */
	_float					m_fRotationSpeed[3] = { 0.f, 0.f, 0.f };
	_uint					m_iSelected_RotationAngleOption = { 0 };
	_float					m_fRotationAngle[3] = { 1.f, 1.f, 1.f };
};

