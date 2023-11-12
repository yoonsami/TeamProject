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
	Set_FinishedEffect_List();
}

void Widget_EffectMaker_Mesh::Tick()
{
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
	ImGui::Begin("Mesh Effect Maker");
	ImGui_EffectMaker();
	ImGui::End();

	ImGui::Begin("Finished Effect");
	ImGui_FinishedEffect();
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

void Widget_EffectMaker_Mesh::Set_FinishedEffect_List()
{
	// For. Clear list
	m_iNumFinishedEffects = 0 ;
	m_vecFinishedEffects.clear();
	delete(m_pszFinishedEffects);

	// For. add effect to list 
	wstring assetPath = L"..\\Resources\\EffectData\\MeshEffectData\\";
	for (auto& entry : fs::recursive_directory_iterator(assetPath))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension().wstring() != L".dat")
			continue;

		string tag = entry.path().string();
		tag = entry.path().filename().string();
		m_vecFinishedEffects.push_back(tag);
	}

	m_iNumFinishedEffects = (_uint)m_vecFinishedEffects.size();
	m_pszFinishedEffects = new const char* [m_iNumFinishedEffects];

	int iIndex = 0;
	for (auto iter : m_vecFinishedEffects)
	{
		m_pszFinishedEffects[iIndex] = m_vecFinishedEffects[iIndex].c_str();
		iIndex++;
	}
}

void Widget_EffectMaker_Mesh::ImGui_EffectMaker()
{
	Option_Property();
	Option_Mesh();

	Option_Opacity();
	Option_Blend();
	Option_Diffuse();
	Option_Normal();
	Option_AlphaGradation();
	Option_Gradation();
	Option_Overlay();
	Option_Dissolve();
	Option_Distortion();
	Option_ColorEdit();

	/* For. Create, Save, Load Effect */
	ImGui::Spacing();
	ImGui::SeparatorText("Create / Load");
	if (ImGui::Button("Create"))
		Create();
	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		// For. Update file data 
		Save();	
	}
	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::ImGui_FinishedEffect()
{
	ImGui::SeparatorText("Effect List");
	ImGui::ListBox("##FinishedEffect", &m_iFinishedObject, m_pszFinishedEffects, m_iNumFinishedEffects, 10);
	ImGui::Spacing();

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
			{
				m_iDiffuseOption = n;

				if (0 != m_iDiffuseOption)
				{
					m_DiffuseTexture.first = 0;
					m_DiffuseTexture.second = "None";
				}
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		ImGui::BeginChild("##Child1_Diffuse", ImVec2(ImGui::GetContentRegionAvail().x - 100, 150), false, window_flags);

		switch (m_iDiffuseOption)
		{
		case 0: 
			// For. Texture 
			SubWidget_TextureCombo(&m_DiffuseTexture.first, &m_DiffuseTexture.second, m_strTexturePath,"Texture##Diffuse");
			break;
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
		ImGui::BeginChild("##Child2_Diffuse", ImVec2(80, 100), false, window_flags);
		SubWidget_ImageViewer(m_DiffuseTexture.second, m_strTexturePath, "##Img_Diffuse");

		ImGui::EndChild();
	}	

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Opacity()
{
	ImGui::SeparatorText("Opacity");

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		ImGui::BeginChild("##Child1_Opacity", ImVec2(ImGui::GetContentRegionAvail().x - 100, 200), false, window_flags);
		
		// For. Texture 
		SubWidget_TextureCombo(&m_OpacityTexture.first, &m_OpacityTexture.second, m_strTexturePath, "Texture##Opacity");
		ImGui::Spacing();
	
		ImGui::EndChild();
	}
	ImGui::SameLine();
	{
		ImGui::BeginChild("##Child2_Opacity", ImVec2(80, 200), false);

		SubWidget_ImageViewer(m_OpacityTexture.second, m_strTexturePath, "##Img_Opacity");

		ImGui::EndChild();
	}
	ImGui::Spacing();

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
	ImGui::Spacing();

	// For. Tiling, move Texture UV Speed 
	SubWidget_SettingTexUV(m_fTiling_Opacity, m_fUVSpeed_Opacity, "Tiling(x,y)##Opacity", "Move TexUV Speed(x,y)##Opacity");

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Blend()
{
	ImGui::SeparatorText("Blend");

	ImGui::Checkbox("Blend On", &m_bBlend_On);

	if (m_bBlend_On)
	{
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("##Child1_Blend", ImVec2(ImGui::GetContentRegionAvail().x - 100, 180), false, window_flags);

			// For. Texture 
			SubWidget_TextureCombo(&m_BlendTexture.first, &m_BlendTexture.second, m_strTexturePath, "Texture##Blend");

			ImGui::EndChild();
		}

		ImGui::SameLine();

		{
			ImGui::BeginChild("##Child2_Blend", ImVec2(80, 100), false);

			SubWidget_ImageViewer(m_BlendTexture.second, m_strTexturePath, "##Img_Blend");

			ImGui::EndChild();
		}
	}
	else
	{
		m_BlendTexture.first = 0;
		m_BlendTexture.second = "None";
	}

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Normal()
{
	ImGui::SeparatorText("Normal");

	ImGui::Checkbox("Normal On", &m_bNormal_On);

	if (m_bNormal_On)
	{
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("##Child1_Normal", ImVec2(ImGui::GetContentRegionAvail().x - 100, 180), false, window_flags);

			// For. Texture 
			SubWidget_TextureCombo(&m_NormalTexture.first, &m_NormalTexture.second, m_strTexturePath, "Texture##Normal");

			ImGui::EndChild();
		}

		ImGui::SameLine();

		{
			ImGui::BeginChild("##Child2_Normal", ImVec2(80, 100), false);

			SubWidget_ImageViewer(m_NormalTexture.second, m_strTexturePath, "##Img_Normal");

			ImGui::EndChild();
		}
	}
	else
	{
		m_NormalTexture.first = 0;
		m_NormalTexture.second = "None";
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

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Gradation()
{
	ImGui::SeparatorText("Gradation");

	ImGui::Checkbox("Gradation On", &m_bGra_On);

	if (m_bGra_On)
	{
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("##Child1_Gra", ImVec2(ImGui::GetContentRegionAvail().x - 100, 260), false, window_flags);

			// For. Texture 
			SubWidget_TextureCombo(&m_GraTexture.first, &m_GraTexture.second, m_strTexturePath, "Texture##Gra");

			// For. Color 
			ImGui::ColorEdit4("Color##Gra", (float*)&m_vGraColor_Base, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

			if (m_bColorChangingOn)
			{
				ImGui::ColorEdit4("Dest Color##Gra", (float*)&m_vGraColor_Dest, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
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
				m_fTiling_Gra[1] = m_fTiling_Opacity[1];
				m_fUVSpeed_Gra[0] = m_fUVSpeed_Opacity[0];
				m_fUVSpeed_Gra[1] = m_fUVSpeed_Opacity[1];
			}
			SubWidget_SettingTexUV(m_fTiling_Gra, m_fUVSpeed_Gra, "Tiling(x,y)##Gra", "Move TexUV Speed(x,y)##Gra");

			ImGui::EndChild();
		}

		ImGui::SameLine();

		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("##Child2_Gra", ImVec2(80, 100), false, window_flags);

			SubWidget_ImageViewer(m_GraTexture.second, m_strTexturePath, "##Img_Gra");

			ImGui::EndChild();
		}
	}
	else
	{
		m_GraTexture.first = 0;
		m_GraTexture.second = "None";
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
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("##Child1_Overlay", ImVec2(ImGui::GetContentRegionAvail().x - 100, 260), false, window_flags);

			// For. Texture 
			SubWidget_TextureCombo(&m_OverlayTexture.first, &m_OverlayTexture.second, m_strTexturePath, "Texture##Overlay");

			// For. Color 
			ImGui::ColorEdit4("Color##Overlay", (float*)&m_vOverlayColor_Base, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);		

			ImGui::Checkbox("UV Option same with opacity##Overlay", &m_bUVOptionSameWithOpacity_Overlay);
			if (m_bUVOptionSameWithOpacity_Overlay)
			{
				m_fTiling_Overlay[0] = m_fTiling_Opacity[0];
				m_fTiling_Overlay[1] = m_fTiling_Opacity[1];
				m_fUVSpeed_Overlay[0] = m_fUVSpeed_Opacity[0];
				m_fUVSpeed_Overlay[1] = m_fUVSpeed_Opacity[1];
			}
			SubWidget_SettingTexUV(m_fTiling_Overlay, m_fUVSpeed_Overlay, "Tiling(x,y)##Overlay", "Move TexUV Speed##(x,y)Overlay");

			ImGui::EndChild();
		}

		ImGui::SameLine();

		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("##Child2_Overlay", ImVec2(80, 100), false, window_flags);

			SubWidget_ImageViewer(m_OverlayTexture.second, m_strTexturePath, "##Img_Overlay");

			ImGui::EndChild();
		}
	}
	else
	{
		m_OverlayTexture.first = 0;
		m_OverlayTexture.second = "None";
	}

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Dissolve()
{
	ImGui::SeparatorText("Dissolve");

	ImGui::Checkbox("Dissolve On", &m_bDissolve_On);

	if (m_bDissolve_On)
	{
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("##Child1_Dissolve", ImVec2(ImGui::GetContentRegionAvail().x - 100, 250), false, window_flags);

			// For. Inverse 
			ImGui::Checkbox("Inverse##Dissolve", &m_bDissolveInverse);

			// For. Texture 
			SubWidget_TextureCombo(&m_DissolveTexture.first, &m_DissolveTexture.second, m_strTexturePath, "Texture##Dissolve");

			ImGui::Checkbox("UV Option same with opacity##Dissolve", &m_bUVOptionSameWithOpacity_Dissolve);
			if (m_bUVOptionSameWithOpacity_Dissolve)
			{
				m_fTiling_Dissolve[0] = m_fTiling_Opacity[0];
				m_fTiling_Dissolve[1] = m_fTiling_Opacity[1];
				m_fUVSpeed_Dissolve[0] = m_fUVSpeed_Opacity[0];
				m_fUVSpeed_Dissolve[1] = m_fUVSpeed_Opacity[1];
			}
			SubWidget_SettingTexUV(m_fTiling_Dissolve, m_fUVSpeed_Dissolve, "Tiling(x,y)##Dissolve", "Move TexUV Speed(x,y)##Dissolve");

			ImGui::EndChild();
		}

		ImGui::SameLine();

		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("##Child2_Dissolve", ImVec2(80, 100), false, window_flags);

			SubWidget_ImageViewer(m_DissolveTexture.second, m_strTexturePath, "##Img_Dissolve");

			ImGui::EndChild();
		}
	}
	else
	{
		m_DissolveTexture.first = 0;
		m_DissolveTexture.second = "None";
	}

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Distortion()
{
	ImGui::SeparatorText("Distortion");

	ImGui::Checkbox("Distortion On", &m_bDistortion_On);

	if (m_bDistortion_On)
	{
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("##Child1_Distortion", ImVec2(ImGui::GetContentRegionAvail().x - 100, 250), false, window_flags);

			// For. Texture 
			SubWidget_TextureCombo(&m_DistortionTexture.first, &m_DistortionTexture.second, m_strTexturePath, "Texture##Distortion");

			ImGui::Checkbox("UV Option same with opacity##Distortion", &m_bUVOptionSameWithOpacity_Distortion);
			if (m_bUVOptionSameWithOpacity_Dissolve)
			{
				m_fTiling_Distortion[0] = m_fTiling_Opacity[0];
				m_fTiling_Distortion[1] = m_fTiling_Opacity[1];
				m_fUVSpeed_Distortion[0] = m_fUVSpeed_Opacity[0];
				m_fUVSpeed_Distortion[1] = m_fUVSpeed_Opacity[1];
			}
			SubWidget_SettingTexUV(m_fTiling_Distortion, m_fUVSpeed_Distortion, "Tiling(x,y)##Distortion", "Move TexUV Speed(x,y)##Distortion");

			ImGui::EndChild();
		}

		ImGui::SameLine();

		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("##Child2_Distortion", ImVec2(80, 100), false, window_flags);

			SubWidget_ImageViewer(m_DistortionTexture.second, m_strTexturePath, "##Img_Distortion");

			ImGui::EndChild();
		}
	}
	else
	{
		m_DistortionTexture.first = 0;
		m_DistortionTexture.second = "None";
	}

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_ColorEdit()
{
	ImGui::SeparatorText("Final Color Editor");

	ImGui::InputFloat("Contrast", &m_fContrast);
	
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

	if (!m_bColorChangingOn)
	{
		m_vDiffuseColor_Dest = m_vDiffuseColor_BaseStart;
		m_vAlphaGraColor_Dest = m_vAlphaGraColor_Base;
		m_vGraColor_Dest = m_vGraColor_Base;
	}

	MeshEffectData::DESC tMeshEffectDesc
	{
		m_szTag,
		m_fDuration,
		m_bBlurOn,
		m_bUseFadeOut,

		m_strMesh,

		m_bColorChangingOn,

		m_DiffuseTexture.second,
		ImVec4toColor(m_vDiffuseColor_BaseStart),
		ImVec4toColor(m_vDiffuseColor_BaseEnd),
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
		m_bDissolveInverse,

		m_DistortionTexture.second,
		_float2(m_fTiling_Distortion[0], m_fTiling_Distortion[1]),
		_float2(m_fUVSpeed_Distortion[0], m_fUVSpeed_Distortion[1]),

		m_BlendTexture.second,

		m_fContrast

	};
	EffectObj->Get_MeshEffect()->Init(&tMeshEffectDesc);

	// For. Add Effect GameObject to current scene
	CUR_SCENE->Add_GameObject(EffectObj);
}

void Widget_EffectMaker_Mesh::Save()
{
	string strFileName = m_szTag;
	string strFilePath = "..\\Resources\\EffectData\\MeshEffectData\\";
	strFilePath += strFileName + ".dat";

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(Utils::ToWString(strFilePath), FileMode::Write);

	/* Property */
	file->Write<string>(m_szTag);
	file->Write<_float>(m_fDuration);
	file->Write<_bool>(m_bBlurOn);
	file->Write<_bool>(m_bUseFadeOut);

	/* Mesh */
	// file->Write<_bool>(m_iMesh); 
	file->Write<string>(m_strMesh);

	/* Coloring Option */
	file->Write<_bool>(m_bColorChangingOn);

	/* Diffuse */
	// file->Write<_int>(m_DiffuseTexture.first);
	file->Write<string>(m_DiffuseTexture.second);
	file->Write<_float4>(ImVec4toColor(m_vDiffuseColor_BaseStart));
	file->Write<_float4>(ImVec4toColor(m_vDiffuseColor_BaseEnd));
	file->Write<_float4>(ImVec4toColor(m_vDiffuseColor_Dest));

	/* Alpha Gradation */
	file->Write<_float>(m_fAlphaGraIntensity);
	file->Write<_float4>(ImVec4toColor(m_vAlphaGraColor_Base));
	// file->Write<_bool>(m_bDestSameWithBase_AlphaGra);
	file->Write<_float4>(ImVec4toColor(m_vAlphaGraColor_Dest));

	/* Opacity */
	// file->Write<_int>(m_OpacityTexture.first);
	file->Write<string>(m_OpacityTexture.second);
	// file->Write<_bool>(m_bUVOptionSameWithOpacity_Opacity);
	file->Write<_int>(m_iSamplerType);
	file->Write<_float2>(_float2(m_fTiling_Opacity[0], m_fTiling_Opacity[1]));
	file->Write<_float2>(_float2(m_fUVSpeed_Opacity[0], m_fUVSpeed_Opacity[1]));

	/* Gradation by Texture */
	// file->Write<_bool>(m_bGra_On);
	// file->Write<_int>(m_GraTexture.first);
	file->Write<string>(m_GraTexture.second);
	file->Write<_float4>(ImVec4toColor(m_vGraColor_Base));
	// file->Write<_bool>(m_bUVOptionSameWithOpacity_Gra);
	file->Write<_float2>(_float2(m_fTiling_Gra[0], m_fTiling_Gra[1]));
	file->Write<_float2>(_float2(m_fUVSpeed_Gra[0], m_fUVSpeed_Gra[1]));
	// file->Write<_bool>(m_bDestSameWithBase_Gra);
	file->Write<_float4>(ImVec4toColor(m_vGraColor_Dest));

	/* Overlay */
	file->Write<_bool>(m_bOverlay_On);
	// file->Write<_int>(m_OverlayTexture.first);
	file->Write<string>(m_OverlayTexture.second);
	file->Write<_float4>(ImVec4toColor(m_vOverlayColor_Base));
	// file->Write<_bool>(m_bUVOptionSameWithOpacity_Overlay);
	file->Write<_float2>(_float2(m_fTiling_Overlay[0], m_fTiling_Overlay[1]));
	file->Write<_float2>(_float2(m_fUVSpeed_Overlay[0], m_fUVSpeed_Overlay[1]));

	/* Normal */
	// file->Write<_int>(m_NormalTexture.first);
	file->Write<string>(m_NormalTexture.second);

	/* Dissolve */
	// file->Write<_int>(m_DissolveTexture.first);
	file->Write<string>(m_DissolveTexture.second);
	// file->Write<_bool>(m_bUVOptionSameWithOpacity_Dissolve);
	file->Write<_float2>(_float2(m_fTiling_Dissolve[0], m_fTiling_Dissolve[1]));
	file->Write<_float2>(_float2(m_fUVSpeed_Dissolve[0], m_fUVSpeed_Dissolve[1]));
	file->Write<_bool>(m_bDissolveInverse);

	/* Distortion */
	// file->Write<_bool>(m_bDistortion_On);
	// file->Write<_int>(m_DistortionTexture.first);
	file->Write<string>(m_DistortionTexture.second);
	// file->Write<_bool>(m_bUVOptionSameWithOpacity_Distortion);
	file->Write<_float2>(_float2(m_fTiling_Distortion[0], m_fTiling_Distortion[1]));
	file->Write<_float2>(_float2(m_fUVSpeed_Distortion[0], m_fUVSpeed_Distortion[1]));

	/* Blend */
	// file->Write<_bool>(m_bBlend_On);
	// file->Write<_int>(m_BlendTexture.first);
	file->Write<string>(m_BlendTexture.second);

	/* Color Edit */
	file->Write<_float>(m_fContrast);

	RESOURCES.ReloadOrAddMeshEffectData(Utils::ToWString(strFileName), Utils::ToWString(strFilePath));
	
	// For. Update Finished Effect List 
	Set_FinishedEffect_List();

}

void Widget_EffectMaker_Mesh::Load()
{
	_float2 tiling = {0.f, 0.f};
	_float2 UVSpeed = { 0.f, 0.f };
	
	// For. load file and fill imgui 
	string strFilePath = "..\\Resources\\EffectData\\MeshEffectData\\";
	string strFileName = m_pszFinishedEffects[m_iFinishedObject];
	strFilePath += strFileName;
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(Utils::ToWString(strFilePath), FileMode::Read);

	/* Property */
	string strTag = file->Read<string>();
	strcpy_s(m_szTag, MAX_PATH, strTag.c_str());
	m_fDuration = file->Read<_float>();
	m_bBlurOn = file->Read<_bool>();
	m_bUseFadeOut = file->Read<_bool>();

	/* Mesh */
	m_strMesh = file->Read<string>();
	m_iMesh = GetIndex_FromMeshList(m_strMesh);

	/* Coloring Option */
	m_bColorChangingOn = file->Read<_bool>();

	/* Diffuse */
	m_DiffuseTexture.second = file->Read<string>();
	m_vDiffuseColor_BaseStart = ColorToImVec4(file->Read<_float4>());
	m_vDiffuseColor_BaseEnd = ColorToImVec4(file->Read<_float4>());
	m_vDiffuseColor_Dest = ColorToImVec4(file->Read<_float4>());
	m_DiffuseTexture.first = GetIndex_FromTexList(m_DiffuseTexture.second);

	/* Alpha Gradation */
	m_fAlphaGraIntensity = file->Read<_float>();
	m_vAlphaGraColor_Base = ColorToImVec4(file->Read<_float4>());
	m_vAlphaGraColor_Dest = ColorToImVec4(file->Read<_float4>());
	m_bDestSameWithBase_AlphaGra = Compare_IsSameColor(m_vAlphaGraColor_Base, m_vAlphaGraColor_Dest);
	if (0 == m_fAlphaGraIntensity) m_bAlphaGra_On = false;
	else m_bAlphaGra_On = true;

	/* Opacity */
	m_OpacityTexture.second = file->Read<string>();
	m_iSamplerType = file->Read<_int>();
	tiling = file->Read<_float2>();
	m_fTiling_Opacity[0] = tiling.x;
	m_fTiling_Opacity[1] = tiling.y;
	UVSpeed = file->Read<_float2>();
	m_fUVSpeed_Opacity[0] = UVSpeed.x;
	m_fUVSpeed_Opacity[1] = UVSpeed.y;
	m_OpacityTexture.first = GetIndex_FromTexList(m_OpacityTexture.second);
	
	/* Gradation by Texture */
	m_GraTexture.second = file->Read<string>();
	m_vGraColor_Base = ColorToImVec4(file->Read<_float4>());
	tiling = file->Read<_float2>();
	m_fTiling_Gra[0] = tiling.x;
	m_fTiling_Gra[1] = tiling.y;
	UVSpeed = file->Read<_float2>();
	m_fUVSpeed_Gra[0] = UVSpeed.x;
	m_fUVSpeed_Gra[1] = UVSpeed.y;
	m_vGraColor_Dest = ColorToImVec4(file->Read<_float4>());
	m_GraTexture.first = GetIndex_FromTexList(m_GraTexture.second);
	if (0 == m_GraTexture.first) m_bGra_On = false;
	else m_bGra_On = true;
	m_bUVOptionSameWithOpacity_Gra = Compare_IsSameUVOptionsWithOpacity(tiling, UVSpeed);
	m_bDestSameWithBase_Gra = Compare_IsSameColor(m_vGraColor_Base, m_vGraColor_Dest);

	/* Overlay */
	m_bOverlay_On = file->Read<_bool>();
	m_OverlayTexture.second = file->Read<string>();
	m_vOverlayColor_Base = ColorToImVec4(file->Read<_float4>());
	tiling = file->Read<_float2>();
	m_fTiling_Overlay[0] = tiling.x;
	m_fTiling_Overlay[1] = tiling.y;
	UVSpeed = file->Read<_float2>();
	m_fUVSpeed_Overlay[0] = UVSpeed.x;
	m_fUVSpeed_Overlay[1] = UVSpeed.y;
	m_OverlayTexture.first = GetIndex_FromTexList(m_OverlayTexture.second);
	m_bUVOptionSameWithOpacity_Overlay = Compare_IsSameUVOptionsWithOpacity(tiling, UVSpeed);

	/* Normal */
	m_NormalTexture.second = file->Read<string>();
	m_NormalTexture.first = GetIndex_FromTexList(m_NormalTexture.second);
	if (0 == m_NormalTexture.first) m_bNormal_On = false;
	else m_bNormal_On = true;

	/* Dissolve */
	m_DissolveTexture.second = file->Read<string>();
	tiling = file->Read<_float2>();
	m_fTiling_Dissolve[0] = tiling.x;
	m_fTiling_Dissolve[1] = tiling.y;
	UVSpeed = file->Read<_float2>();
	m_fTiling_Dissolve[0] = UVSpeed.x;
	m_fTiling_Dissolve[1] = UVSpeed.y;
	m_bDissolveInverse = file->Read<_bool>();
	m_DissolveTexture.first = GetIndex_FromTexList(m_DissolveTexture.second);
	m_bUVOptionSameWithOpacity_Dissolve = Compare_IsSameUVOptionsWithOpacity(tiling, UVSpeed);
	if (0 == m_DissolveTexture.first) m_bDissolve_On = false;
	else m_bDissolve_On = true;

	/* Distortion */
	m_DistortionTexture.second = file->Read<string>();
	tiling = file->Read<_float2>();
	m_fTiling_Distortion[0] = tiling.x;
	m_fTiling_Distortion[1] = tiling.y;
	UVSpeed = file->Read<_float2>();
	m_fUVSpeed_Distortion[0] = UVSpeed.x;
	m_fUVSpeed_Distortion[1] = UVSpeed.y;
	m_DistortionTexture.first = GetIndex_FromTexList(m_DistortionTexture.second);
	m_bUVOptionSameWithOpacity_Distortion = Compare_IsSameUVOptionsWithOpacity(tiling, UVSpeed);
	if (0 == m_DistortionTexture.first) m_bDistortion_On = false;
	else m_bDistortion_On = true;

	/* Blend */
	m_BlendTexture.second = file->Read<string>();
	m_BlendTexture.first = GetIndex_FromTexList(m_BlendTexture.second);
	if (0 == m_BlendTexture.first) m_bBlend_On = false;
	else m_bBlend_On = true;

	/* Color Edit */
	m_fContrast = file->Read<_float>();

	// For. Create Effect GameObjects
	Create();
}

void Widget_EffectMaker_Mesh::SubWidget_TextureCombo(_int* iSelected, string* strSelected, string strFilePath, const char* pszWidgetKey)
{
	/*
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
	*/

	ImGui::Text("Texture List");

	{
		ImGui::BeginChild(pszWidgetKey, ImVec2(ImGui::GetContentRegionAvail().x, 150), false);

		for (_uint n = 1; n < m_iNumUniversalTextures; n++)
		{
			wstring wstrKey = Utils::ToWString(m_pszUniversalTextures[n]);
			wstring wstrPath = Utils::ToWString(strFilePath) + wstrKey;
			Utils::DetachExt(wstrKey);
			auto pTexture = RESOURCES.GetOrAddTexture(wstrKey, wstrPath);

			char pszButtonkey[MAX_PATH];
			strcpy_s(pszButtonkey, sizeof(pszButtonkey), pszWidgetKey);
			strcat_s(pszButtonkey, sizeof(pszButtonkey), m_pszUniversalTextures[n]);
			if (ImGui::ImageButton(pszButtonkey, (pTexture->Get_SRV().Get()), ImVec2(50, 50)))
			{
				*iSelected = n;
				*strSelected = m_pszUniversalTextures[n];
			}

			if (0 != n % 4)
				ImGui::SameLine();
		}
		ImGui::EndChild();
	}
}

void Widget_EffectMaker_Mesh::SubWidget_ImageViewer(string strFileName, string strFilePath, const char* pszWidgetKey)
{
	wstring wstrKey = Utils::ToWString(strFileName);
	wstring wstrPath = Utils::ToWString(strFilePath) + wstrKey;
	Utils::DetachExt(wstrKey);

	auto pTexture = RESOURCES.GetOrAddTexture(wstrKey, wstrPath);

	ImGui::Text("Selected");
	
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		ImGui::BeginChild("##Child2_Diffuse", ImVec2(80,80), true, window_flags);
		if (nullptr != pTexture)
		{
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(1, 255, 1, 255));
			ImGui::Image((pTexture->Get_SRV().Get()), ImVec2(63, 63));
			ImGui::PopStyleColor();
		}
		ImGui::EndChild();
	}
}

void Widget_EffectMaker_Mesh::SubWidget_SettingTexUV(_float* arrTiling, _float* arrTexUVSpeed, const char* pszWidgetKey1, const char* pszWidgetKey2)
{
	ImGui::SliderFloat2(pszWidgetKey1, arrTiling, 0.00f, 1.00f); 
	ImGui::InputFloat2(pszWidgetKey2, arrTexUVSpeed);
}

Color Widget_EffectMaker_Mesh::ImVec4toColor(ImVec4 imvec)
{
	return Color(imvec.x, imvec.y, imvec.z, imvec.w);
}

ImVec4 Widget_EffectMaker_Mesh::ColorToImVec4(Color color)
{
	return ImVec4(color.x, color.y, color.z, color.w);
}

_int Widget_EffectMaker_Mesh::GetIndex_FromTexList(string strValue)
{
	_int iIndex = 0;
	for (auto iter : m_vecUniversalTextures)
	{
		if (strValue == iter)
			return iIndex;
		iIndex++;
	}
	return -1;
}

_int Widget_EffectMaker_Mesh::GetIndex_FromMeshList(string strValue)
{
	_int iIndex = 0;
	for (auto iter : m_vecMeshes)
	{
		if (strValue == iter)
			return iIndex;
		iIndex++;
	}
	return -1;
}

_bool Widget_EffectMaker_Mesh::Compare_IsSameColor(ImVec4 color1, ImVec4 color2)
{
	if (color1.x == color2.x &&
		color1.y == color2.y &&
		color1.z == color2.z &&
		color1.w == color2.w)
		return true;
	else
		return false;
}

_bool Widget_EffectMaker_Mesh::Compare_IsSameUVOptionsWithOpacity(_float2 tiling, _float2 UVSpeed)
{
	if(tiling.x == m_fTiling_Opacity[0] &&
		tiling.y == m_fTiling_Opacity[1] &&
		UVSpeed.x == m_fUVSpeed_Overlay[0] &&
		UVSpeed.y == m_fUVSpeed_Overlay[1] )
		return true;
	else
		return false;
}
