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
	ImGui::Begin("Mesh Effect Maker");
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

	for (_uint i = 0; i < 8; i++)
	{
		m_iTexture[i] = 0;
		m_strTexture[i] = "None";
	}
}

void Widget_EffectMaker_Mesh::ImGui_EffectMaker()
{
	Option_Property();
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

void Widget_EffectMaker_Mesh::Option_Property()
{
	ImGui::SeparatorText("Property");

	ImGui::InputText("Tag", m_szTag, MAX_PATH);
	ImGui::Spacing();

	ImGui::InputFloat("Duration", &m_fDuration);
	ImGui::Spacing();

	ImGui::Checkbox("Blur On", &m_bBlurOn);
}

void Widget_EffectMaker_Mesh::Option_Mesh()
{
	ImGui::SeparatorText("Vfx Mesh");

	if (ImGui::BeginCombo("VfxMesh", m_pszMeshes[m_iMesh], 0))
	{
		for (_uint n = 0; n < m_iNumMeshes; n++)
		{
			const bool is_selected = (m_iMesh == n);
			if (ImGui::Selectable(m_pszMeshes[n], is_selected))
			{
				m_iMesh = n;
				m_strMesh = m_pszMeshes[m_iMesh];
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
	ImGui::SeparatorText("Texture");

 	for (_int i = 0; i < m_iNumTextureTypes; i++)
	{
		if (ImGui::BeginCombo(m_pszTextureTypes[i], m_pszUniversalTextures[m_iTexture[i]], 0))
		{
			for (_uint n = 0; n < m_iNumUniversalTextures; n++)
			{
				const bool is_selected = (m_iTexture[i] == n);
				if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
				{
					m_iTexture[i] = n;
					m_strTexture[i] = m_pszUniversalTextures[m_iTexture[i]];
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::Spacing();
 	}
}

void Widget_EffectMaker_Mesh::Option_Color()
{
	ImGuiColorEditFlags ColorEdit_flags = 0 | ImGuiColorEditFlags_AlphaBar;	// RGB, Alpha Bar

	// Start Color 
	ImGui::SeparatorText("Color");

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
		ImGui::ColorEdit4("Dest Color##ChangingColor", (float*)&m_vDestColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		if (m_bGradationOn && 0 == m_iGradationOption)
			ImGui::ColorEdit4("Gradiant Dest Color##ChangingColor", (float*)&m_vDestGradationColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
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

	_float fNoise = _float(rand() % 11) / 10.f;
	Color vRangStartColor = Color(m_vRangeStartColor.x, m_vRangeStartColor.y, m_vRangeStartColor.z, m_vRangeStartColor.w);
	Color vRangEndColor = Color(m_vRangeEndColor.x, m_vRangeEndColor.y, m_vRangeEndColor.z, m_vRangeEndColor.w);
	Color vFinal_StartColor = vRangStartColor * fNoise + vRangEndColor * (1.f - fNoise);

	MeshEffect::DESC tMeshEffectDesc
	{
		m_szTag,
		m_fDuration,
		m_bBlurOn,

		m_strMesh,

		m_strTexture[0], m_strTexture[1], m_strTexture[2], m_strTexture[3],
		m_strTexture[4], m_strTexture[5], m_strTexture[6], m_strTexture[7],
			
		vFinal_StartColor,
		
		m_iChangingColorOption,
		Color(m_vDestColor.x, m_vDestColor.y, m_vDestColor.z, m_vDestColor.w),

		m_iGradationOption,
		m_fGradationIntensity,
		Color(m_vGradationColor.x, m_vGradationColor.y, m_vGradationColor.z, m_vGradationColor.w),
		Color(m_vDestGradationColor.x, m_vDestGradationColor.y, m_vDestGradationColor.z, m_vDestGradationColor.w),

		m_bUseFadeOut
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
