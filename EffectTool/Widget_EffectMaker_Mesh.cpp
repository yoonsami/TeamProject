#include "pch.h"
#include "Widget_EffectMaker_Mesh.h"

Widget_EffectMaker_Mesh::Widget_EffectMaker_Mesh()
{
}

Widget_EffectMaker_Mesh::~Widget_EffectMaker_Mesh()
{
}

void Widget_EffectMaker_Mesh::Initialize()
{
	Set_Mesh_List();
	Set_Texture_List();
}

void Widget_EffectMaker_Mesh::Tick()
{
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
	ImGui::Begin("Particle Maker");
	ImGui_EffectMaker();
	ImGui::End();
}

void Widget_EffectMaker_Mesh::Set_Mesh_List()
{
	m_vecMeshes.push_back("None");

	wstring assetPath = L"..\\Resources\\Models\\VfxMesh\\";
	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		string tag = entry.path().string();
		tag = entry.path().filename().string();
		m_vecMeshes.push_back(tag);
	}

	m_iNumMeshes = (_uint)m_vecMeshes.size();
	m_pszMeshes = new const char* [m_iNumMeshes];

	int iIndex = 0;
	for (auto iter : m_vecMeshes)
	{
		m_pszMeshes[iIndex] = m_vecMeshes[iIndex].c_str();
		iIndex++;
	}
}

void Widget_EffectMaker_Mesh::Set_Texture_List()
{
	/* Make Texture list in ../Resources/Textures/Universal/ .png */

	m_vecUniversalTextures.push_back("None");

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
}

void Widget_EffectMaker_Mesh::ImGui_EffectMaker()
{
	Option_Mesh();
	Option_Texture();	
	Option_Color();

	/* For. Create, Save, Load Effect */
	ImGui::Spacing();
	ImGui::SeparatorText(" Create/Delete ");
	if (ImGui::Button("Create"))
		Create();
	ImGui::SameLine();
	if (ImGui::Button("Save"))
		Save();	
	ImGui::SameLine();
	if (ImGui::Button("Load"))
		Load();
	ImGui::SameLine();
}

void Widget_EffectMaker_Mesh::Option_Mesh()
{
	ImGui::SeparatorText("Vfx Mesh");

	if (ImGui::BeginCombo("VfxMesh", m_pszMeshes[m_iMeshOption], 0))
	{
		for (_uint n = 0; n < m_iNumMeshes; n++)
		{
			const bool is_selected = (m_iMeshOption == n);
			if (ImGui::Selectable(m_pszMeshes[n], is_selected))
			{
				m_iMeshOption = n;
				m_strMeshOption = m_pszMeshes[m_iMeshOption];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void Widget_EffectMaker_Mesh::Option_Texture()
{
	ImGui::SeparatorText("Textures");

	if (ImGui::BeginCombo("Diffuse", m_pszUniversalTextures[m_iTextureOption_Diffuse], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iTextureOption_Diffuse == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iTextureOption_Diffuse = n;
				m_strTextureOption_Diffuse = m_pszUniversalTextures[m_iTextureOption_Diffuse];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Dissolve", m_pszUniversalTextures[m_iTextureOption_Dissolve], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iTextureOption_Dissolve == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iTextureOption_Dissolve = n;
				m_strTextureOption_Dissolve = m_pszUniversalTextures[m_iTextureOption_Dissolve];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Distortion", m_pszUniversalTextures[m_iTextureOption_Distortion], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iTextureOption_Distortion == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iTextureOption_Distortion = n;
				m_strTextureOption_Distortion = m_pszUniversalTextures[m_iTextureOption_Distortion];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Option1", m_pszUniversalTextures[m_iTextureOption_Option1], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (m_iTextureOption_Option1 == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				m_iTextureOption_Option1 = n;
				m_strTextureOption_Option1 = m_pszUniversalTextures[m_iTextureOption_Option1];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void Widget_EffectMaker_Mesh::Option_Color()
{
	ImGuiColorEditFlags ColorEdit_flags = 0 | ImGuiColorEditFlags_AlphaBar;	// RGB, Alpha Bar

	/* Diffuse Color */
	ImGui::SeparatorText("Diffuse Color");
	const char* pszItems_DesideDiffuseColor[] = { "Use Distortion Texture Color", "Custom Color", "Random Color"};
	if (ImGui::BeginCombo("Option##Color", pszItems_DesideDiffuseColor[m_iSelected_StartColorOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems_DesideDiffuseColor); n++)
		{
			const bool is_selected = (m_iSelected_StartColorOption == n);
			if (ImGui::Selectable(pszItems_DesideDiffuseColor[n], is_selected))
				m_iSelected_StartColorOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	switch (m_iSelected_StartColorOption)
	{
	case 1:
		ImGui::ColorEdit4("Color##DiffuseColor_Custom", (float*)&m_vStartColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		m_vEndColor = m_vStartColor;
		break;
	case 2:
		ImGui::ColorEdit4("Color##DiffuseColorRange_Start", (float*)&m_vStartColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		ImGui::ColorEdit4("Color##DiffuseColorRange_End", (float*)&m_vEndColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		break;
	}
	ImGui::Separator();
	ImGui::Checkbox("On Fade Out", &m_bUseFadeOut);

	ImGui::Separator();
	ImGui::SliderFloat("Brigher Offset", &m_fGradationByAlpha_Brighter, 0.f, 1.f);
	ImGui::SliderFloat("Darker Offset", &m_fGradationByAlpha_Darker, 0.f, 1.f);



}

void Widget_EffectMaker_Mesh::Create()
{
}

void Widget_EffectMaker_Mesh::Save()
{
}

void Widget_EffectMaker_Mesh::Load()
{
}
