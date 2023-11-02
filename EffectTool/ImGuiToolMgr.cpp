#include "ImGuiToolMgr.h"

/* Widgets */
#include "Widget_ParticleEffectTool.h"

ImGuiToolMgr::~ImGuiToolMgr()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiToolMgr::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

    ImGuizmo::Enable(true);
    ImGuizmo::SetRect(0.f, 0.f, g_iWinSizeX, g_iWinSizeY);
}

void ImGuiToolMgr::Tick()
{
    // For. Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // For. Basic Widget
    //ImGui::ShowDemoWindow();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Basic Widget");
    ImGui_BasicWidget();
    ImGui::End();
    
    //if (m_bIsParticleEffectTool_On)
    //    m_pWidget_ParticleEffectTool->Tick();
}

void ImGuiToolMgr::Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiToolMgr::ImGui_BasicWidget()
{
    ImGui::Checkbox("Particle Tool", &m_bIsParticleEffectTool_On);
}

