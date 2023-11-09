#include "pch.h"
#include "DebugTool.h"
#ifdef _DEBUGTOOL
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
		if (BeginTabBar("##a"))
		{
			RenderOptionTab();
			FogOptionTab();
			LensFlareTab();
			EndTabBar();
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

	if (BeginTabItem("Render Option"))
	{
		string fps  = to_string(TIME.GetFPS());
		Text(fps.c_str());
		_float& g_fBrightness = GAMEINSTANCE.g_fBrightness;
		_float& g_fContrast = GAMEINSTANCE.g_fContrast;
		_float& g_fMaxWhite = GAMEINSTANCE.g_fMaxWhite;
		_int& g_iTMIndex = GAMEINSTANCE.g_iTMIndex;
		_float& g_fGamma = GAMEINSTANCE.g_fGamma;
		_float& g_fBloomMin = GAMEINSTANCE.g_fBloomMin;
		
		DragFloat("Brightness", &g_fBrightness, 0.001f, 0.01f, 5.f);
		DragFloat("Contrast", &g_fContrast, 0.001f, 0.01f, 5.f);
		DragFloat("Gamma", &g_fGamma, 0.001f, 0.01f, 5.f);
		SeparatorText("Bloom");
		DragFloat("Bloom Min Value", &g_fBloomMin, 0.001f, 0.01f, 1.f);
		
		static _int tmIndex = 0;
		SeparatorText("Tone Mapping");
		InputInt("ToneMapping Mod", &tmIndex);

		if (tmIndex > 3) tmIndex %= 4;
		else if (tmIndex < 0) tmIndex += 4;

		g_iTMIndex = tmIndex;
		if(g_iTMIndex == 1)
			DragFloat("Max_White Value", &g_fMaxWhite, 0.1f, 0.01f, 5.f);
		
		SeparatorText("SSAO");
		_bool& g_bSSAO_On = GAMEINSTANCE.g_SSAOData.g_bSSAO_On;
		Checkbox("SSAO On", &g_bSSAO_On);
		if (g_bSSAO_On)
		{
			_float& g_fOcclusionRadius = GAMEINSTANCE.g_SSAOData.g_fOcclusionRadius;
			_float& g_OcclusionFadeStart = GAMEINSTANCE.g_SSAOData.g_OcclusionFadeStart;
			_float& g_OcclusionFadeEnd = GAMEINSTANCE.g_SSAOData.g_OcclusionFadeEnd;

			DragFloat("SSAO Radius", &g_fOcclusionRadius, 0.01f, 0.0001f, 1.f);
			DragFloat("SSAO FadeStart", &g_OcclusionFadeStart, 0.01f, 0.0001f, g_OcclusionFadeEnd);
			DragFloat("SSAO FadeEnd", &g_OcclusionFadeEnd, 0.01f, g_OcclusionFadeStart, 1.f);


		}
		SeparatorText("Outline");
		_bool& g_bOutline = GAMEINSTANCE.g_bDrawOutline;
		Checkbox("Outline On", &g_bOutline);

		SeparatorText("FXAA");
		_bool& g_bFXAAOn = GAMEINSTANCE.g_bFXAAOn;
		Checkbox("FXAA On", &g_bFXAAOn);

		SeparatorText("Aberration");
		_bool& g_bAberrationOn = GAMEINSTANCE.g_bAberrationOn;
		Checkbox("Aberration On", &g_bAberrationOn);
		if (g_bAberrationOn)
		{
			_float& g_fAberrationPower = GAMEINSTANCE.g_fAberrationPower;
			DragFloat("Aberration Power", &g_fAberrationPower, 1.f,-300.f, 300.f);
		}
		EndTabItem();
	}
}
void DebugTool::FogOptionTab()
{
	if (BeginTabItem("Fog Option"))
	{
		SeparatorText("Fog");
		_bool& g_FogOn = GAMEINSTANCE.g_FogData.g_FogOn;
		_float& g_FogRange = GAMEINSTANCE.g_FogData.gFogRange;
		_float& gFogStart = GAMEINSTANCE.g_FogData.gFogStart;
		Color& gColorFog = GAMEINSTANCE.g_FogData.gFogColor;

		Checkbox("Fog On", &g_FogOn);
		DragFloat("Fog Range", &g_FogRange, 1.f, gFogStart, 2000.f);
		DragFloat("Fog Start Range", &gFogStart, 1.f, 0.0001f, g_FogRange);
		static bool alpha_preview = true;
		static bool alpha_half_preview = false;
		static bool drag_and_drop = true;
		static bool options_menu = true;
		static bool hdr = false;

		ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit4("FogColor", (float*)&gColorFog, ImGuiColorEditFlags_DisplayHSV | misc_flags);

		EndTabItem();
	}
}
void DebugTool::LensFlareTab()
{
	if (BeginTabItem("Flare Option"))
	{
		if (CUR_SCENE)
		{
			_bool& g_bLensFlare = GAMEINSTANCE.g_bLensFlare;
			Checkbox("LensFlare On", &g_bLensFlare);
			DragFloat3("Test1", (_float*)&GAMEINSTANCE.g_testVec1,0.01f);
			DragFloat3("Test2", (_float*)&GAMEINSTANCE.g_testVec2, 0.01f);
		}
		EndTabItem();
	}
}
#endif