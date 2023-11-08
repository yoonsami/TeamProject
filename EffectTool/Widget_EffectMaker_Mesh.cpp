#include "pch.h"
#include "Widget_EffectMaker_Mesh.h"

/* Components */
#include "MeshEffect.h"
#include "Texture.h"

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
		Utils::DetachExt(tag);
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
	Option_TextureUV();

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
	ImGui::Spacing();

	ImGui::Checkbox("On Fade Out", &m_bUseFadeOut);
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
	
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##TextureChild1", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

		for (_int i = 1; i < m_iNumTextureTypes; i++)
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

						m_iCurrEditingTextureType = i;
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Spacing();
		}
		ImGui::EndChild();
	}

	ImGui::SameLine();

	// Child 2: rounded border
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		ImGui::BeginChild("##TextureChild2", ImVec2(0, 260), true, window_flags);
		wstring wstrKey = Utils::ToWString(m_strTexture[m_iCurrEditingTextureType]);
		wstring wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
		Utils::DetachExt(wstrKey);
		auto pTexture = RESOURCES.GetOrAddTexture(wstrKey, wstrPath);
		if (nullptr != pTexture)
			ImGui::Image((pTexture->Get_SRV().Get()), ImVec2(120, 120));

		ImGui::EndChild();
	}
}

void Widget_EffectMaker_Mesh::Option_Color()
{
	ImGuiColorEditFlags ColorEdit_flags = 0 | ImGuiColorEditFlags_AlphaBar;

	/* Base Color */
	ImGui::SeparatorText("Base Color");
	const char* pszItem_BaseColor[] = {"Static Color", "Random in range"};
	if (ImGui::BeginCombo("Option##BaseColor", pszItem_BaseColor[m_iBaseColorOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_BaseColor); n++)
		{
			const bool is_selected = (m_iBaseColorOption == n);
			if (ImGui::Selectable(pszItem_BaseColor[n], is_selected))
				m_iBaseColorOption = n;
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	switch (m_iBaseColorOption)
	{
	case 0 : 
		ImGui::ColorEdit4("Base Color", (float*)&m_vBaseColor_RangeStart, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		m_vBaseColor_RangeEnd = m_vBaseColor_RangeStart;
		break;
	case 1:
		ImGui::ColorEdit4("Range Start Color", (float*)&m_vBaseColor_RangeStart, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		ImGui::ColorEdit4("Range End Color", (float*)&m_vBaseColor_RangeEnd, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		break;
	}
	ImGui::Spacing();

	/* Gradation */
	ImGui::SeparatorText("Gradation");
	ImGui::Checkbox("Gradation On", &m_bGradationOn);
	if (m_bGradationOn)
	{
		ImGui::ColorEdit4("Gradation Color", (float*)&m_vGradationColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		ImGui::SliderFloat("Intensity##Gradation", &m_fGradationIntensity, 0.1f, 0.9f);
	}
	ImGui::Spacing();

	/* Overlay */
	ImGui::SeparatorText("Overlay");
	ImGui::Checkbox("Overlay On", &m_bOverlayOn);
	if (m_bOverlayOn)
	{
		const char* pszItem_OverlayColor[] = { "One Color", "Gradation" };
		if (ImGui::BeginCombo("Option##OverlayColor", pszItem_OverlayColor[m_iOverlayOption], 0))
		{
			for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_OverlayColor); n++)
			{
				const bool is_selected = (m_iOverlayOption == n);
				if (ImGui::Selectable(pszItem_OverlayColor[n], is_selected))
					m_iOverlayOption = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		switch (m_iOverlayOption)
		{
		case 0:
			ImGui::ColorEdit4("Overlay Color", (float*)&m_vOverlayColor_Start, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
			m_vOverlayColor_End = m_vOverlayColor_Start;
			break;
		case 1:
			ImGui::ColorEdit4("Overlay Start Color##Overlay", (float*)&m_vOverlayColor_Start, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
			ImGui::ColorEdit4("Overlay End Color##Overlay", (float*)&m_vOverlayColor_End, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
			break;
		}
	}
	ImGui::Spacing();

	/* Changing Color */
	ImGui::SeparatorText("Color Changing");
	ImGui::Checkbox("Color Changing On", &m_bChangingColorOn);
	ImGui::Spacing();
	if (m_bChangingColorOn)
	{
		// Dest Base Color 
		ImGui::ColorEdit4("Dest Base Color", (float*)&m_vDestBaseColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

		// Dest Gradation Color 
		if (m_bGradationOn)
			ImGui::ColorEdit4("Dest Gradation Color", (float*)&m_vDestGradationColor, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

		// Dest Overlay Color
		if (m_bOverlayOn)
		{
			switch (m_iOverlayOption)
			{
			case 0:
				ImGui::ColorEdit4("Dest Overlay Color", (float*)&m_vDestOverlayColor_Start, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
				m_vDestOverlayColor_End = m_vDestOverlayColor_Start;
				break;
			case 1:
				ImGui::ColorEdit4("Dest Overlay Start Color##Overlay", (float*)&m_vDestOverlayColor_Start, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
				ImGui::ColorEdit4("Dest Overlay End Color##Overlay", (float*)&m_vDestOverlayColor_End, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
				break;
			}
		}
	}
	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_TextureUV()
{
	ImGui::SeparatorText("Texture UV position");

	ImGui::InputFloat2("Tiling##TextureUV", m_fTexcoordTiling);
	ImGui::InputFloat2("Move Speed##TextureUV", m_fTexcoordSpeed);
}

void Widget_EffectMaker_Mesh::Create()
{
	// For. Create GameObject 
	shared_ptr<GameObject> EffectObj = make_shared<GameObject>();

	// For. Add and Setting Transform Component
	EffectObj->GetOrAddTransform();
	EffectObj->Get_Transform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));
	EffectObj->Get_Transform()->Scaled(_float3(1.f));

	// For. Add and Setting Effect Component to GameObject
	shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Effect2.fx");
	shared_ptr<MeshEffect> meshEffect = make_shared<MeshEffect>(shader);
	EffectObj->Add_Component(meshEffect);

	_float fNoise = _float(rand() % 11) / 10.f;
	Color vRangStartColor = Color(m_vBaseColor_RangeStart.x, m_vBaseColor_RangeStart.y, m_vBaseColor_RangeStart.z, m_vBaseColor_RangeStart.w);
	Color vRangEndColor = Color(m_vBaseColor_RangeEnd.x, m_vBaseColor_RangeEnd.y, m_vBaseColor_RangeEnd.z, m_vBaseColor_RangeEnd.w);
	Color vBaseColor = vRangStartColor * fNoise + vRangEndColor * (1.f - fNoise);
	
	if (!m_bChangingColorOn)
	{
		m_vDestBaseColor = ImVec4(vBaseColor.x, vBaseColor.y, vBaseColor.z, vBaseColor.w);
		m_vDestGradationColor = m_vGradationColor;
		m_vDestOverlayColor_Start = m_vOverlayColor_Start;
		m_vDestOverlayColor_End = m_vOverlayColor_End;
	}

	MeshEffect::DESC tMeshEffectDesc
	{
		m_szTag,
		m_fDuration,
		m_bBlurOn,
		m_bUseFadeOut,

		m_strMesh,

		m_strTexture[0], m_strTexture[1], m_strTexture[2], m_strTexture[3],
		m_strTexture[4], m_strTexture[5], m_strTexture[6], m_strTexture[7],
			
		vBaseColor,

		m_bGradationOn,
		m_fGradationIntensity,
		Color(m_vGradationColor.x, m_vGradationColor.y, m_vGradationColor.z, m_vGradationColor.w),
	
		m_bOverlayOn,
		Color(m_vOverlayColor_Start.x, m_vOverlayColor_Start.y, m_vOverlayColor_Start.z, m_vOverlayColor_Start.w),
		Color(m_vOverlayColor_End.x, m_vOverlayColor_End.y, m_vOverlayColor_End.z, m_vOverlayColor_End.w),

		m_bChangingColorOn,
		Color(m_vDestBaseColor.x, m_vDestBaseColor.y, m_vDestBaseColor.z, m_vDestBaseColor.w),
		Color(m_vDestGradationColor.x, m_vDestGradationColor.y, m_vDestGradationColor.z, m_vDestGradationColor.w),
		Color(m_vOverlayColor_Start.x, m_vOverlayColor_Start.y, m_vOverlayColor_Start.z, m_vOverlayColor_Start.w),
		Color(m_vOverlayColor_End.x, m_vOverlayColor_End.y, m_vOverlayColor_End.z, m_vOverlayColor_End.w),

		_float2(m_fTexcoordSpeed[0], m_fTexcoordSpeed[1]),
		_float2(m_fTexcoordTiling[0], m_fTexcoordTiling[1])
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
