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
}

void Widget_EffectMaker_Mesh::ImGui_EffectMaker()
{
	Option_Property();
	Option_Mesh();

	Option_Opacity();
	Option_Diffuse();
	Option_Normal();
	Option_AlphaGradation();
	Option_Gradation();
	Option_Overlay();
	Option_Dissolve();
	Option_Distortion();

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

void Widget_EffectMaker_Mesh::Option_Diffuse()
{
	ImGui::SeparatorText("Diffuse");

	// For. Diffuse Option 
	const char* pszItem_Option[] = { "Use Texture color ", "custom(const)", "custom(random in range)", };
	if (ImGui::BeginCombo("Diffuse Option##Diffuse", pszItem_Option[m_iDiffuseOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_Option); n++)
		{
			const bool is_selected = (m_iDiffuseOption == n);
			if (ImGui::Selectable(pszItem_Option[n], is_selected))
				m_iDiffuseOption = n;
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##Child1_Diffuse", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

		switch (m_iDiffuseOption)
		{
		case 0: 
			// For. Texture 
			ImGui::Text("Texture##Diffuse");
			SubWidget_TextureCombo(&m_DiffuseTexture.first, &m_DiffuseTexture.second, "##TexCB_Diffuse");

		case 1:
			ImGui::ColorEdit4("Color##Diffuse", (float*)&m_vDiffuseColor_BaseStart, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
			m_vDiffuseColor_BaseEnd = m_vDiffuseColor_BaseStart;
			break;
		case 2:
			ImGui::ColorEdit4("Range Color1##Diffuse", (float*)&m_vDiffuseColor_BaseStart, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
			ImGui::ColorEdit4("Range Color2##Diffuse", (float*)&m_vDiffuseColor_BaseEnd, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
			break;
		default:
			break;
		}

		if (m_bColorChangingOn)
			ImGui::ColorEdit4("Dest Color##Diffuse", (float*)&m_vDiffuseColor_Dest, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

		ImGui::EndChild();
	}

	ImGui::SameLine();
	
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		ImGui::BeginChild("##Child2_Diffuse", ImVec2(0, 260), true, window_flags);

		SubWidget_ImageViewer(m_DiffuseTexture.second, m_strTexturePath, "##Img_Diffuse");

		ImGui::EndChild();
	}	

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Opacity()
{
	ImGui::SeparatorText("Opacity");

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##Child1_Opacity", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);
		
		// For. Texture 
		ImGui::Text("Texture##Opacity");
		SubWidget_TextureCombo(&m_OpacityTexture.first, &m_OpacityTexture.second, "##TexCB_Opacity");

		// For. Sampler 
		const char* pszItem_Sampler[] = { "Wrap", "Clamp", "Mirror", "Border" };
		if (ImGui::BeginCombo("Sampler##Opacity", pszItem_Sampler[m_iSamplerType], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_Sampler); n++)
		{
			const bool is_selected = (m_iSamplerType == n);
			if (ImGui::Selectable(pszItem_Sampler[n], is_selected))
				m_iSamplerType = n;
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	
		ImGui::EndChild();
	}

	ImGui::SameLine();
	
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##Child2_Opacity", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

		SubWidget_ImageViewer(m_OpacityTexture.second, m_strTexturePath, "##Img_Opacity");

		ImGui::EndChild();
	}

	// For. Tiling, move Texture UV Speed 
	SubWidget_SettingTexUV(m_fTiling_Opacity, m_fUVSpeed_Opacity, "Tiling##Opacity", "Move TexUV Speed##Opacity");

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Normal()
{
	ImGui::SeparatorText("Normal");

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##Child1_Normal", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

		// For. Texture 
		ImGui::Text("Texture##Normal");
		SubWidget_TextureCombo(&m_NormalTexture.first, &m_NormalTexture.second, "##TexCB_Normal");

		ImGui::EndChild();
	}

	ImGui::SameLine();

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##Child2_Normal", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

		SubWidget_ImageViewer(m_NormalTexture.second, m_strNormalTexturePath, "##Img_Normal");

		ImGui::EndChild();
	}

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_AlphaGradation()
{
	ImGui::SeparatorText("Alpha Gradation");

	ImGui::SliderFloat("Intensity", &m_fAlphaGraIntensity, 0.f, 1.f);
	ImGui::ColorEdit4("Color##Diffuse", (float*)&m_vAlphaGraColor_Base, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

	if (m_bColorChangingOn)
	{
		ImGui::ColorEdit4("Dest Color##Diffuse", (float*)&m_vAlphaGraColor_Dest, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		if (ImGui::Checkbox("Same with Base color##AlphaGra", &m_bDestSameWithBase_AlphaGra))
		{
			if (m_bDestSameWithBase_AlphaGra)
				m_vAlphaGraColor_Dest = m_vAlphaGraColor_Base;
		}
	}

	ImGui::SameLine();
}

void Widget_EffectMaker_Mesh::Option_Gradation()
{
	ImGui::SeparatorText("Gradation");

	ImGui::Checkbox("Gradation On", &m_bGra_On);

	if (m_bGra_On)
	{
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("##Child1_Gra", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

			// For. Texture 
			ImGui::Text("Texture##Gra");
			SubWidget_TextureCombo(&m_GraTexture.first, &m_GraTexture.second, "##TexCB_Gra");

			// For. Color 
			ImGui::ColorEdit4("Color##Gra", (float*)&m_vGraColor_Base, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

			if (m_bColorChangingOn)
			{
				ImGui::ColorEdit4("Color##Gra", (float*)&m_vGraColor_Dest, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
				if (ImGui::Checkbox("Same with Base color##Gra", &m_bDestSameWithBase_Gra))
				{
					if (m_bDestSameWithBase_Gra)
						m_vGraColor_Dest = m_vGraColor_Base;
				}
			}

			ImGui::Checkbox("UV Option same with opacity##Opacity", &m_bUVOptionSameWithOpacity_Gra);
			if (m_bUVOptionSameWithOpacity_Gra)
			{
				m_fTiling_Gra[0] = m_fTiling_Opacity[0];
				m_fTiling_Gra[1] = m_fTiling_Opacity[0];
				m_fUVSpeed_Gra[0] = m_fUVSpeed_Opacity[0];
				m_fUVSpeed_Gra[1] = m_fUVSpeed_Opacity[0];
			}
			SubWidget_SettingTexUV(m_fTiling_Gra, m_fUVSpeed_Gra, "Tiling##Gra", "Move TexUV Speed##Gra");

			ImGui::EndChild();
		}

		ImGui::SameLine();

		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("##Child2_Gra", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

			SubWidget_ImageViewer(m_GraTexture.second, m_strTexturePath, "##Img_Gra");

			ImGui::EndChild();
		}
	}

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Overlay()
{
	ImGui::SeparatorText("Overlay");

	ImGui::Checkbox("Overlay On", &m_bOverlay_On);

	if (m_bOverlay_On)
	{
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("##Child1_Overlay", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

			// For. Texture 
			ImGui::Text("Texture##Overlay");
			SubWidget_TextureCombo(&m_OverlayTexture.first, &m_OverlayTexture.second, "##TexCB_Overlay");

			// For. Color 
			ImGui::ColorEdit4("Color##Overlay", (float*)&m_vOverlayColor_Base, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);		

			ImGui::Checkbox("UV Option same with opacity##Overlay", &m_bUVOptionSameWithOpacity_Overlay);
			if (m_bUVOptionSameWithOpacity_Overlay)
			{
				m_fTiling_Overlay[0] = m_fTiling_Opacity[0];
				m_fTiling_Overlay[1] = m_fTiling_Opacity[0];
				m_fUVSpeed_Overlay[0] = m_fUVSpeed_Opacity[0];
				m_fUVSpeed_Overlay[1] = m_fUVSpeed_Opacity[0];
			}
			SubWidget_SettingTexUV(m_fTiling_Overlay, m_fUVSpeed_Overlay, "Tiling##Overlay", "Move TexUV Speed##Overlay");

			ImGui::EndChild();
		}

		ImGui::SameLine();

		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("##Child2_Overlay", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

			SubWidget_ImageViewer(m_OverlayTexture.second, m_strTexturePath, "##Img_Overlay");

			ImGui::EndChild();
		}
	}

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Dissolve()
{
	ImGui::SeparatorText("Dissolve");

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##Child1_Dissolve", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

		// For. Texture 
		ImGui::Text("Texture##Dissolve");
		SubWidget_TextureCombo(&m_DissolveTexture.first, &m_DissolveTexture.second, "##TexCB_Dissolve");

		ImGui::Checkbox("UV Option same with opacity##Dissolve", &m_bUVOptionSameWithOpacity_Dissolve);
		if (m_bUVOptionSameWithOpacity_Dissolve)
		{
			m_fTiling_Dissolve[0] = m_fTiling_Opacity[0];
			m_fTiling_Dissolve[1] = m_fTiling_Opacity[0];
			m_fUVSpeed_Dissolve[0] = m_fUVSpeed_Opacity[0];
			m_fUVSpeed_Dissolve[1] = m_fUVSpeed_Opacity[0];
		}
		SubWidget_SettingTexUV(m_fTiling_Dissolve, m_fUVSpeed_Dissolve, "Tiling##Dissolve", "Move TexUV Speed##Dissolve");

		ImGui::EndChild();
	}

	ImGui::SameLine();

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##Child2_Dissolve", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

		SubWidget_ImageViewer(m_DissolveTexture.second, m_strTexturePath, "##Img_Dissolve");

		ImGui::EndChild();
	}

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Distortion()
{
	ImGui::SeparatorText("Distortion");

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##Child1_Distortion", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

		// For. Texture 
		ImGui::Text("Texture##Distortion");
		SubWidget_TextureCombo(&m_DistortionTexture.first, &m_DistortionTexture.second, "##TexCB_Distortion");

		ImGui::Checkbox("UV Option same with opacity##Distortion", &m_bUVOptionSameWithOpacity_Distortion);
		if (m_bUVOptionSameWithOpacity_Dissolve)
		{
			m_fTiling_Distortion[0] = m_fTiling_Opacity[0];
			m_fTiling_Distortion[1] = m_fTiling_Opacity[0];
			m_fUVSpeed_Distortion[0] = m_fUVSpeed_Opacity[0];
			m_fUVSpeed_Distortion[1] = m_fUVSpeed_Opacity[0];
		}
		SubWidget_SettingTexUV(m_fTiling_Distortion, m_fUVSpeed_Distortion, "Tiling##Distortion", "Move TexUV Speed##Distortion");

		ImGui::EndChild();
	}

	ImGui::SameLine();

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("##Child2_Distortion", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

		SubWidget_ImageViewer(m_DistortionTexture.second, m_strTexturePath, "##Img_Distortion");

		ImGui::EndChild();
	}

	ImGui::Spacing();
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
	Color vRangStartColor = Color(m_vDiffuseColor_BaseStart.x, m_vDiffuseColor_BaseStart.y, m_vDiffuseColor_BaseStart.z, m_vDiffuseColor_BaseStart.w);
	Color vRangEndColor = Color(m_vDiffuseColor_BaseEnd.x, m_vDiffuseColor_BaseEnd.y, m_vDiffuseColor_BaseEnd.z, m_vDiffuseColor_BaseEnd.w);
	Color vDiffuseColor_Base = vRangStartColor * fNoise + vRangEndColor * (1.f - fNoise);
	
	if (!m_bColorChangingOn)
	{
		m_vDiffuseColor_Dest = ImVec4(vDiffuseColor_Base.x, vDiffuseColor_Base.y, vDiffuseColor_Base.z, vDiffuseColor_Base.w);
		m_vAlphaGraColor_Dest = m_vAlphaGraColor_Base;
		m_vGraColor_Dest = m_vGraColor_Base;
	}

	MeshEffect::DESC tMeshEffectDesc
	{
		m_szTag,
		m_fDuration,
		m_bBlurOn,
		m_bUseFadeOut,

		m_strMesh,

		m_bColorChangingOn,

		m_DiffuseTexture.second,
		vDiffuseColor_Base,
		ImVec4toColor(m_vDiffuseColor_Dest),

		m_OpacityTexture.second,
		m_iSamplerType,
		_float2(m_fTiling_Opacity[0], m_fTiling_Opacity[1]),
		_float2(m_fUVSpeed_Opacity[0], m_fUVSpeed_Opacity[1]),

		m_fAlphaGraIntensity,
		ImVec4toColor(m_vAlphaGraColor_Base),
		ImVec4toColor(m_vAlphaGraColor_Dest),

		m_GraTexture.second,
		ImVec4toColor(m_vGraColor_Base),
		ImVec4toColor(m_vGraColor_Dest),
		_float2(m_fTiling_Gra[0], m_fTiling_Gra[1]),
		_float2(m_fUVSpeed_Gra[0], m_fUVSpeed_Gra[1]),

		m_bOverlay_On,
		m_OverlayTexture.second,
		ImVec4toColor(m_vOverlayColor_Base),
		_float2(m_fTiling_Overlay[0], m_fTiling_Overlay[1]),
		_float2(m_fUVSpeed_Overlay[0], m_fUVSpeed_Overlay[1]),

		m_NormalTexture.second,

		m_DissolveTexture.second,
		_float2(m_fTiling_Overlay[0], m_fTiling_Overlay[1]),
		_float2(m_fUVSpeed_Overlay[0], m_fUVSpeed_Overlay[1]),

		m_DistortionTexture.second,
		_float2(m_fTiling_Distortion[0], m_fTiling_Distortion[1]),
		_float2(m_fUVSpeed_Distortion[0], m_fUVSpeed_Distortion[1]),

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

void Widget_EffectMaker_Mesh::SubWidget_TextureCombo(_int* iSelected, string* strSelected, const char* pszWidgetKey)
{
	if (ImGui::BeginCombo(pszWidgetKey, m_pszUniversalTextures[*iSelected], 0))
	{
		for (_uint n = 0; n < m_iNumUniversalTextures; n++)
		{
			const bool is_selected = (*iSelected == n);
			if (ImGui::Selectable(m_pszUniversalTextures[n], is_selected))
			{
				*iSelected = n;
				*strSelected = m_pszUniversalTextures[*iSelected];
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void Widget_EffectMaker_Mesh::SubWidget_ImageViewer(string strFileName, string strFilePath, const char* pszWidgetKey)
{
	wstring wstrKey = Utils::ToWString(strFileName);
	wstring wstrPath = Utils::ToWString(strFilePath) + wstrKey;
	Utils::DetachExt(wstrKey);

	auto pTexture = RESOURCES.GetOrAddTexture(wstrKey, wstrPath);
	if (nullptr != pTexture)
		ImGui::Image((pTexture->Get_SRV().Get()), ImVec2(120, 120));
}

void Widget_EffectMaker_Mesh::SubWidget_SettingTexUV(_float* arrTiling, _float* arrTexUVSpeed, const char* pszWidgetKey1, const char* pszWidgetKey2)
{
	ImGui::SliderFloat2(pszWidgetKey1, arrTiling, 0.00f, 1.00f);
	ImGui::SliderFloat2(pszWidgetKey2, arrTexUVSpeed, 0.00f, 1.00f);
}

Color Widget_EffectMaker_Mesh::ImVec4toColor(ImVec4 imvec)
{
	return Color(imvec.x, imvec.y, imvec.z, imvec.w);
}
