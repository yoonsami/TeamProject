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

			Option_TextureOp(0);

			if (!m_bUseSpriteAnimation)
			{
				Option_TextureOp(1);
				Option_TextureOp(2);
			}

			Option_Normal();
			Option_Blend();
			Option_Overlay();
			Option_Dissolve();
			Option_Distortion();

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
				m_bSaveMsgBox_On = true;
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

		ImGui::Text(" ");
		if (ImGui::Button("   Ok   "))
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

	const char* pszItem_Sampler[] = { "Wrap", "Clamp", "Mirror", "Border"};
	if (ImGui::BeginCombo("Sampler##Property", pszItem_Sampler[m_iSamplerType], 0))
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

void Widget_EffectMaker_Mesh::Option_TextureOp(_int iIndex)
{
	string strIndex = to_string(iIndex);
	string strSeparatorTag = "Texture " + strIndex;
	string strIsOn = "Texture Option On##" + strIndex;
	string strChild1 = "##Child1_" + strIndex;
	string strImageViewer = "##Img_" + strIndex;
	string strTextureButton = "Change Texture##" + strIndex;
	string strTextureList = "Texture List##" + strIndex;
	string strColoringOptionCombo = "Color Option##" + strIndex;
	string strColorEdit_Base1 = "Color Base1 ##" + strIndex;
	string strColorEdit_Base2 = "Color Base2 ##" + strIndex;
	string strColorEdit_Dest1 = "Color Dest1 ##" + strIndex;
	string strColorEdit_Dest2 = "Color Dest2 ##" + strIndex;
	string strSolidColorCheckBox = "Use solid color##" + strIndex;
	string strTiling = "Tiling(x,y)##" + strIndex;
	string strUVSpeed = "UV Speed(x,y)##" + strIndex;
	string strContrast = "Contrast##" + strIndex;
	string strAlphaOffset = "Alpha Offset##" + strIndex;

	ImGui::SeparatorText(strSeparatorTag.c_str());

	// For. On/Off Texture Option
	ImGui::Checkbox(strIsOn.c_str(), &m_TexOption[iIndex].bIsOption_On);

	if (!m_TexOption[iIndex].bIsOption_On)
	{
		m_TexOption[iIndex].Texture.first = 0;
		m_TexOption[iIndex].Texture.second = "None";
		return;
	}

	// For. Texture 
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		ImGui::BeginChild(strChild1.c_str(), ImVec2(280, 170), false, window_flags);

		SubWidget_ImageViewer(m_TexOption[iIndex].Texture.second, m_strTexturePath, strImageViewer.c_str());

		ImGui::SameLine();

		if (ImGui::Button(strTextureButton.c_str()))
		{
			m_iTexture_TextureList = &m_TexOption[iIndex].Texture.first;
			m_pTextureTag_TextureList = &m_TexOption[iIndex].Texture.second;
			const char* pszTextureListTag = strTextureList.c_str();
			m_pszWidgetKey_TextureList = pszTextureListTag;
			m_bTextureList_On = true;
		}

		ImGui::EndChild();
	}

	// For. Color 
	const char* pszItem_ColoringOptions[] = { "Custom", "Use Texture Color"};
	if (ImGui::BeginCombo(strColoringOptionCombo.c_str(), pszItem_ColoringOptions[m_TexOption[iIndex].iColoringOption], 0))
	{
		for (_uint n = 0; n < IM_ARRAYSIZE(pszItem_ColoringOptions); n++)
		{
			const bool is_selected = (m_TexOption[iIndex].iColoringOption == n);
			if (ImGui::Selectable(pszItem_ColoringOptions[n], is_selected))
				m_TexOption[iIndex].iColoringOption = n;
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::Checkbox(strSolidColorCheckBox.c_str(), &m_TexOption[iIndex].bUseSolidColor);

	if (0 == m_TexOption[iIndex].iColoringOption)
	{
		if (m_TexOption[iIndex].bUseSolidColor)
		{
			ImGui::ColorEdit4(strColorEdit_Base1.c_str(), (float*)&m_TexOption[iIndex].vColorBase1, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
			m_TexOption[iIndex].vColorBase2 = m_TexOption[iIndex].vColorBase1;

			if (m_bColorChangingOn)
			{
				ImGui::ColorEdit4(strColorEdit_Dest1.c_str(), (float*)&m_TexOption[iIndex].vColorDest1, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
				m_TexOption[iIndex].vColorDest2 = m_TexOption[iIndex].vColorDest1;
			}
		}
		else
		{
			ImGui::ColorEdit4(strColorEdit_Base1.c_str(), (float*)&m_TexOption[iIndex].vColorBase1, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
			ImGui::ColorEdit4(strColorEdit_Base2.c_str(), (float*)&m_TexOption[iIndex].vColorBase2, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

			if (m_bColorChangingOn)
			{
				ImGui::ColorEdit4(strColorEdit_Dest1.c_str(), (float*)&m_TexOption[iIndex].vColorDest1, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
				ImGui::ColorEdit4(strColorEdit_Dest2.c_str(), (float*)&m_TexOption[iIndex].vColorDest2, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
			}
		}
	}
	else
	{
		m_TexOption[iIndex].vColorBase1 = ImVec4(1.f, 1.f, 1.f, 1.f);
		m_TexOption[iIndex].vColorBase2 = ImVec4(1.f, 1.f, 1.f, 1.f);
		m_TexOption[iIndex].vColorDest1 = ImVec4(1.f, 1.f, 1.f, 1.f);
		m_TexOption[iIndex].vColorDest2 = ImVec4(1.f, 1.f, 1.f, 1.f);
	}

	// For. Texcoord Options
	ImGui::InputFloat(strContrast.c_str(), &m_TexOption[iIndex].fContrast);
	ImGui::InputFloat(strAlphaOffset.c_str(), &m_TexOption[iIndex].fAlphaOffset);
	SubWidget_SettingTexUV(m_TexOption[iIndex].fTiling_Op, m_TexOption[iIndex].fUVSpeed_Op, strTiling.c_str(), strUVSpeed.c_str());
}

void Widget_EffectMaker_Mesh::Option_Normal()
{
	ImGui::SeparatorText("Normal");

	ImGui::Checkbox("Normal On", &m_bNormal_On);

	if (!m_bNormal_On)
	{
		m_NormalTexture.first = 0;
		m_NormalTexture.second = "None";
		return;
	}

	SubWidget_ImageViewer(m_NormalTexture.second, m_strTexturePath, "##Img_Normal");

	ImGui::SameLine();

	if (ImGui::Button("Change Texture##Normal"))
	{
		m_iTexture_TextureList = &m_NormalTexture.first;
		m_pTextureTag_TextureList = &m_NormalTexture.second;
		m_pszWidgetKey_TextureList = "Texture List##Normal";
		m_bTextureList_On = true;
	}

	ImGui::Spacing();
}

void Widget_EffectMaker_Mesh::Option_Blend()
{
	ImGui::SeparatorText("Blend");

	ImGui::Checkbox("Blend On", &m_bBlend_On);

	if (!m_bBlend_On)
	{
		m_BlendTexture.first = 0;
		m_BlendTexture.second = "None";
		return;
	}

	SubWidget_ImageViewer(m_BlendTexture.second, m_strTexturePath, "##Img_Blend");

	ImGui::SameLine();
	if (ImGui::Button("Change Texture##Blend"))
	{
		m_iTexture_TextureList = &m_BlendTexture.first;
		m_pTextureTag_TextureList = &m_BlendTexture.second;
		m_pszWidgetKey_TextureList = "Texture List##Blend";
		m_bTextureList_On = true;
	}

	SubWidget_SettingTexUV(m_fTiling_Blend, m_fUVSpeed_Blend, "Tiling(x,y)##Blend", "UV Speed(x,y)##Blend");
	ImGui::InputFloat("Alpha Offset##Blend", &m_fAlphaOffset_Blend);
}

void Widget_EffectMaker_Mesh::Option_Overlay()
{
	ImGui::SeparatorText("Overlay");

	ImGui::Checkbox("Overlay On", &m_bOverlay_On);

	if (!m_bOverlay_On)
	{
		m_OverlayTexture.first = 0;
		m_OverlayTexture.second = "None";
		return;
	}

	SubWidget_ImageViewer(m_OverlayTexture.second, m_strTexturePath, "##Img_Overlay");

	ImGui::SameLine();
	if (ImGui::Button("Change Texture##Overlay"))
	{
		m_iTexture_TextureList = &m_OverlayTexture.first;
		m_pTextureTag_TextureList = &m_OverlayTexture.second;
		m_pszWidgetKey_TextureList = "Texture List##Overlay";
		m_bTextureList_On = true;
	}

	// For. Color 
	ImGui::ColorEdit4("Color##Overlay", (float*)&m_vOverlayColor_Base, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);
	if(m_bColorChangingOn)
		ImGui::ColorEdit4("Dest Color##Overlay", (float*)&m_vOverlayColor_Dest, ImGuiColorEditFlags_DisplayHSV | ColorEdit_flags);

	// For. Texcoord Options
	SubWidget_SettingTexUV(m_fTiling_Overlay, m_fUVSpeed_Overlay, "Tiling(x,y)##Overlay", "UV Speed(x,y)##Overlay");
}

void Widget_EffectMaker_Mesh::Option_Dissolve()
{
	ImGui::SeparatorText("Dissolve");

	ImGui::Checkbox("Dissolve On", &m_bDissolve_On);

	if (!m_bDissolve_On)
	{
		m_DissolveTexture.first = 0;
		m_DissolveTexture.second = "None";
		return;
	}

	SubWidget_ImageViewer(m_DissolveTexture.second, m_strTexturePath, "##Img_Dissolve");
	
	ImGui::SameLine();
	if (ImGui::Button("Change Texture##Dissolve"))
	{
		m_iTexture_TextureList = &m_DissolveTexture.first;
		m_pTextureTag_TextureList = &m_DissolveTexture.second;
		m_pszWidgetKey_TextureList = "Texture List##Dissolve";
		m_bTextureList_On = true;
	}
	
	SubWidget_SettingTexUV(m_fTiling_Dissolve, m_fUVSpeed_Dissolve, "Tiling(x,y)##Dissolve", "UV Speed(x,y)##Dissolve");
}

void Widget_EffectMaker_Mesh::Option_Distortion()
{
	ImGui::SeparatorText("Distortion");

	ImGui::Checkbox("Distortion On", &m_bDistortion_On);

	if (!m_bDistortion_On)
	{
		m_DistortionTexture.first = 0;
		m_DistortionTexture.second = "None";
		return;
	}

	SubWidget_ImageViewer(m_DistortionTexture.second, m_strTexturePath, "##Img_Distortion");

	ImGui::SameLine();

	if (ImGui::Button("Change Texture##Distortion"))
	{
		m_iTexture_TextureList = &m_DistortionTexture.first;
		m_pTextureTag_TextureList = &m_DistortionTexture.second;
		m_pszWidgetKey_TextureList = "Texture List##Distortion";
		m_bTextureList_On = true;
	}

	SubWidget_SettingTexUV(m_fTiling_Distortion, m_fUVSpeed_Distortion, "Tiling(x,y)##Distortion", "UV Speed(x,y)##Distortion");
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

		ImGui::InputFloat3("Offset in tool##InitTransform", m_fPosOffsetInTool);

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
	EffectObj->Get_Transform()->Set_State(Transform_State::POS, _float4(m_fPosOffsetInTool[0], m_fPosOffsetInTool[1], m_fPosOffsetInTool[2], 1.f));
	EffectObj->Get_Transform()->Scaled(_float3(1.0f));

	// For. Add and Setting Effect Component to GameObject
	shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Effect2.fx");
	shared_ptr<MeshEffect> meshEffect = make_shared<MeshEffect>(shader);
	EffectObj->Add_Component(meshEffect);

	if (!m_bColorChangingOn)
	{
		for (_int i = 0; i < m_iTexOptionCnt; i++)
		{
			m_TexOption[i].vColorDest1 = m_TexOption[i].vColorBase1;
			m_TexOption[i].vColorDest2 = m_TexOption[i].vColorBase2;
		}
		m_vOverlayColor_Dest = m_vOverlayColor_Base;
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
		m_iSamplerType,

		m_strMesh,

		m_bUseSpriteAnimation,
		m_iSpriteAni_Count[0],
		m_iSpriteAni_Count[1],
		m_fSpriteAni_Speed,

		m_bColorChangingOn,

		m_TexOption[0].Texture.second,
		m_TexOption[0].iColoringOption,
		ImVec4toColor(m_TexOption[0].vColorBase1),
		ImVec4toColor(m_TexOption[0].vColorBase2),
		ImVec4toColor(m_TexOption[0].vColorDest1),
		ImVec4toColor(m_TexOption[0].vColorDest2),
		_float2(m_TexOption[0].fTiling_Op),
		_float2(m_TexOption[0].fUVSpeed_Op),
		m_TexOption[0].fContrast,
		m_TexOption[0].fAlphaOffset,

		m_TexOption[1].Texture.second,
		m_TexOption[1].iColoringOption,
		ImVec4toColor(m_TexOption[1].vColorBase1),
		ImVec4toColor(m_TexOption[1].vColorBase2),
		ImVec4toColor(m_TexOption[1].vColorDest1),
		ImVec4toColor(m_TexOption[1].vColorDest2),
		_float2(m_TexOption[1].fTiling_Op),
		_float2(m_TexOption[1].fUVSpeed_Op),
		m_TexOption[1].fContrast,
		m_TexOption[1].fAlphaOffset,

		m_TexOption[2].Texture.second,
		m_TexOption[2].iColoringOption,
		ImVec4toColor(m_TexOption[2].vColorBase1),
		ImVec4toColor(m_TexOption[2].vColorBase2),
		ImVec4toColor(m_TexOption[2].vColorDest1),
		ImVec4toColor(m_TexOption[2].vColorDest2),
		_float2(m_TexOption[2].fTiling_Op),
		_float2(m_TexOption[2].fUVSpeed_Op),
		m_TexOption[2].fContrast,
		m_TexOption[2].fAlphaOffset,

		// Blend
		m_BlendTexture.second,
		_float2(m_fTiling_Blend),
		_float2(m_fUVSpeed_Blend),
		m_fAlphaOffset_Blend,

		// Overlay
		m_bOverlay_On,
		m_OverlayTexture.second,
		ImVec4toColor(m_vOverlayColor_Base),
		ImVec4toColor(m_vOverlayColor_Dest),
		_float2(m_fTiling_Overlay),
		_float2(m_fUVSpeed_Overlay),

		// Normal
		m_NormalTexture.second,

		// Diffolve
		m_DissolveTexture.second,
		_float2(m_fTiling_Overlay),
		_float2(m_fUVSpeed_Overlay),
		m_bDissolveInverse,

		// Distortion
		m_DistortionTexture.second,
		_float2(m_fTiling_Distortion),
		_float2(m_fUVSpeed_Distortion),

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
		file->Write<_int>(m_iSamplerType);

		/* Mesh */
		file->Write<string>(m_strMesh);

		/* Sprite Animation */
		file->Write<_bool>(m_bUseSpriteAnimation);
		file->Write<_int>(m_iSpriteAni_Count[0]);
		file->Write<_int>(m_iSpriteAni_Count[1]);
		file->Write<_float>(m_fSpriteAni_Speed);

		/* Coloring Option */
		file->Write<_bool>(m_bColorChangingOn);

		/* Option */
		for (_int i = 0; i < m_iTexOptionCnt; i++)
		{
			file->Write<string>(m_TexOption[i].Texture.second);
			file->Write<_int>(m_TexOption[i].iColoringOption);
			file->Write<_float4>(ImVec4toColor(m_TexOption[i].vColorBase1));
			file->Write<_float4>(ImVec4toColor(m_TexOption[i].vColorBase2));
			file->Write<_float4>(ImVec4toColor(m_TexOption[i].vColorDest1));
			file->Write<_float4>(ImVec4toColor(m_TexOption[i].vColorDest2));
			file->Write<_float2>(_float2(m_TexOption[i].fTiling_Op));
			file->Write<_float2>(_float2(m_TexOption[i].fUVSpeed_Op));
			file->Write<_float>(m_TexOption[i].fContrast);
			file->Write<_float>(m_TexOption[i].fAlphaOffset);
		}

		/* Blend */
		file->Write<string>(m_BlendTexture.second);
		file->Write<_float2>(_float2(m_fTiling_Blend));
		file->Write<_float2>(_float2(m_fUVSpeed_Blend));
		file->Write<_float>(m_fAlphaOffset_Blend);

		/* Overlay */
		file->Write<_bool>(m_bOverlay_On);
		file->Write<string>(m_OverlayTexture.second);
		file->Write<_float4>(ImVec4toColor(m_vOverlayColor_Base));
		file->Write<_float4>(ImVec4toColor(m_vOverlayColor_Dest));
		file->Write<_float2>(_float2(m_fTiling_Overlay));
		file->Write<_float2>(_float2(m_fUVSpeed_Overlay));

		/* Normal */
		file->Write<string>(m_NormalTexture.second);

		/* Dissolve */
		file->Write<string>(m_DissolveTexture.second);
		file->Write<_float2>(_float2(m_fTiling_Dissolve));
		file->Write<_float2>(_float2(m_fUVSpeed_Dissolve));
		file->Write<_bool>(m_bDissolveInverse);

		/* Distortion */
		file->Write<string>(m_DistortionTexture.second);
		file->Write<_float2>(_float2(m_fTiling_Distortion));
		file->Write<_float2>(_float2(m_fUVSpeed_Distortion));

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
	_float3 vTemp_vec3 = { 0.f , 0.f, 0.f };
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
	m_iSamplerType = file->Read<_int>();
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

	/* Option */
	for (_int i = 0; i < m_iTexOptionCnt; i++)
	{
		m_TexOption[i].Texture.second = file->Read<string>();
		m_TexOption[i].Texture.first = GetIndex_FromTexList(m_TexOption[i].Texture.second);
		if (0 != m_TexOption[i].Texture.first)
			m_TexOption[i].bIsOption_On = true;
		m_TexOption[i].iColoringOption = file->Read<_int>();
		m_TexOption[i].vColorBase1 = ColorToImVec4(file->Read<_float4>());
		m_TexOption[i].vColorBase2 = ColorToImVec4(file->Read<_float4>());
		m_TexOption[i].vColorDest1 = ColorToImVec4(file->Read<_float4>());
		m_TexOption[i].vColorDest2 = ColorToImVec4(file->Read<_float4>());
		vTemp_vec2 = file->Read<_float2>();
		memcpy(m_TexOption[i].fTiling_Op, &vTemp_vec2, sizeof(m_TexOption[i].fTiling_Op));
		vTemp_vec2 = file->Read<_float2>();
		memcpy(m_TexOption[i].fUVSpeed_Op, &vTemp_vec2, sizeof(m_TexOption[i].fUVSpeed_Op));
		m_TexOption[i].fContrast = file->Read<_float>();
		m_TexOption[i].fAlphaOffset = file->Read<_float>();
		m_TexOption[i].bUseSolidColor = Equal(m_TexOption[i].vColorBase1, m_TexOption[i].vColorBase2) && Equal(m_TexOption[i].vColorDest1, m_TexOption[i].vColorDest2);	
	}

	/* Blend */
	m_BlendTexture.second = file->Read<string>();
	m_BlendTexture.first = GetIndex_FromTexList(m_BlendTexture.second);
	if (0 == m_BlendTexture.first)
		m_bBlend_On = false;
	else
		m_bBlend_On = true;
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fTiling_Blend, &vTemp_vec2, sizeof(m_fTiling_Blend));
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fUVSpeed_Blend, &vTemp_vec2, sizeof(m_fUVSpeed_Blend));
	m_fAlphaOffset_Blend = file->Read<_float>();

	/* Overlay */
	m_bOverlay_On = file->Read<_bool>();
	m_OverlayTexture.second = file->Read<string>();
	m_vOverlayColor_Base = ColorToImVec4(file->Read<_float4>());
	m_vOverlayColor_Dest = ColorToImVec4(file->Read<_float4>());
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fTiling_Overlay, &vTemp_vec2, sizeof(m_fTiling_Overlay));
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fUVSpeed_Overlay, &vTemp_vec2, sizeof(m_fUVSpeed_Overlay));
	m_OverlayTexture.first = GetIndex_FromTexList(m_OverlayTexture.second);

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
	if (0 == m_DissolveTexture.first) m_bDissolve_On = false;
	else m_bDissolve_On = true;

	/* Distortion */
	m_DistortionTexture.second = file->Read<string>();
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fTiling_Distortion, &vTemp_vec2, sizeof(m_fTiling_Dissolve));
	vTemp_vec2 = file->Read<_float2>();
	memcpy(m_fUVSpeed_Distortion, &vTemp_vec2, sizeof(m_fUVSpeed_Distortion));
	m_DistortionTexture.first = GetIndex_FromTexList(m_DistortionTexture.second);
	if (0 == m_DistortionTexture.first) m_bDistortion_On = false;
	else m_bDistortion_On = true;

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
	ImGui::InputFloat2(pszWidgetKey1, arrTiling);
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
