#include "pch.h"
#include "Widget_EffectMaker_Mesh.h"

/* Components */
#include "MeshEffect.h"

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

		if (entry.path().extension().wstring() != L".Model")
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

	// Start Color 
	ImGui::SeparatorText("Diffuse Color");
	
	const char* pszItems_StartColor[] = { "Custom Color", "Random Color in range"};
	if (ImGui::BeginCombo("Option##StartColor", pszItems_StartColor[m_iStartColorOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems_StartColor); n++)
		{
			const bool is_selected = (m_iStartColorOption == n);
			if (ImGui::Selectable(pszItems_StartColor[n], is_selected))
				m_iStartColorOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	switch (m_iStartColorOption)
	{
	case 0:
		ImGui::ColorEdit4("Color##StartColor", (float*)&m_vRangeStartColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		m_vRangeEndColor = m_vRangeStartColor;
		break;
	case 1:
		ImGui::ColorEdit4("Range Color1##StartColor", (float*)&m_vRangeStartColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		ImGui::ColorEdit4("Range Color2##StartColor", (float*)&m_vRangeEndColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		break;
	}
	
	// Gradation 
	ImGui::SeparatorText("Change Color by lifetime");
	ImGui::Checkbox("Gradation On", &m_bGradationOn);
	if (m_bGradationOn)
	{
		const char* pszItems_GradationColor[] = { "Custom", "Darker", "Brighter"};
		if (ImGui::BeginCombo("Option##GradationColor", pszItems_GradationColor[m_iGradationOption], 0))
		{
			for (_uint n = 0; n < IM_ARRAYSIZE(pszItems_GradationColor); n++)
			{
				const bool is_selected = (m_iGradationOption == n);
				if (ImGui::Selectable(pszItems_GradationColor[n], is_selected))
					m_iGradationOption = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		if (0 == m_iGradationOption)
			ImGui::ColorEdit4("Color##GradationColor", (float*)&m_vGradationColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		ImGui::SliderFloat("Gradation Intencity", &m_fGradationIntensity, 0.1f, 0.9f);
	}

	// Changing Color
	ImGui::SeparatorText("Change Color by lifetime");
	const char* pszItems_ChangingColor[] = { "No Change", "Linear", "Curve"};
	if (ImGui::BeginCombo("Option##ChangingColor", pszItems_ChangingColor[m_iChangingColorOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItems_ChangingColor); n++)
		{
			const bool is_selected = (m_iChangingColorOption == n);
			if (ImGui::Selectable(pszItems_ChangingColor[n], is_selected))
				m_iChangingColorOption = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	if (0 != m_iChangingColorOption)
	{
		ImGui::ColorEdit4("Color##ChangingColor", (float*)&m_vDestColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		if (m_bGradationOn && 2 == m_iGradationOption)
			ImGui::ColorEdit4("Color##ChangingColor", (float*)&m_vDestGradationColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
	}

	ImGui::SeparatorText("ETC");
	ImGui::Checkbox("On Fade Out", &m_bUseFadeOut);
}

void Widget_EffectMaker_Mesh::Create()
{
	// For. Create GameObject 
	shared_ptr<GameObject> EffectObj = make_shared<GameObject>();

	// For. Add and Setting Transform Component
	EffectObj->GetOrAddTransform();
	EffectObj->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));

	// For. Add and Setting Effect Component to GameObject
	shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Effect2.fx");
	shared_ptr<MeshEffect> meshEffect = make_shared<MeshEffect>(shader);
	EffectObj->Add_Component(meshEffect);

	MeshEffect::DESC tMeshEffectDesc
	{
		m_szTag

	};
	EffectObj->Get_MeshEffect()->Init(&tMeshEffectDesc);

	// For. Add Effect GameObject to current scene
	CUR_SCENE->Add_GameObject(EffectObj);
}

void Widget_EffectMaker_Mesh::Save()
{
}

void Widget_EffectMaker_Mesh::Load()
{
}
