#pragma once
class Widget_ParticleMaker_Instancing
{
public:
	Widget_ParticleMaker_Instancing();
	~Widget_ParticleMaker_Instancing();

public:
	void	Initialize();
	void	Tick();

private:
	/* Initialize List */
	void					Set_Texture_List();
	
	/* ImGui Windows */
	void					ImGui_ParticleMaker(); 

	/*  Funtions */
	void					Option_ParticleObjectProperty();
	void					Option_ParticleProperty();
	void					Option_Textures();
	void					Option_Color();
	void					Option_Transform();
	void					Option_Movement();

	void					Create();
	void					Load();
	void					Save();

private:
	/* For. Target Particle Object */
	shared_ptr<GameObject>	m_pTargetParticle = { nullptr };

	/* For. Particle GameObject Property */
	char					m_pszParticleTag[MAX_PATH] = "-";
	_float					m_fDuration = { 5.f };
	_int					m_iParticleCnt[2] = { 1, 10 };
	_int					m_iMaxInstanceCnt = { 500 };
	_bool					m_bIsbCreateOnce = { false };
	_float					m_fCreateInterval = { 0.5f };

	/* For. Particle Property */
	_int					m_iSelected_LifeTimeOption = { 0 };
	_float					m_fLifeTime[2] = { 1.f, 5.f };
	_int					m_iSelected_BillbordOption = { 0 };

	/* For. Particle Texture */
	_uint					m_iNumUniversalTextures = { 0 };
	vector<string>			m_vecUniversalTextures;
	const char**			m_pszUniversalTextures = { nullptr };

	_uint					m_iSelected_Texture_Shape = { 0 };
	string					m_strSelected_Texture_Shape = { "None" };
	_uint					m_iSelected_Texture_Dissolve = { 0 };
	string					m_strSelected_Texture_Dissolve = { "None" };
	_uint					m_iSelected_Texture_Option1 = { 0 };
	string					m_strSelected_Texture_Option1 = { "None" };
	_uint					m_iSelected_Texture_Option2 = { 0 };
	string					m_strSelected_Texture_Option2 = { "None" };

	/* For. Color */
	_int					m_iSelected_StartColorOption = { 0 };
	ImVec4					m_vStartColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); 
	ImVec4					m_vEndColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);	
	_int					m_iSelected_EndColorOption = { 0 };
	
	_int					m_iSelected_ChangingColorOption = { 0 };
	ImVec4					m_vDestColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	_float					m_fGradationByAlpha_Brighter = { 0.f };
	_float					m_fGradationByAlpha_Darker = { 0.f };
	
	_bool					m_bUseFadeOut = { true };				

	/* For. Create Particle position */
	_float					m_fCenterPosition[3] = { 0.f, 0.f, 0.f };
	_float					m_fCreateRange[3] = { 1.f, 1.f, 1.f };
	_float4					m_fCreateOffset = { 0.f, 0.f, 0.f, 0.f };

	/* For. Scale */
	_float					m_fStartScale[2] = { 1.f, 1.f };
	_float					m_fEndScale [2] = { 1.f, 5.f };
	_int					m_iSelected_EndScaleOption = { 0 };

	/* For. Rotation (Billbord option에 따라 선택할 수 있는 옵션이 달라진다.) */
	_float					m_fRotationSpeed_X[2] = { 0.f, 0.f };	// use 
	_float					m_fRotationSpeed_Y[2] = { 0.f, 0.f };
	_float					m_fRotationSpeed_Z[2] = { 0.f, 0.f };
	_float					m_fRotationAngle_X[2] = { -1.f, 1.f };	// use 
	_float					m_fRotationAngle_Y[2] = { -1.f, 1.f };	// use
	_float					m_fRotationAngle_Z[2] = { -1.f, 1.f };	// use

	/* For. Speed */
	_float					m_fStartSpeed[2] = { 1.f, 1.f };
	_float					m_fEndSpeed[2] = { 1.f, 5.f };
	_int					m_iSelected_EndSpeedOption = { 0 };

	/* For. Movement */
	_int					m_iSelected_MovementOption = { 0 };
	_float					m_vMovementOffsets[4] = { 0.f, 0.f, 0.f, 0.f };
};

