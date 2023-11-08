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
			RenderOptionTap();

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

void DebugTool::RenderOptionTap()
{

	if (BeginTabItem("Render Option"))
	{
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

		EndTabItem();
	}
}
void DebugTool::FogOptionTap()
{
	if (BeginTabItem("Fog Option"))
	{
		SeparatorText("Fog");
		_bool& g_FogOn = GAMEINSTANCE.g_FogData.g_FogOn;
		_float& g_FogRange = GAMEINSTANCE.g_FogData.gFogRange;
		_float& gFogStart = GAMEINSTANCE.g_FogData.gFogStart;
		Color& gColorFog = GAMEINSTANCE.g_FogData.gFogColor;
		EndTabItem();
	}
}
#endif