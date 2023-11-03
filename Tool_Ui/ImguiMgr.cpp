#include "pch.h"
#include "ImguiMgr.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Utils.h"

ImguiMgr::~ImguiMgr()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

HRESULT ImguiMgr::Initialize(HWND& hWnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

    ImGuizmo::Enable(true);
    ImGuizmo::SetRect(0.f, 0.f, g_iWinSizeX, g_iWinSizeY);

    return S_OK;
}

void ImguiMgr::Tick()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGui::ShowDemoWindow(); // Show demo window! :)


    Key_Input();

    Create_Ui();
    Manapulate_Ui();


}

HRESULT ImguiMgr::Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

void ImguiMgr::Key_Input()
{
    // F1 : on/off Create Tool
    if (KEYTAP(KEY_TYPE::F1))
       m_bRender_Create_Ui = !m_bRender_Create_Ui;

    // F2 : on/off Manapulate Tool
    if (KEYTAP(KEY_TYPE::F2))
       m_bRender_Manapulate_Ui = !m_bRender_Manapulate_Ui;


}

void ImguiMgr::Create_Ui()
{
   if (false == m_bRender_Create_Ui)
      return;

   ImGui::Begin("Create Ui");


   File_Dialog();

   Select_Texture();
   Decide_Transform_Value();
   Create_Object();


   ImGui::End();
}

void ImguiMgr::Manapulate_Ui()
{
   if (false == m_bRender_Manapulate_Ui)
      return;

   ImGui::Begin("Manapulate Ui");


   Select_Object();
   Change_Value_Object();


   Delete_Object();

   ImGui::End();
}

void ImguiMgr::File_Dialog()
{
   // open Dialog Simple
   if (ImGui::Button("Open File Dialog"))
      ImGuiFileDialog::Instance()->OpenDialog("ddsFileDlgKey", "dds File Dialog", ".dds", ".");

   // display
   if (ImGuiFileDialog::Instance()->Display("ddsFileDlgKey"))
   {
      // action if OK
      if (ImGuiFileDialog::Instance()->IsOk())
      {
         string strPath = ImGuiFileDialog::Instance()->GetFilePathName();
         string strFileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

         wstring path = Utils::ToWString(strPath);
         wstring FileName = Utils::ToWString(strFileName);

         size_t pos = path.find(L"Resources");
         if (pos != std::wstring::npos) {
            path.erase(0, pos);
         }
         path = L"../" + path;

         size_t dotPos = FileName.rfind(L'.');
         if (dotPos != std::wstring::npos) {
            FileName.erase(dotPos);
         }

         RESOURCES.Load<Texture>(FileName, path);
      }

      // close
      ImGuiFileDialog::Instance()->Close();
   }
}

void ImguiMgr::Select_Texture()
{
   auto textures = RESOURCES.Get_Resources(0);
   auto texture = textures[static_cast<_uint>(ResourceType::Texture)];

   _uint iSize = static_cast<_uint>(texture.size());
   _uint iIndex = 0;

   char inputBuffer[256] = {};
   if (ImGui::InputText("Search", inputBuffer, sizeof(inputBuffer))) {
      string str(inputBuffer);
      m_strSearchTexture = Utils::ToWString(str);
   }

   if (ImGui::BeginListBox("texture"))
   {
      for (const auto& pair : texture) {

         if (pair.first.find(m_strSearchTexture) == std::wstring::npos) {
            continue;
         }

         string key = Utils::ToString(pair.first);
         
         const bool is_selected = (iIndex == iSize);

         if (ImGui::Selectable(key.c_str(), iIndex == m_iTextureCursor))
         {
            m_iTextureCursor = iIndex;
            m_strKeyTexture = pair.first;
         }

         ++iIndex;
      }

      ImGui::EndListBox();
   }
}

void ImguiMgr::Decide_Transform_Value()
{
   ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

   ImGui::NewLine();
   ImGui::Text("Texture", IMGUI_VERSION, IMGUI_VERSION_NUM);

   string strName = Utils::ToString(m_strKeyTexture);
   ImGui::SetNextItemWidth(200);
   if (ImGui::InputText("Name", strName.data(), 256)) {
   }

   ImGui::NewLine();
   ImGui::Text("Position", IMGUI_VERSION, IMGUI_VERSION_NUM);

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("X##PosX", &m_vecPos.x, 0.01f, 1.0f, "%.3f", eFlag);

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("Y##PosY", &m_vecPos.y, 0.01f, 1.0f, "%.3f", eFlag);

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("Z##PosZ", &m_vecPos.z, 0.01f, 1.0f, "%.3f", eFlag);

   ImGui::NewLine();
   ImGui::Text("Scale", IMGUI_VERSION, IMGUI_VERSION_NUM);

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("X##ScaleX", &m_vecScale.x, 0.01f, 1.0f, "%.3f", eFlag);

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("Y##ScaleY", &m_vecScale.y, 0.01f, 1.0f, "%.3f", eFlag);

   ImGui::NewLine();
   ImGui::SetNextItemWidth(200);

   char inputBuffer[256] = {};
   if (ImGui::InputText("Object Name", inputBuffer, sizeof(inputBuffer))) {
      string str(inputBuffer);
      m_strName = Utils::ToWString(str);
   }

}

void ImguiMgr::Create_Object()
{
   if (ImGui::Button("Create Ui", ImVec2(80.f, 20.f)))
   {
      if (!ImGui::IsItemActive())
      {
         auto UiObject = make_shared<GameObject>();
         //_float4x4 matWorld = XMMatrixScaling(m_vecScale.x, m_vecScale.y, 1.f) * XMMatrixTranslation(m_vecPos.x, m_vecPos.y, m_vecPos.z);
         //UiObject->GetOrAddTransform()->Set_WorldMat(matWorld);
         UiObject->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(m_vecPos.x, m_vecPos.y, m_vecPos.z, 1));
         UiObject->GetOrAddTransform()->Scaled(_float3(m_vecScale.x, m_vecScale.y, 1.f));
         shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx"));

         auto mesh = RESOURCES.Get<Mesh>(L"Quad");
         renderer->Set_Mesh(mesh);

         auto material = make_shared<Material>();
         material->Set_TextureMap(RESOURCES.Get<Texture>(m_strKeyTexture), TextureMapType::DIFFUSE);
         renderer->Get_RenderParamDesc().SetVec4(0, _float4(1));
         renderer->Set_Material(material);
         renderer->Set_Pass(MeshRenderer::PASS_INFO::Default_UI);
         UiObject->Add_Component(renderer);

         UiObject->Set_Name(m_strName);
         m_GameobjectName.push_back(m_strName);

         UiObject->Set_LayerIndex(Layer_UI);
         UiObject->Set_Instancing(false);
         CUR_SCENE->Add_GameObject(UiObject);
      }
   }
}

void ImguiMgr::Select_Object()
{
   ImGui::NewLine();
   _uint iSize = static_cast<_uint>(m_GameobjectName.size());
   _uint iIndex = 0;

   if (ImGui::BeginListBox("ObjName"))
   {
      for (const auto& Name : m_GameobjectName) {
         string key = Utils::ToString(Name);

         const bool is_selected = (iIndex == iSize);

         if (ImGui::Selectable(key.c_str(), iIndex == m_iObjNameCursor))
         {
            m_iObjNameCursor = iIndex;
            m_strSelectObjName = Name;
         }

         ++iIndex;
      }

      ImGui::EndListBox();
   }
}

void ImguiMgr::Delete_Object()
{
   if (ImGui::Button("Delete", ImVec2(80.f, 20.f)))
   {
      if (!ImGui::IsItemActive())
      {
         CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(m_strSelectObjName));

         for (auto iter = m_GameobjectName.begin();
            iter != m_GameobjectName.end();
            ++iter)
         {
            if (*iter == m_strSelectObjName)
            {
               iter = m_GameobjectName.erase(iter);
               break;
            }
            else
            {
               ++iter;
            }
         }
      }
   }
}

void ImguiMgr::Change_Value_Object()
{
   ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("X##PosX2", &m_vecPos.x, 0.01f, 1.0f, "%.3f", eFlag);

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("Y##PosY2", &m_vecPos.y, 0.01f, 1.0f, "%.3f", eFlag);

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("Z##PosZ2", &m_vecPos.z, 0.01f, 1.0f, "%.3f", eFlag);

   ImGui::NewLine();
   ImGui::Text("Scale", IMGUI_VERSION, IMGUI_VERSION_NUM);

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("X##ScaleX2", &m_vecScale.x, 0.01f, 1.0f, "%.3f", eFlag);

   ImGui::SetNextItemWidth(200);
   ImGui::InputFloat("Y##ScaleY2", &m_vecScale.y, 0.01f, 1.0f, "%.3f", eFlag);

   if (ImGui::Button("Change", ImVec2(80.f, 20.f)) || m_bIsChangeCheck)
   {
      if (!ImGui::IsItemActive() || m_bIsChangeCheck)
      {
         auto pGameobject = CUR_SCENE->Get_GameObject(m_strSelectObjName);
         if (nullptr == pGameobject)
         {
            return;
         }

         auto transform = pGameobject->Get_Transform();

         transform->Set_State(Transform_State::POS, _float4(m_vecPos.x, m_vecPos.y, m_vecPos.z, 1));
         transform->Scaled(_float3(m_vecScale.x, m_vecScale.y, 1.f));
      }
   }

   ImGui::SameLine();
   ImGui::Checkbox("Immediately", &m_bIsChangeCheck);
}
