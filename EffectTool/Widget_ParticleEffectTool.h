#pragma once

class Widget_ParticleEffectTool 
{
private:
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
	

private:
	/* For. Particle Texture list  */
	_uint					m_iNumTextures = { 0 };
	vector<string>			m_vecTextures;
	const char**			m_pszTextures = { nullptr };

	/* For. Select Texture */
	_uint					m_iSelected_Texture1 = { 0 };
	wstring					m_wstrSelected_Texture1;
	_uint					m_iSelected_Texture2 = { 0 };
	wstring					m_wstrSelected_Texture2;
	_uint					m_iSelected_Texture3 = { 0 };
	wstring					m_wstrSelected_Texture3;
	_uint					m_iSelected_Texture4 = { 0 };
	wstring					m_wstrSelected_Texture4;
};

