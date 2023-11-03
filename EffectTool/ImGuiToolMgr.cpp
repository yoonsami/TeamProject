#include "ImGuiToolMgr.h"

/* Widgets */
#include "Widget_MeshEffectTool.h"
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

    // For. Setting Widgets
    m_pWidget_ParticleEffectTool = make_shared<Widget_ParticleEffectTool>();
    m_pWidget_MeshEffectTool = make_shared<Widget_MeshEffectTool>();
    m_pWidget_ParticleEffectTool->Initialize();
    m_pWidget_MeshEffectTool->Initialize();
}

void ImGuiToolMgr::Tick()
{
    // For. Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // For. Basic Widget
    ImGui::SetNextWindowPos(ImVec2(0, g_iWinSizeY - 50.f));
    ImGui::Begin("Basic Widget");
    ImGui_BasicWidget();
    ImGui::End();
    
    if (m_bIsParticleEffectTool_On)
        m_pWidget_ParticleEffectTool->Tick();
    if (m_bIsMeshEffectTool_On)
        m_pWidget_MeshEffectTool->Tick();
}

void ImGuiToolMgr::Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiToolMgr::ImGui_BasicWidget()
{
    ImGui::Checkbox("MeshEffect Tool", &m_bIsMeshEffectTool_On);
    ImGui::SameLine();
    ImGui::Checkbox("Particle Tool", &m_bIsParticleEffectTool_On);
}
