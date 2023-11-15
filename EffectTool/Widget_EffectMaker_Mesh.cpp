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

	if (m_bSaveMsgBox_On)
	{
		ImGui::SetNextWindowPos(ImVec2(g_iWinSizeX/2.f - 30.f, g_iWinSizeY / 2.f - 10.f));
		ImGui::Begin("Save");
		ImGui_SaveMsgBox();
		ImGui::End();
	}

	if(m_bTextureList_On)
	{
		ImGui::SetNextWindowPos(ImVec2(480.f, 100.f));
		ImGui::SetNextWindowSize(ImVec2(500.f, 530.f));
		ImGui::Begin("Texture List");
		ImGui_TextureList();
		ImGui::End();
	}
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
	if (ImGui::BeginTabBar("##Tab1"))
	{
		if (ImGui::BeginTabItem("Visual"))
		{
			Option_Property();
			Option_Mesh();
			Option_SpriteAnimation();

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
				m_bSaveMsgBox_On = true;
			ImGui::Spacing();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Transform"))
		{
			Option_InitTransform();
			Option_Movement();
			ImGui::Spacing();

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

			ImGui::EndTabItem();
		}
		
		ImGui::EndTabBar();
	}
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

void Widget_EffectMaker_Mesh::ImGui_SaveMsgBox()
{
	ImGui::Text("Are you sure you want to save it?");
	ImGui::Spacing();
	if (ImGui::Button("Save"))
	{
		Save();
		m_bSaveMsgBox_On = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("No"))
		m_bSaveMsgBox_On = false;
}

void Widget_EffectMaker_Mesh::ImGui_TextureList()
{
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		ImGui::BeginChild("##Child1_TextureList", ImVec2(390, 480), false, window_flags);

		SubWidget_TextureList();

		ImGui::EndChild();
	}
	ImGui::SameLine();
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		ImGui::BeginChild("##Child2_TextureList", ImVec2(50, 360), false, window_flags);

		if (ImGui::Button("  Ok  "))
			m_bTextureList_On = false;

		ImGui::EndChild();
	}
}

void Widget_EffectMaker_Mesh::Option_Property()
{
	ImGui::SeparatorText("Property");	

	ImGui::InputText("Tag", m_szTag, MAX_PATH);
	ImGui::Spacing();
	
	ImGui::InputInt("Number of Mesh##Property", &m_iMeshCnt);
	
	ImGui::InputFloat("Create Interval##", &m_fCreateInterval);
	
	if (0.f < m_fCreateInterval)
	{
		const char* pszItem_ParticleDuration[] = { "Static", "Random in rang" };
		if (ImGui::BeginCombo("Particle Duration##Opacity", pszItem_ParticleDuration[m_iParticleDurationOption], 0))
		{
			for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_ParticleDuration); n++)
			{
				const bool is_selected = (m_iParticleDurationOption == n);
				if (ImGui::Selectable(pszItem_ParticleDuration[n], is_selected))
					m_iParticleDurationOption = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		switch (m_iParticleDurationOption)
		{
		case 0:
			ImGui::InputFloat("Particle Duration##Property", &m_fParticleDuration[0]);
			m_fParticleDuration[1] = m_fParticleDuration[0];
			break;
		case 1:
			ImGui::InputFloat2("Particle Duration (min, max)##Property", m_fParticleDuration);
			break;
		}
	}
	else
	{
		m_fParticleDuration[0] = m_fDuration;
		m_fParticleDuration[1] = m_fDuration;
	}
	ImGui::Spacing();

	ImGui::InputFloat("Duration##Property", &m_fDuration);
	ImGui::Spacing();

	ImGui::Checkbox("Blur On##Property", &m_bBlurOn);
	ImGui::Spacing();

	ImGui::Checkbox("On Fade Out##Property", &m_bUseFadeOut);
	ImGui::Spacing();	

	ImGui::Checkbox("Color Changing On##Property", &m_bColorChangingOn);
	ImGui::Spacing();
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

	if(0 == m_iDiffuseOption && !m_bIsTextureSameWithOpacity)
		SubWidget_ImageViewer(m_DiffuseTexture.second, m_strTexturePath, "##Img_Diffuse");

	switch (m_iDiffuseOption)
	{
	case 0:
		ImGui::SameLine();
		if (!m_bIsTextureSameWithOpacity)
		{			
			if (ImGui::Button("Texture"))
			{
				m_iTexture_TextureList = &m_DiffuseTexture.first;
				m_pTextureTag_TextureList = &m_DiffuseTexture.second;
				m_pszWidgetKey_TextureList = "Texture##Diffuse";
				m_bTextureList_On = true;
			}
		}
		if (ImGui::Checkbox("Use same texture with Opacity##DIffuse", &m_bIsTextureSameWithOpacity))
		{
			m_DiffuseTexture.first = m_OpacityTexture.first;
			m_DiffuseTexture.second = m_OpacityTexture.second;
		}
		break;
	case 1:
		ImGui::ColorEdit4("Color##Diffuse", (float*)&m_vDiffuseColor_BaseStart, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		m_vDiffuseColor_BaseEnd = m_vDiffuseColor_BaseStart;

		if (m_bColorChangingOn)
			ImGui::ColorEdit4("Dest Color##Diffuse", (float*)&m_vDiffuseColor_Dest, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

		break;
	case 2:
		ImGui::ColorEdit4("Range Color1##Diffuse", (float*)&m_vDiffuseColor_BaseStart, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		ImGui::ColorEdit4("Range Color2##Diffuse", (float*)&m_vDiffuseColor_BaseEnd, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
		
		if (m_bColorChangingOn)
			ImGui::ColorEdit4("Dest Color##Diffuse", (float*)&m_vDiffuseColor_Dest, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

		break;
	default:
		break;
	}

	ImGui::Spacing();

	// For. Tiling, move Texture UV Speed 
	if (0 == m_iDiffuseOption && !m_bUseSpriteAnimation)
		SubWidget_SettingTexUV(m_fTiling_Diffuse, m_fUVSpeed_Diffuse, "Tiling(x,y)##Diffuse", "Move TexUV Speed(x,y)##Diffuse");

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Opacity()
{
	ImGui::SeparatorText("Opacity");

	SubWidget_ImageViewer(m_OpacityTexture.second, m_strTexturePath, "##Img_Opacity");
	
	ImGui::SameLine();

	if (ImGui::Button("Texture##Opacity"))
	{
		m_iTexture_TextureList = &m_OpacityTexture.first;
		m_pTextureTag_TextureList = &m_OpacityTexture.second;
		m_pszWidgetKey_TextureList = "Texture##Opacity";
		m_bTextureList_On = true;
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
	if (!m_bUseSpriteAnimation)
		SubWidget_SettingTexUV(m_fTiling_Opacity, m_fUVSpeed_Opacity, "Tiling(x,y)##Opacity", "Move TexUV Speed(x,y)##Opacity");

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_SpriteAnimation()
{
	/* Lock Diffuse and Opacity tiling and uvspeed*/

	ImGui::SeparatorText("Sprite Animation");
	ImGui::Checkbox("Use Sprite Animation##SpriteAnimation", &m_bUseSpriteAnimation);
	if (m_bUseSpriteAnimation)
	{
		ImGui::InputInt2("Number of Sprite(x,y)##SpriteAnimation", m_iSpriteAni_Count);
		ImGui::InputFloat("Animation Speed##SpriteAnimation", &m_fSpriteAni_Speed);
	}
}

void Widget_EffectMaker_Mesh::Option_Blend()
{
	ImGui::SeparatorText("Blend");

	ImGui::Checkbox("Blend On", &m_bBlend_On);

	if (m_bBlend_On)
	{
		SubWidget_ImageViewer(m_BlendTexture.second, m_strTexturePath, "##Img_Blend");
		ImGui::SameLine();
		if (ImGui::Button("Texture##Blend"))
		{
			m_iTexture_TextureList = &m_BlendTexture.first;
			m_pTextureTag_TextureList = &m_BlendTexture.second;
			m_pszWidgetKey_TextureList = "Texture##Blend";
			m_bTextureList_On = true;
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
		SubWidget_ImageViewer(m_NormalTexture.second, m_strTexturePath, "##Img_Normal");

		ImGui::SameLine();

		if (ImGui::Button("Texture##Normal"))
		{
			m_iTexture_TextureList = &m_NormalTexture.first;
			m_pTextureTag_TextureList = &m_NormalTexture.second;
			m_pszWidgetKey_TextureList = "Texture##Normal";
			m_bTextureList_On = true;
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
		SubWidget_ImageViewer(m_GraTexture.second, m_strTexturePath, "##Img_Gradation");

		ImGui::SameLine();
		if (ImGui::Button("Texture##Gradation"))
		{
			m_iTexture_TextureList = &m_GraTexture.first;
			m_pTextureTag_TextureList = &m_GraTexture.second;
			m_pszWidgetKey_TextureList = "Texture##Gradation";
			m_bTextureList_On = true;
		}
		if (ImGui::Button("Texture##Gradation"))
		{
			m_iTexture_TextureList = &m_GraTexture.first;
			m_pTextureTag_TextureList = &m_GraTexture.second;
			m_pszWidgetKey_TextureList = "Texture##Gradation";
			m_bTextureList_On = true;
		}

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
		SubWidget_ImageViewer(m_OverlayTexture.second, m_strTexturePath, "##Img_Overlay");

		ImGui::SameLine();
		if (ImGui::Button("Texture##Dissolve"))
		{
			m_iTexture_TextureList = &m_OverlayTexture.first;
			m_pTextureTag_TextureList = &m_OverlayTexture.second;
			m_pszWidgetKey_TextureList = "Texture##Dissolve";
			m_bTextureList_On = true;
		}

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
		SubWidget_ImageViewer(m_DissolveTexture.second, m_strTexturePath, "##Img_Dissolve");

		ImGui::SameLine();
		if (ImGui::Button("Texture##Dissolve"))
		{
			m_iTexture_TextureList = &m_DissolveTexture.first;
			m_pTextureTag_TextureList = &m_DissolveTexture.second;
			m_pszWidgetKey_TextureList = "Texture##Dissolve";
			m_bTextureList_On = true;
		}

		ImGui::Checkbox("UV Option same with opacity##Dissolve", &m_bUVOptionSameWithOpacity_Dissolve);
		if (m_bUVOptionSameWithOpacity_Dissolve)
		{
			m_fTiling_Dissolve[0] = m_fTiling_Opacity[0];
			m_fTiling_Dissolve[1] = m_fTiling_Opacity[1];
			m_fUVSpeed_Dissolve[0] = m_fUVSpeed_Opacity[0];
			m_fUVSpeed_Dissolve[1] = m_fUVSpeed_Opacity[1];
		}
		SubWidget_SettingTexUV(m_fTiling_Dissolve, m_fUVSpeed_Dissolve, "Tiling(x,y)##Dissolve", "Move TexUV Speed(x,y)##Dissolve");
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
		SubWidget_ImageViewer(m_DistortionTexture.second, m_strTexturePath, "##Img_Distortion");

		ImGui::SameLine();

		if (ImGui::Button("Texture##Distortion"))
		{
			m_iTexture_TextureList = &m_DistortionTexture.first;
			m_pTextureTag_TextureList = &m_DistortionTexture.second;
			m_pszWidgetKey_TextureList = "Texture##Distortion";
			m_bTextureList_On = true;
		}

		ImGui::Checkbox("UV Option same with opacity##Distortion", &m_bUVOptionSameWithOpacity_Distortion);
		if (m_bUVOptionSameWithOpacity_Dissolve)
		{
			m_fTiling_Distortion[0] = m_fTiling_Opacity[0];
			m_fTiling_Distortion[1] = m_fTiling_Opacity[1];
			m_fUVSpeed_Distortion[0] = m_fUVSpeed_Opacity[0];
			m_fUVSpeed_Distortion[1] = m_fUVSpeed_Opacity[1];
		}

		SubWidget_SettingTexUV(m_fTiling_Distortion, m_fUVSpeed_Distortion, "Tiling(x,y)##Distortion", "Move TexUV Speed(x,y)##Distortion");
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

void Widget_EffectMaker_Mesh::Option_InitTransform()
{
	ImGui::SeparatorText("Initial Transform");

	if(ImGui::TreeNode("Init Position##InitTransform"))
	{
		const char* pszItem_InitPosOption[] = { "Fixed in center", "Random in range" };
		if (ImGui::BeginCombo("Decide init position option##InitTransform", pszItem_InitPosOption[m_iInitPosOption], 0))
		{
			for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_InitPosOption); n++)
			{
				const bool is_selected = (m_iInitPosOption == n);
				if (ImGui::Selectable(pszItem_InitPosOption[n], is_selected))
					m_iInitPosOption = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		switch (m_iInitPosOption)
		{
		case 0:
			m_fPosRange[0] = 0.f;
			m_fPosRange[1] = 0.f;
			m_fPosRange[2] = 0.f;
			break;
		case 1:
			ImGui::InputFloat3("Range(x,y,z)##InitTransform", m_fPosRange);
			break;
		}

		ImGui::TreePop();
	}
	ImGui::Spacing();

	if (ImGui::TreeNode("Init Scale##EffectMesh"))
	{
		const char* pszItem_InitScaleOption[] = { "Static", "Random in range" };
		if (ImGui::BeginCombo("Decide init scale option##InitScale", pszItem_InitScaleOption[m_iInitScaleOption], 0))
		{
			for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_InitScaleOption); n++)
			{
				const bool is_selected = (m_iInitScaleOption == n);
				if (ImGui::Selectable(pszItem_InitScaleOption[n], is_selected))
					m_iInitScaleOption = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		switch (m_iInitScaleOption)
		{
		case 0:
			ImGui::InputFloat3("Range(x,y,z)##InitScale", m_fInitScale_Min);
			memcpy(m_fInitScale_Max, m_fInitScale_Min, sizeof(m_fInitScale_Min));
			break;
		case 1:
			ImGui::InputFloat3("Range min(x,y,z)##InitScale", m_fInitScale_Min);
			ImGui::InputFloat3("Range max(x,y,z)##InitScale", m_fInitScale_Max);
			break;
		}

		ImGui::TreePop();
	}
	ImGui::Spacing();

	if (ImGui::TreeNode("Init Rotation##EffectMesh"))
	{
		const char* pszItem_InitRotationOption[] = { "Static", "Random in range" };
		if (ImGui::BeginCombo("Decide init rotation option##InitRotation", pszItem_InitRotationOption[m_iInitRotationOption], 0))
		{
			for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_InitRotationOption); n++)
			{
				const bool is_selected = (m_iInitRotationOption == n);
				if (ImGui::Selectable(pszItem_InitRotationOption[n], is_selected))
					m_iInitRotationOption = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		switch (m_iInitRotationOption)
		{
		case 0:
			ImGui::InputFloat3("Angle##InitRotation", m_fInitRotation_Min);
			memcpy(m_fInitRotation_Max, m_fInitRotation_Min, sizeof(m_fInitRotation_Min));
			break;
		case 1:
			ImGui::InputFloat3("Range min(x,y,z)##InitRotation", m_fInitRotation_Min);
			ImGui::InputFloat3("Range max(x,y,z)##InitRotation", m_fInitRotation_Max);
			break;
		}

		ImGui::TreePop();
	}
	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Movement()
{
	ImGui::SeparatorText("Movement");

	if (ImGui::TreeNode("Translate##Movement"))
	{
		const char* pszItem_TranslateOption[] = { "No change", "Move to target position", "Move to random target position", // 2
			"Go Straight", "Go Back", "Go Lift", "Go Right", // 6
			"Spreading dust", "Scattered embers" // 8
		};
		if (ImGui::BeginCombo("Translate option##Movement", pszItem_TranslateOption[m_iTranslateOption], 0))
		{
			for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_TranslateOption); n++)
			{
				const bool is_selected = (m_iTranslateOption == n);
				if (ImGui::Selectable(pszItem_TranslateOption[n], is_selected))
					m_iTranslateOption = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		switch (m_iTranslateOption)
		{
		case 0:	// No change
			m_fTranslateSpeed = 0.f;
			ZeroMemory(m_fEndPositionOffset_Min, sizeof(m_fEndPositionOffset_Min));
			ZeroMemory(m_fEndPositionOffset_Max, sizeof(m_fEndPositionOffset_Max));
			break;
		case 1:	// Move to target
			m_fTranslateSpeed = 0;
			ImGui::InputFloat3("Target Position##Movement", m_fEndPositionOffset_Min);
			memcpy(m_fEndPositionOffset_Max, m_fEndPositionOffset_Min, sizeof(m_fEndPositionOffset_Min));
			break;
		case 2: // Move to random target
			m_fTranslateSpeed = 0;
			ImGui::InputFloat3("Target Position (min)##Movement", m_fEndPositionOffset_Min);
			ImGui::InputFloat3("Target Position (max)##Movement", m_fEndPositionOffset_Max);
			break;
		case 3:	// Go straight
		case 4:	// Go back
		case 5:	// Go left
		case 6:	// Go right
			ImGui::InputFloat("Translate Speed##Movement", &m_fTranslateSpeed);
			ZeroMemory(m_fEndPositionOffset_Min, sizeof(m_fEndPositionOffset_Min));
			ZeroMemory(m_fEndPositionOffset_Max, sizeof(m_fEndPositionOffset_Max));
			break;
		}

		ImGui::TreePop();
	}
	ImGui::Spacing();

	if (ImGui::TreeNode("Scaling##Movement"))
	{
		const char* pszItem_ScaleOption[] = { "No change", "Change to target scale" };
		if (ImGui::BeginCombo("Scaling option##Movement", pszItem_ScaleOption[m_iScalingOption], 0))
		{
			for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_ScaleOption); n++)
			{
				const bool is_selected = (m_iScalingOption == n);
				if (ImGui::Selectable(pszItem_ScaleOption[n], is_selected))
					m_iScalingOption = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		
		switch (m_iScalingOption)
		{
		case 0:
			ZeroMemory(m_fEndScaleOffset, sizeof(m_fEndScaleOffset));
			break;
		case 1:
			ImGui::InputFloat3("Target Scale##Movement", m_fEndScaleOffset);
			break;
		}
		
		ImGui::TreePop();
	}
	ImGui::Spacing();

	if (ImGui::TreeNode("Turn##Movement"))
	{
		const char* pszItem_TurnOption[] = { "No change", "Turn static", "Turn random"};
		if (ImGui::BeginCombo("Turn option##Movement", pszItem_TurnOption[m_iTurnOption], 0))
		{
			for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_TurnOption); n++)
			{
				const bool is_selected = (m_iTurnOption == n);
				if (ImGui::Selectable(pszItem_TurnOption[n], is_selected))
					m_iTurnOption = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		switch (m_iTurnOption)
		{
		case 0:
			m_fTurnSpeed = 0.f;
			ZeroMemory(m_fRandomAxis_Min, sizeof(m_fRandomAxis_Min));
			ZeroMemory(m_fRandomAxis_Max, sizeof(m_fRandomAxis_Max));
			break;
		case 1:
			ImGui::InputFloat("Turn Speed##Movemenet1", &m_fTurnSpeed);
			ImGui::InputFloat3("Axis##Movement1", m_fRandomAxis_Min);
			memcpy(m_fRandomAxis_Max, m_fRandomAxis_Min, sizeof(m_fRandomAxis_Min));
			break;
		case 2:
			ImGui::InputFloat("Turn Speed##Movemenet2", &m_fTurnSpeed);
			ImGui::InputFloat3("Axis Range (min)##Movement", m_fRandomAxis_Min);
			ImGui::InputFloat3("Axis Range (max)##Movement", m_fRandomAxis_Max);
			break;
		}
		ImGui::TreePop();
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
	EffectObj->Get_Transform()->Scaled(_float3(1.0f));

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
		m_iMeshCnt,
		m_fCreateInterval,
		_float2(m_fParticleDuration),

		m_strMesh,

		m_bUseSpriteAnimation,
		m_iSpriteAni_Count[0],
		m_iSpriteAni_Count[1],
		m_fSpriteAni_Speed,

		m_bColorChangingOn,

		m_DiffuseTexture.second,
		ImVec4toColor(m_vDiffuseColor_BaseStart),
		ImVec4toColor(m_vDiffuseColor_BaseEnd),
		ImVec4toColor(m_vDiffuseColor_Dest),
		_float2(m_fTiling_Diffuse),
		_float2(m_fUVSpeed_Diffuse),

		m_OpacityTexture.second,
		m_iSamplerType,
		_float2(m_fTiling_Opacity),
		_float2(m_fUVSpeed_Opacity),

		m_fAlphaGraIntensity,
		ImVec4toColor(m_vAlphaGraColor_Base),
		ImVec4toColor(m_vAlphaGraColor_Dest),

		m_GraTexture.second,
		ImVec4toColor(m_vGraColor_Base),
		ImVec4toColor(m_vGraColor_Dest),
		_float2(m_fTiling_Gra),
		_float2(m_fUVSpeed_Gra),

		m_bOverlay_On,
		m_OverlayTexture.second,
		ImVec4toColor(m_vOverlayColor_Base),
		_float2(m_fTiling_Overlay),
		_float2(m_fUVSpeed_Overlay),

		m_NormalTexture.second,

		m_DissolveTexture.second,
		_float2(m_fTiling_Overlay),
		_float2(m_fUVSpeed_Overlay),
		m_bDissolveInverse,

		m_DistortionTexture.second,
		_float2(m_fTiling_Distortion),
		_float2(m_fUVSpeed_Distortion),

		m_BlendTexture.second,

		m_fContrast

	};
	EffectObj->Get_MeshEffect()->Init(&tMeshEffectDesc);

	// For. Add and Setting EffectMovement Script to GameObject
	MeshEffectData::Transform_Desc tTransform_Desc
	{
		_float3(m_fPosRange),

		_float3(m_fInitScale_Min),
		_float3(m_fInitScale_Max),

		_float3(m_fInitRotation_Min),
		_float3(m_fInitRotation_Max),

		m_iTranslateOption,
		m_fTranslateSpeed,
		_float3(m_fEndPositionOffset_Min),
		_float3(m_fEndPositionOffset_Max),

		m_iScalingOption,
		_float3(m_fEndScaleOffset),

		m_iTurnOption,
		m_fTurnSpeed,
		_float3(m_fRandomAxis_Min),
		_float3(m_fRandomAxis_Max),
	};
	EffectObj->Get_MeshEffect()->Set_TransformDesc(&tTransform_Desc);
	EffectObj->Get_MeshEffect()->InitialTransform();

	// For. Add Effect GameObject to current scene
	CUR_SCENE->Add_GameObject(EffectObj);
}

void Widget_EffectMaker_Mesh::Save()
{
	string strFileName = m_szTag;
	string strFilePath = "..\\Resources\\EffectData\\MeshEffectData\\";
	strFilePath += strFileName + ".dat";

	{	
		shared_ptr<FileUtils> file = make_shared<FileUtils>();
		file->Open(Utils::ToWString(strFilePath), FileMode::Write);

		/* Property */
		file->Write<string>(m_szTag);
		file->Write<_float>(m_fDuration);
		file->Write<_bool>(m_bBlurOn);
		file->Write<_bool>(m_bUseFadeOut);
		file->Write<_int>(m_iMeshCnt);
		file->Write<_float>(m_fCreateInterval);
		file->Write<_float2>(_float2(m_fParticleDuration));

		/* Mesh */
		// file->Write<_bool>(m_iMesh); 
		file->Write<string>(m_strMesh);

		/* Sprite Animation */
		file->Write<_bool>(m_bUseSpriteAnimation);
		file->Write<_int>(m_iSpriteAni_Count[0]);
		file->Write<_int>(m_iSpriteAni_Count[1]);
		file->Write<_float>(m_fSpriteAni_Speed);

		/* Coloring Option */
		file->Write<_bool>(m_bColorChangingOn);

		/* Diffuse */
		// file->Write<_int>(m_DiffuseTexture.first);
		file->Write<string>(m_DiffuseTexture.second);
		file->Write<_float4>(ImVec4toColor(m_vDiffuseColor_BaseStart));
		file->Write<_float4>(ImVec4toColor(m_vDiffuseColor_BaseEnd));
		file->Write<_float4>(ImVec4toColor(m_vDiffuseColor_Dest));
		file->Write<_float2>(_float2(m_fTiling_Diffuse));
		file->Write<_float2>(_float2(m_fUVSpeed_Diffuse));

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
		file->Write<_float2>(_float2(m_fTiling_Opacity));
		file->Write<_float2>(_float2(m_fUVSpeed_Opacity));

		/* Gradation by Texture */
		// file->Write<_bool>(m_bGra_On);
		// file->Write<_int>(m_GraTexture.first);
		file->Write<string>(m_GraTexture.second);
		file->Write<_float4>(ImVec4toColor(m_vGraColor_Base));
		// file->Write<_bool>(m_bUVOptionSameWithOpacity_Gra);
		file->Write<_float2>(_float2(m_fTiling_Gra));
		file->Write<_float2>(_float2(m_fUVSpeed_Gra));
		// file->Write<_bool>(m_bDestSameWithBase_Gra);
		file->Write<_float4>(ImVec4toColor(m_vGraColor_Dest));

		/* Overlay */
		file->Write<_bool>(m_bOverlay_On);
		// file->Write<_int>(m_OverlayTexture.first);
		file->Write<string>(m_OverlayTexture.second);
		file->Write<_float4>(ImVec4toColor(m_vOverlayColor_Base));
		// file->Write<_bool>(m_bUVOptionSameWithOpacity_Overlay);
		file->Write<_float2>(_float2(m_fTiling_Overlay));
		file->Write<_float2>(_float2(m_fUVSpeed_Overlay));

		/* Normal */
		// file->Write<_int>(m_NormalTexture.first);
		file->Write<string>(m_NormalTexture.second);

		/* Dissolve */
		// file->Write<_int>(m_DissolveTexture.first);
		file->Write<string>(m_DissolveTexture.second);
		// file->Write<_bool>(m_bUVOptionSameWithOpacity_Dissolve);
		file->Write<_float2>(_float2(m_fTiling_Dissolve));
		file->Write<_float2>(_float2(m_fUVSpeed_Dissolve));
		file->Write<_bool>(m_bDissolveInverse);

		/* Distortion */
		// file->Write<_bool>(m_bDistortion_On);
		// file->Write<_int>(m_DistortionTexture.first);
		file->Write<string>(m_DistortionTexture.second);
		// file->Write<_bool>(m_bUVOptionSameWithOpacity_Distortion);
		file->Write<_float2>(_float2(m_fTiling_Distortion));
		file->Write<_float2>(_float2(m_fUVSpeed_Distortion));

		/* Blend */
		// file->Write<_bool>(m_bBlend_On);
		// file->Write<_int>(m_BlendTexture.first);
		file->Write<string>(m_BlendTexture.second);

		/* Color Edit */
		file->Write<_float>(m_fContrast);

		// For. Transform Desc 
		/* Init position */
		//file->Write<_int>(m_iInitPosOption);
		file->Write<_float3>(_float3(m_fPosRange));

		/* Init Scale */
		//file->Write<_int>(m_iInitScaleOption);
		file->Write<_float3>(_float3(m_fInitScale_Min));
		file->Write<_float3>(_float3(m_fInitScale_Max));

		/* Init Rotation */
		//file->Write<_int>(m_iInitRotationOption);
		file->Write<_float3>(_float3(m_fInitRotation_Min));
		file->Write<_float3>(_float3(m_fInitRotation_Max));

		/* Translate */
		file->Write<_int>(m_iTranslateOption);
		file->Write<_float>(m_fTranslateSpeed);
		file->Write<_float3>(_float3(m_fEndPositionOffset_Min));
		file->Write<_float3>(_float3(m_fEndPositionOffset_Max));

		/* Scaling */
		file->Write<_int>(m_iScalingOption);
		file->Write<_float3>(_float3(m_fEndScaleOffset));

		/* Turn */
		file->Write<_int>(m_iTurnOption);
		file->Write<_float>(m_fTurnSpeed);
		file->Write<_float3>(_float3(m_fRandomAxis_Min));
		file->Write<_float3>(_float3(m_fRandomAxis_Max));
	}	
	
	RESOURCES.ReloadOrAddMeshEffectData(Utils::ToWString(strFileName), Utils::ToWString(strFilePath));
	
	// For. Update Finished Effect List 
	Set_FinishedEffect_List();
}

void Widget_EffectMaker_Mesh::Load()
{	
	_float3 vTemp_vec3 = { 0.f, 0.f, 0.f };
	_float2 vTemp_vec2 = { 0.f, 0.f };
	
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
	m_iMeshCnt = file->Read<_int>();
	m_fCreateInterval = file->Read<_float>();
	_float2 vFloat2 = file->Read<_float2>();
	memcpy(m_fParticleDuration, &vFloat2, sizeof(m_fParticleDuration));

	/* Mesh */
	m_strMesh = file->Read<string>();
	m_iMesh = GetIndex_FromMeshList(m_strMesh);

	/* Sprite Animation */
	m_bUseSpriteAnimation = file->Read<_bool>();
	m_iSpriteAni_Count[0] = file->Read<_int>();
	m_iSpriteAni_Count[1] = file->Read<_int>();
	m_fSpriteAni_Speed = file->Read<_float>();

	/* Coloring Option */
	m_bColorChangingOn = file->Read<_bool>();

	/* Diffuse */
	m_DiffuseTexture.second = file->Read<string>();
	m_vDiffuseColor_BaseStart = ColorToImVec4(file->Read<_float4>());
	m_vDiffuseColor_BaseEnd = ColorToImVec4(file->Read<_float4>());
	m_vDiffuseColor_Dest = ColorToImVec4(file->Read<_float4>());
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fTiling_Diffuse, &vTemp_vec2, sizeof(m_fTiling_Diffuse));
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fUVSpeed_Diffuse, &vTemp_vec2, sizeof(m_fUVSpeed_Diffuse));
	m_DiffuseTexture.first = GetIndex_FromTexList(m_DiffuseTexture.second);

	/* Alpha Gradation */
	m_fAlphaGraIntensity = file->Read<_float>();
	m_vAlphaGraColor_Base = ColorToImVec4(file->Read<_float4>());
	m_vAlphaGraColor_Dest = ColorToImVec4(file->Read<_float4>());
	m_bDestSameWithBase_AlphaGra = Equal(m_vAlphaGraColor_Base, m_vAlphaGraColor_Dest);
	if (0 == m_fAlphaGraIntensity) m_bAlphaGra_On = false;
	else m_bAlphaGra_On = true;

	/* Opacity */
	m_OpacityTexture.second = file->Read<string>();
	m_iSamplerType = file->Read<_int>();
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fTiling_Opacity, &vTemp_vec2, sizeof(m_fTiling_Opacity));
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fUVSpeed_Opacity, &vTemp_vec2, sizeof(m_fUVSpeed_Opacity));
	m_OpacityTexture.first = GetIndex_FromTexList(m_OpacityTexture.second);
	
	/* Gradation by Texture */
	m_GraTexture.second = file->Read<string>();
	m_vGraColor_Base = ColorToImVec4(file->Read<_float4>());
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fTiling_Gra, &vTemp_vec2, sizeof(m_fTiling_Gra));
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fUVSpeed_Gra, &vTemp_vec2, sizeof(m_fUVSpeed_Gra));
	m_vGraColor_Dest = ColorToImVec4(file->Read<_float4>());
	m_GraTexture.first = GetIndex_FromTexList(m_GraTexture.second);
	if (0 == m_GraTexture.first) m_bGra_On = false;
	else m_bGra_On = true;
	m_bUVOptionSameWithOpacity_Gra = Equal(m_fTiling_Gra, m_fTiling_Opacity, sizeof(_float) * 2) && Equal(m_fUVSpeed_Gra, m_fUVSpeed_Opacity, sizeof(_float) * 2);
	m_bDestSameWithBase_Gra = Equal(m_vGraColor_Base, m_vGraColor_Dest);

	/* Overlay */
	m_bOverlay_On = file->Read<_bool>();
	m_OverlayTexture.second = file->Read<string>();
	m_vOverlayColor_Base = ColorToImVec4(file->Read<_float4>());
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fTiling_Overlay, &vTemp_vec2, sizeof(m_fTiling_Overlay));
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fUVSpeed_Overlay, &vTemp_vec2, sizeof(m_fUVSpeed_Overlay));
	m_OverlayTexture.first = GetIndex_FromTexList(m_OverlayTexture.second);
	m_bUVOptionSameWithOpacity_Overlay = Equal(m_fTiling_Overlay, m_fTiling_Opacity, sizeof(_float) * 2) && Equal(m_fUVSpeed_Overlay, m_fUVSpeed_Opacity, sizeof(_float) * 2);

	/* Normal */
	m_NormalTexture.second = file->Read<string>();
	m_NormalTexture.first = GetIndex_FromTexList(m_NormalTexture.second);
	if (0 == m_NormalTexture.first) m_bNormal_On = false;
	else m_bNormal_On = true;

	/* Dissolve */
	m_DissolveTexture.second = file->Read<string>();
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fTiling_Dissolve, &vTemp_vec2, sizeof(m_fTiling_Dissolve));
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fUVSpeed_Dissolve, &vTemp_vec2, sizeof(m_fUVSpeed_Dissolve));
	m_bDissolveInverse = file->Read<_bool>();
	m_DissolveTexture.first = GetIndex_FromTexList(m_DissolveTexture.second);
	m_bUVOptionSameWithOpacity_Dissolve = Equal(m_fTiling_Dissolve, m_fTiling_Opacity, sizeof(_float) * 2) && Equal(m_fUVSpeed_Dissolve, m_fUVSpeed_Opacity, sizeof(_float) * 2);
	if (0 == m_DissolveTexture.first) m_bDissolve_On = false;
	else m_bDissolve_On = true;

	/* Distortion */
	m_DistortionTexture.second = file->Read<string>();
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fTiling_Distortion, &vTemp_vec2, sizeof(m_fTiling_Dissolve));
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fUVSpeed_Distortion, &vTemp_vec2, sizeof(m_fUVSpeed_Distortion));
	m_DistortionTexture.first = GetIndex_FromTexList(m_DistortionTexture.second);
	m_bUVOptionSameWithOpacity_Distortion = Equal(m_fTiling_Distortion, m_fTiling_Opacity, sizeof(_float) * 2) && Equal(m_fUVSpeed_Distortion, m_fUVSpeed_Opacity, sizeof(_float) * 2);
	if (0 == m_DistortionTexture.first) m_bDistortion_On = false;
	else m_bDistortion_On = true;

	/* Blend */
	m_BlendTexture.second = file->Read<string>();
	m_BlendTexture.first = GetIndex_FromTexList(m_BlendTexture.second);
	if (0 == m_BlendTexture.first) m_bBlend_On = false;
	else m_bBlend_On = true;

	/* Color Edit */
	m_fContrast = file->Read<_float>();

	// For. Transform Desc
	/* Init Pos */
	_float3 vTemp1 = file->Read<_float3>();
	memcpy(m_fPosRange, &vTemp1, sizeof(_float)*3);
	if (Equal(_float3(m_fPosRange), _float3(0.f, 0.f, 0.f)))
		m_iInitPosOption = 0;
	else 
		m_iInitPosOption = 1;

	/* Init Scale */
	vTemp_vec3 = file->Read<_float3>();
	memcpy(m_fInitScale_Min, &vTemp_vec3, sizeof(m_fInitScale_Min));
	vTemp_vec3 = file->Read<_float3>();
	memcpy(m_fInitScale_Max, &vTemp_vec3, sizeof(m_fInitScale_Max));
	if (Equal(m_fInitScale_Min, m_fInitScale_Max, 3))
		m_iInitScaleOption = 0;
	else
		m_iInitScaleOption = 1;

	/* Init Rotation */
	vTemp_vec3 = file->Read<_float3>();
	memcpy(m_fInitRotation_Min, &vTemp_vec3, sizeof(m_fInitRotation_Min));
	vTemp_vec3 = file->Read<_float3>();
	memcpy(m_fInitRotation_Max, &vTemp_vec3, sizeof(m_fInitRotation_Max));
	if (Equal(m_fInitRotation_Min, m_fInitRotation_Max, 3))
		m_iInitRotationOption = 0;
	else
		m_iInitRotationOption = 1;

	/* Translate */
	m_iTranslateOption = file->Read<_int>();
	m_fTranslateSpeed = file->Read<_float>();
	vTemp_vec3 = file->Read<_float3>();
	memcpy(m_fEndPositionOffset_Min, &vTemp_vec3, sizeof(m_fEndPositionOffset_Min));
	vTemp_vec3 = file->Read<_float3>();
	memcpy(m_fEndPositionOffset_Max, &vTemp_vec3, sizeof(m_fEndPositionOffset_Max));

	/* Scaling */
	m_iScalingOption = file->Read<_int>();
	vTemp_vec3 = file->Read<_float3>();
	memcpy(m_fEndScaleOffset, &vTemp_vec3, sizeof(m_fEndScaleOffset));
	if (Equal(_float3(m_fEndScaleOffset), _float3(0.f, 0.f, 0.f)))
		m_iScalingOption = 0;
	else
		m_iScalingOption = 1;

	/* Turn */
	m_iTurnOption = file->Read<_int>();
	m_fTurnSpeed = file->Read<_float>();
	vTemp_vec3 = file->Read<_float3>();
	memcpy(m_fRandomAxis_Min, &vTemp_vec3, sizeof(m_fRandomAxis_Min));
	vTemp_vec3 = file->Read<_float3>();
	memcpy(m_fRandomAxis_Max, &vTemp_vec3, sizeof(m_fRandomAxis_Max));
	if (Equal(_float3(m_fRandomAxis_Min), _float3(0.f, 0.f, 0.f)))
		m_iTurnOption = 0;
	else
	{
		if (Equal(m_fRandomAxis_Min, m_fRandomAxis_Max, 3))
			m_iTurnOption = 1;
		else
			m_iTurnOption = 2;
	}

	// For. Create Effect GameObjects
	Create();
}

void Widget_EffectMaker_Mesh::SubWidget_TextureCombo(_int* iSelected, string* strSelected, string strFilePath, const char* pszWidgetKey)
{
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

void Widget_EffectMaker_Mesh::SubWidget_TextureList()
{
	ImGui::Text("Texture List");
	{
		ImGui::BeginChild(m_pszWidgetKey_TextureList, ImVec2(ImGui::GetContentRegionAvail().x, 500), false);

		for (_uint n = 1; n < m_iNumUniversalTextures; n++)
		{
			wstring wstrKey = Utils::ToWString(m_pszUniversalTextures[n]);
			wstring wstrPath = Utils::ToWString(m_strTexturePath) + wstrKey;
			Utils::DetachExt(wstrKey);
			auto pTexture = RESOURCES.GetOrAddTexture(wstrKey, wstrPath);

			char pszButtonkey[MAX_PATH];
			strcpy_s(pszButtonkey, sizeof(pszButtonkey), m_pszWidgetKey_TextureList);
			strcat_s(pszButtonkey, sizeof(pszButtonkey), m_pszUniversalTextures[n]);
			if (ImGui::ImageButton(pszButtonkey, (pTexture->Get_SRV().Get()), ImVec2(60, 60)))
			{
				*m_iTexture_TextureList = n;
				*m_pTextureTag_TextureList = m_pszUniversalTextures[n];
			}

			if (0 != n % 5)
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

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		ImGui::BeginChild(pszWidgetKey, ImVec2(150,150), true, window_flags);
		if (nullptr != pTexture)
		{
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(1, 255, 1, 255));
			ImGui::Image((pTexture->Get_SRV().Get()), ImVec2(135, 135));
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

_bool Widget_EffectMaker_Mesh::Equal(_float2 vSrc, _float2 vDest)
{
	if (vSrc.x == vDest.x &&
		vSrc.y == vDest.y)
		return true;
	
	return false;
}

_bool Widget_EffectMaker_Mesh::Equal(_float3 vSrc, _float3 vDest)
{
	if (vSrc.x == vDest.x &&
		vSrc.y == vDest.y && 
		vSrc.z == vDest.z )
		return true;

	return false;
}

_bool Widget_EffectMaker_Mesh::Equal(ImVec4 vSrc, ImVec4 vDest)
{
	if (vSrc.x == vDest.x &&
		vSrc.y == vDest.y &&
		vSrc.z == vDest.z && 
		vSrc.w == vDest.w )
		return true;

	return false;
}

_bool Widget_EffectMaker_Mesh::Equal(_float* arrSrc, _float* arrDest, _int iSize)
{
	_bool bIsSame = true;
	for (_int i = 0; i < iSize; i++)
	{
		if (arrSrc[i] != arrDest[i])
			bIsSame = false;
	}

	return bIsSame;
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
