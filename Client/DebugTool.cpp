#include "pch.h"
#include "DebugTool.h"
#include "Light.h"
#include <Utils.h>
#include "Model.h"
#include "Camera.h"
#ifdef _DEBUGTOOL

bool VectorOfStringGetter(void* data, int n, const char** out_text)
{
	const vector<string>* v = (vector<string>*)data;
	*out_text = (v[0][n]).c_str();
	return true;
}
DebugTool::~DebugTool()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

HRESULT DebugTool::Init()
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	srand((_uint)time(nullptr));
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

    return S_OK;
}

void DebugTool::Tick()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (m_bIsRender)
	{
		ImGui::Begin("Debug");
		if(CUR_SCENE)
		{
			if (BeginTabBar("##a"))
			{
				RenderOptionTab();
				ModelOptionTab();
				CameraOptionTab();
				EndTabBar();
			}
		}

		End();
	}

	if (KEYTAP(KEY_TYPE::ESC))
		m_bIsRender = !m_bIsRender;
}

void DebugTool::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DebugTool::RenderOptionTab()
{
	if(BeginTabItem("Render Option"))
	{
		string fps = "FPS : " + to_string(TIME.GetFPS());
		Text(fps.c_str());
		if (CollapsingHeader("RenderOption"))
		{

			_float& g_fBrightness = CUR_SCENE->g_fBrightness;
			_float& g_fContrast = CUR_SCENE->g_fContrast;
			_float& g_Saturation = CUR_SCENE->g_Saturation;



			DragFloat("Brightness", &g_fBrightness, 0.01f, 0.f, 5.f);
			DragFloat("Contrast", &g_fContrast, 0.01f, 0.01f, 5.f);
			DragFloat("Saturation", &g_Saturation, 0.01f, 0.01f, 5.f);


		}
		if (CollapsingHeader("Bloom"))
		{
			Checkbox("Bloom On", &CUR_SCENE->g_BloomData.g_BloomOn);
			_float& g_fBloomMin = CUR_SCENE->g_BloomData.g_BloomMin;
			DragFloat("Bloom Min Value", &g_fBloomMin, 0.001f, 0.01f, 1.f);
		}
		if (CollapsingHeader("ToneMapping"))
		{
			_float& g_fMaxWhite = CUR_SCENE->g_fMaxWhite;
			_int& g_iTMIndex = CUR_SCENE->g_iTMIndex;

			static _int tmIndex = 0;
			InputInt("ToneMapping Mod", &tmIndex);

			if (tmIndex > 3) tmIndex %= 4;
			else if (tmIndex < 0) tmIndex += 4;

			g_iTMIndex = tmIndex;
			if (g_iTMIndex == 1)
				DragFloat("Max_White Value", &g_fMaxWhite, 0.1f, 0.01f, 5.f);
		}
		if (CollapsingHeader("SSAO"))
		{
			_bool& g_bSSAO_On = CUR_SCENE->g_SSAOData.g_bSSAO_On;
			Checkbox("SSAO On", &g_bSSAO_On);
			if (g_bSSAO_On)
			{
				_float& g_fOcclusionRadius = CUR_SCENE->g_SSAOData.g_fOcclusionRadius;
				_float& g_OcclusionFadeStart = CUR_SCENE->g_SSAOData.g_OcclusionFadeStart;
				_float& g_OcclusionFadeEnd = CUR_SCENE->g_SSAOData.g_OcclusionFadeEnd;

				DragFloat("SSAO Radius", &g_fOcclusionRadius, 0.01f, 0.0001f, 1.f);
				DragFloat("SSAO FadeStart", &g_OcclusionFadeStart, 0.01f, 0.0001f, g_OcclusionFadeEnd);
				DragFloat("SSAO FadeEnd", &g_OcclusionFadeEnd, 0.01f, g_OcclusionFadeStart, 1.f);
			}
		}
		if (CollapsingHeader("Motion Blur"))
		{
			Scene::MotionBlurData& data = CUR_SCENE->g_MotionBlurData;
			Checkbox("Motion Blur On", &data.g_bMotionBlurOn);
			InputInt("Motion Blur Count", &data.g_iBlurCount);

		}
		if (CollapsingHeader("Fog Option"))
		{
			_bool& g_FogOn = CUR_SCENE->g_FogData.g_FogOn;
			_float& gFogStart = CUR_SCENE->g_FogData.g_fogStart;
			_float& g_FogEnd = CUR_SCENE->g_FogData.g_fogEnd;
			_float& g_fogDensity = CUR_SCENE->g_FogData.g_fogDensity;
			_int& g_fogMode = CUR_SCENE->g_FogData.g_fogMode;
			Color& gColorFog = CUR_SCENE->g_FogData.g_fogColor;

			Checkbox("Fog On", &g_FogOn);
			DragFloat("Fog Start Range", &gFogStart, 1.f, 0.0001f, g_FogEnd);
			DragFloat("Fog End", &g_FogEnd, 1.f, gFogStart, 2000.f);
			DragFloat("Fog Density", &g_fogDensity, 0.001f, 0.001f, 1.f);
			InputInt("Fog Mod", &g_fogMode);
			if (g_fogMode < 0) g_fogMode = 0;
			if (g_fogMode > 2) g_fogMode = 2;
			static bool alpha_preview = true;
			static bool alpha_half_preview = false;
			static bool drag_and_drop = true;
			static bool options_menu = true;
			static bool hdr = false;

			ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
			ImGui::ColorEdit4("FogColor", (float*)&gColorFog, ImGuiColorEditFlags_DisplayHSV | misc_flags);

		}
		if (CollapsingHeader("Lens Flare Option"))
		{
			if (CUR_SCENE)
			{
				_bool& g_bLensFlare = CUR_SCENE->g_bLensFlare;
				Checkbox("LensFlare On", &g_bLensFlare);
			}
		}
		if (CollapsingHeader("DOF"))
		{
			Checkbox("DOF On", &CUR_SCENE->g_DOFData.g_bDOF_On);
			_float& g_FocusDepth = CUR_SCENE->g_DOFData.g_FocusDepth;
			_float& g_DOFRange = CUR_SCENE->g_DOFData.g_DOFRange;
			DragFloat("Focus Depth", &g_FocusDepth, 1.f, 0.1f, 1000.f);
			DragFloat("g_DOFRange", &g_DOFRange, 0.1f, 0.0001f, 1000.f);
		}
		if (CollapsingHeader("Light Option"))
		{
			_float& g_fSpecularPower = CUR_SCENE->g_LightPowerData.g_specularPower;
			_float& g_fRimPower = CUR_SCENE->g_LightPowerData.g_rimPower;
			DragFloat("Specular", &g_fSpecularPower, 0.1f, 0.01f);
			DragFloat("RimPower", &g_fRimPower, 0.1f, 0.01f);
			if (CUR_SCENE->Get_Light())
			{
				Color& g_lightDiffuse = CUR_SCENE->Get_Light()->Get_Light()->Get_LightInfo().color.diffuse;
				static bool alpha_preview = true;
				static bool alpha_half_preview = false;
				static bool drag_and_drop = true;
				static bool options_menu = true;
				static bool hdr = true;

				ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
				DragFloat4("color", (_float*)&g_lightDiffuse);

			}
		}
		if (CollapsingHeader("Other Option"))
		{
			SeparatorText("Outline");
			_bool& g_bOutline = CUR_SCENE->g_bDrawOutline;
			Checkbox("Outline On", &g_bOutline);

			SeparatorText("FXAA");
			_bool& g_bFXAAOn = CUR_SCENE->g_bFXAAOn;
			Checkbox("FXAA On", &g_bFXAAOn);

			SeparatorText("Aberration");
			_bool& g_bAberrationOn = CUR_SCENE->g_bAberrationOn;
			Checkbox("Aberration On", &g_bAberrationOn);
			if (g_bAberrationOn)
			{
				_float& g_fAberrationPower = CUR_SCENE->g_fAberrationPower;
				DragFloat("Aberration Power", &g_fAberrationPower, 1.f, -300.f, 300.f);
			}
			SeparatorText("PBR");
			_bool& g_bPBR_On = CUR_SCENE->g_bPBR_On;
			Checkbox("PBR On", &g_bPBR_On);
			_float& g_lightAttenuation = CUR_SCENE->g_lightAttenuation;
			_float& g_ambientRatio = CUR_SCENE->g_ambientRatio;
			DragFloat("g_lightAttenuation", &g_lightAttenuation);
			DragFloat("g_ambientRatio", &g_ambientRatio, 0.1f, 0.1f, 1.5f);
		}

		EndTabItem();
	}
	
	
}

void DebugTool::ModelOptionTab()
{
	auto model = RESOURCES.Get<Model>(L"Player");
	if (!model)
		return;

	if (BeginTabItem("Model Info"))
	{
		vector<string> partsNames[PARTS_MAX_COUNT];

		for (_uint i = 0; i < PARTS_MAX_COUNT; ++i)
		{
			const auto& parts = RESOURCES.Get_Parts(static_cast<PARTS_INFO>(i));
			for (auto& pair : parts)
			{
				partsNames[i].push_back(Utils::ToString(pair.first));
			}
		}
		static _int partsIndex[PARTS_MAX_COUNT] = {};

		for (_int i = 0; i < PARTS_MAX_COUNT; ++i)
		{
			if(i !=0 && i !=2)
				continue;
			string tag = "##PartsList" + to_string(i);
			Combo(tag.c_str(), &partsIndex[i], VectorOfStringGetter, &partsNames[i], int(partsNames[i].size()));
			SameLine();
			string buttonName = "Set Parts ##" + to_string(i);
			if (Button(buttonName.c_str()))
			{
				model->AddParts(Utils::ToWString(partsNames[i][partsIndex[i]]), static_cast<PARTS_INFO>(i));
			}
		}

		EndTabItem();
	}
}

void DebugTool::CameraOptionTab()
{
	if(CUR_SCENE->Get_MainCamera())
	{
		auto& desc = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_CameraDesc();

		if (BeginTabItem("Camera"))
		{
			DragFloat("Near", &desc.fNear, 0.1f, 0.1f, 100.f);
			DragFloat("Near", &desc.fFar, 1.f, 100.f, 5000.f);
			EndTabItem();
		}
	}
}

#endif