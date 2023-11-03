#include "pch.h"
#include "Widget_ParticleEffectTool.h"


Widget_ParticleEffectTool::Widget_ParticleEffectTool()
{
}

Widget_ParticleEffectTool::~Widget_ParticleEffectTool()
{
}

void Widget_ParticleEffectTool::Initialize()
{
	Set_Texture_List();
}

void Widget_ParticleEffectTool::Tick()
{
	ImGui::SetNextWindowPos(ImVec2(g_iWinSizeX - 450, 0));
	ImGui::Begin("Particle Maker");
	ImGui_ParticleMaker();
	ImGui::End();
}

void Widget_ParticleEffectTool::Set_Texture_List()
{
	/* ../Resources/Universal/ 폴더 내의 택스처들의 이름을 리스트로 만들어 저장하기 */ \

	wstring assetPath = L"..\\Resources\\Textures\\Universal\\";
	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".tga" && entry.path().extension().wstring() != L".png")
			continue;

		string tag = entry.path().string();
		tag = entry.path().filename().string();
		m_vecUniversalTextures.push_back(tag);
	}

	m_iNumUniversalTextures = (_uint)m_vecUniversalTextures.size();
	m_pszUniversalTextures = new const char* [m_iNumUniversalTextures];

	int iIndex = 0;
	for (auto iter : m_vecUniversalTextures)
	{
		m_pszUniversalTextures[iIndex] = m_vecUniversalTextures[iIndex].c_str();
		iIndex++;
	}

	// For. Setting inputs
	m_iSelected_Texture_Shape = 0;
	m_iSelected_Texture_Option1 = 0;
	m_iSelected_Texture_Option2 = 0;
	m_iSelected_Texture_Option3 = 0;
	m_wstrSelected_Texture_Shape = TEXT("None");
	m_wstrSelected_Texture_Option1 = TEXT("None");
	m_wstrSelected_Texture_Option2 = TEXT("None");
	m_wstrSelected_Texture_Option3 = TEXT("None");
}

void Widget_ParticleEffectTool::ImGui_ParticleMaker()
{
	ImGui::SeparatorText("Basic Information");
	ImGui::InputText("Tag", m_pszParticleTag, MAX_PATH);

	/* For. Texture */
	Option_Textures();

	/* For. Color */
	Option_Color();

	/* For. LifeTime */
	Option_LifeTime();

	/* For. Speed */
	Option_Speed();
}

void Widget_ParticleEffectTool::Option_Textures()
{
	ImGui::SeparatorText("Textures");

	if (ImGui::BeginCombo("Shape", m_pszUniversalTextures[m_iSelected_Texture_Shape], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Shape == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Shape = n;

				string strSelected;
				strSelected = m_pszUniversalTextures[m_iSelected_Texture_Shape];
				m_wstrSelected_Texture_Shape.assign(strSelected.begin(), strSelected.end());
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Option1##Texture", m_pszUniversalTextures[m_iSelected_Texture_Option1], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Option1 == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Option1 = n;

				string strSelected;
				strSelected = m_pszUniversalTextures[m_iSelected_Texture_Option1];
				m_wstrSelected_Texture_Option1.assign(strSelected.begin(), strSelected.end());
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if(ImGui::BeginCombo("Option2##Texture", m_pszUniversalTextures[m_iSelected_Texture_Option2], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Option2 == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Option2 = n;

				string strSelected;
				strSelected = m_pszUniversalTextures[m_iSelected_Texture_Option2];
				m_wstrSelected_Texture_Option2.assign(strSelected.begin(), strSelected.end());
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Option3##Texture", m_pszUniversalTextures[m_iSelected_Texture_Option3], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iSelected_Texture_Option3 == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iSelected_Texture_Option3 = n;

				string strSelected;
				strSelected = m_pszUniversalTextures[m_iSelected_Texture_Option3];
				m_wstrSelected_Texture_Option3.assign(strSelected.begin(), strSelected.end());
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void Widget_ParticleEffectTool::Option_Color()
{
	ImGui::SeparatorText("Color");
	const char* pszItems[] = { "Use Shape Texture Color", "Constant", "Gradation"};
	if (ImGui::BeginCombo("Options##Color", pszItems[m_iSelected_ColorOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (m_iSelected_ColorOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				m_iSelected_ColorOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGuiColorEditFlags ColorEdit_flags = 0 | ImGuiColorEditFlags_AlphaBar;	// RGB, Alpha Bar
	switch (m_iSelected_ColorOption)
	{
	case 1: // Constant
		ImGui::ColorEdit4("Selected Color", (float*)&m_vColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		break;
	case 2: // Gradation
		ImGui::ColorEdit4("Start Color", (float*)&m_vStartColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		ImGui::ColorEdit4("End Color", (float*)&m_vEndColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		break;
	}
}

void Widget_ParticleEffectTool::Option_LifeTime()
{
	ImGui::SeparatorText("LifeTime");

	const char* pszItems[] = { "Constant", "Random", "Curve"};
	if (ImGui::BeginCombo("Options##LifeTime", pszItems[m_iSelected_LifeTimeOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (m_iSelected_LifeTimeOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				m_iSelected_LifeTimeOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	switch (m_iSelected_LifeTimeOption)
	{
	case 0: // Constant
		ImGui::InputFloat("LifeTime(sec)", &m_iLifeTime[0]);
		break;
	case 1: // Random
		ImGui::InputFloat2("Min, Max LifeTime(sec)", m_iLifeTime);
		break;
	case 2: // Curve
		
		break;
	}
}

void Widget_ParticleEffectTool::Option_Speed()
{
	ImGui::SeparatorText("Speed");

	const char* pszItems[] = { "Constant", "Random", "Curve" };
	if (ImGui::BeginCombo("Options##Speed", pszItems[m_iSelected_SpeedOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems); n++)
		{
			const bool is_selected = (m_iSelected_SpeedOption == n);
			if (ImGui::Selectable(pszItems[n], is_selected))
				m_iSelected_SpeedOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	switch (m_iSelected_SpeedOption)
	{
	case 0: // Constant
		ImGui::InputFloat("Speed", &m_iSpeed[0]);
		break;
	case 1: // Random
		ImGui::InputFloat2("Min, Max Speed", m_iSpeed);
		break;
	case 2: // Curve

		break;
	}
}
