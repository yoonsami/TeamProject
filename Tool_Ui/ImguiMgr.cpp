#include "pch.h"
#include "ImguiMgr.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Utils.h"
#include "BaseUI.h"
#include "FileUtils.h"
#include "Camera.h"
#include "Texture.h"

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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

    ImGuizmo::Enable(true);
    ImGuizmo::SetRect(0.f, 0.f, g_iWinSizeX, g_iWinSizeY);

    m_pSampleObj = make_shared<GameObject>();
    m_pSampleObj->GetOrAddTransform();

    m_tagParamDesc.vec4Params[0] = _float4(1.f, 1.f, 1.f, 1.f);

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

    Parameter_Ui();


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

    // F3 : on/off Manapulate Tool
    if (KEYTAP(KEY_TYPE::F3))
        m_bRender_Param_Ui = !m_bRender_Param_Ui;


}

void ImguiMgr::Create_Ui()
{
    if (false == m_bRender_Create_Ui)
        return;

    ImGui::Begin("Create Ui");


    File_Dialog();

    Create_Object();
    Select_Texture();
    Add_Texture();
    Decide_Transform_Value();

    // 경로 추출, 저장, 로드 다 포함
    Save_Ui_Desc();


    ImGui::End();
}

void ImguiMgr::Manapulate_Ui()
{
    if (false == m_bRender_Manapulate_Ui)
        return;

    ImGui::Begin("Manapulate Ui");


    Select_Object();
    Change_Object_Value();
    Add_Picking_Zone();
    Record_Two_Point();


    Delete_Object();


    ImGui::End();
}

void ImguiMgr::Parameter_Ui()
{
    if (false == m_bRender_Param_Ui)
        return;

    ImGui::Begin("Parameter Ui");


    Change_ParamDesc();




    ImGui::End();
}

void ImguiMgr::File_Dialog()
{
    ImGui::SeparatorText("File Dialog");

    if (ImGui::Button("Open File Dialog"))
        ImGuiFileDialog::Instance()->OpenDialog("ddsFileDlgKey", "dds File Dialog", ".dds", ".");

    ImGui::BulletText("You can only add dds");

    if (ImGuiFileDialog::Instance()->Display("ddsFileDlgKey"))
    {
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
            path = L"..\\" + path;

            size_t dotPos = FileName.rfind(L'.');
            if (dotPos != std::wstring::npos) {
                FileName.erase(dotPos);
            }

            RESOURCES.Load<Texture>(FileName, path);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void ImguiMgr::Select_Texture()
{
    ImGui::NewLine();
    ImGui::SeparatorText("Select Texture");

    auto textures = RESOURCES.Get_Resources(0);
    auto texture = textures[static_cast<_uint>(ResourceType::Texture)];

    _uint iSize = static_cast<_uint>(texture.size());
    _uint iIndex = 0;

    char inputBuffer[256] = {};
    if (ImGui::InputText("##Search Texture Name", inputBuffer, sizeof(inputBuffer))) {
        string str(inputBuffer);
        m_strSearchTexture = Utils::ToWString(str);
    }

    if (ImGui::BeginListBox("##Texture Name"))
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
                m_strSearchTexture = L"";
            }

            ++iIndex;
        }

        ImGui::EndListBox();
    }

    if (0 != m_strKeyTexture.length())
    {
       ImGui::Begin("Texture Ui");
       ImGui::Image((static_pointer_cast<Texture>(texture.find(m_strKeyTexture)->second)->Get_SRV().Get()), ImVec2(300, 300));
       ImGui::End();
    }

}

void ImguiMgr::Add_Texture()
{
   ImGui::NewLine();
   ImGui::SeparatorText("Selected Texture Name");

   string strName = Utils::ToString(m_strKeyTexture);
   if (ImGui::InputText("##Selected Texture Name", strName.data(), 256, ImGuiInputTextFlags_ReadOnly)) {
   }

   ImGui::Combo("##Texture Type Combo", &m_iTextureType, m_arrItems, IM_ARRAYSIZE(m_arrItems));

   if (ImGui::Button("Add Texture", ImVec2(80.f, 20.f)))
   {
      if (!ImGui::IsItemActive())
      {
         if (0 == m_strSelectObjName.length())
         {
            return;
         }

         auto pGameobject = CUR_SCENE->Get_GameObject(m_strSelectObjName);
         pGameobject->Get_MeshRenderer()->Get_Material()->Set_TextureMap(RESOURCES.Get<Texture>(m_strKeyTexture), static_cast<TextureMapType>(m_iTextureType));
      }
   }

   ImGui::SameLine();
   if (ImGui::Button("Add Submap", ImVec2(80.f, 20.f)))
   {
      if (!ImGui::IsItemActive())
      {
         if (0 == m_strSelectObjName.length())
         {
            return;
         }

         auto pGameobject = CUR_SCENE->Get_GameObject(m_strSelectObjName);

         for (_uint i = 0; i < MAX_SUB_SRV_COUNT; ++i)
         {
            if (nullptr == pGameobject->Get_MeshRenderer()->Get_Material()->Get_SubMap(i))
            {
               pGameobject->Get_MeshRenderer()->Get_Material()->Set_SubMap(i, RESOURCES.Get<Texture>(m_strKeyTexture));
            }
         }

      }
   }
}

void ImguiMgr::Decide_Transform_Value()
{
    ImGui::NewLine();
    ImGui::SeparatorText("Position");
    ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##PosX1", &m_vecPos.x, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##PosY1", &m_vecPos.y, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##PosZ1", &m_vecPos.z, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::NewLine();
    ImGui::SeparatorText("Scale");

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##ScaleX1", &m_vecScale.x, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##ScaleY1", &m_vecScale.y, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##ScaleZ1", &m_vecScale.z, 0.01f, 1.0f, "%.3f", eFlag);

    if (0.0001f > m_vecScale.x)
       m_vecScale.x = 0.0001f;
    if (0.0001f > m_vecScale.y)
       m_vecScale.y = 0.0001f;
    if (0.0001f > m_vecScale.z)
       m_vecScale.z = 0.0001f;

    ImGui::NewLine();
    ImGui::SetNextItemWidth(200);
}

void ImguiMgr::Create_Object()
{
   ImGui::NewLine();
   ImGui::SeparatorText("Input Object Name");

   char inputBuffer[256] = {};
   if (ImGui::InputText("##Object Name", inputBuffer, sizeof(inputBuffer))) {
      string str(inputBuffer);
      m_strName = Utils::ToWString(str);
   }

    if (ImGui::Button("Create Ui", ImVec2(80.f, 20.f)))
    {
        if (!ImGui::IsItemActive())
        {
            auto UiObject = make_shared<GameObject>();
            UiObject->GetOrAddTransform();
            UiObject->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(m_vecPos.x, m_vecPos.y, m_vecPos.z, 1));
            UiObject->GetOrAddTransform()->Scaled(_float3(m_vecScale.x, m_vecScale.y, m_vecScale.z));
            shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx"));

            auto mesh = RESOURCES.Get<Mesh>(L"Quad");
            renderer->Set_Mesh(mesh);

            auto material = make_shared<Material>();
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
    ImGui::SeparatorText("Select Object");

    _uint iSize = static_cast<_uint>(m_GameobjectName.size());
    _uint iIndex = 0;

    if (ImGui::BeginListBox("##Object Name"))
    {
        for (const auto& Name : m_GameobjectName) {
            string key = Utils::ToString(Name);

            const bool is_selected = (iIndex == iSize);

            if (ImGui::Selectable(key.c_str(), iIndex == m_iObjNameCursor))
            {
                m_iObjNameCursor = iIndex;
                m_strSelectObjName = Name;

                auto pGameobject = CUR_SCENE->Get_GameObject(m_strSelectObjName);
                m_iPass = pGameobject->Get_MeshRenderer()->Get_Pass();
                m_tagParamDesc = pGameobject->Get_MeshRenderer()->Get_RenderParamDesc();
                m_pSampleObj->GetOrAddTransform()->Set_WorldMat(pGameobject->GetOrAddTransform()->Get_WorldMatrix());
            }

            ++iIndex;
        }

        ImGui::EndListBox();
    }

    if (ImGui::TreeNode("Textures"))
    {
       if (0 != m_strSelectObjName.length())
       {
          auto pGameobject = CUR_SCENE->Get_GameObject(m_strSelectObjName);

          for (_uint i = 0; i < MAX_TEXTURE_MAP_COUONT; ++i)
          {
             ImGui::BulletText("%s : ", m_arrItems[i]);
             ImGui::SameLine();

             if (nullptr != pGameobject->Get_MeshRenderer()->Get_Material()->Get_TextureMap(static_cast<TextureMapType>(i)))
                ImGui::Text("%s", Utils::ToString(pGameobject->Get_MeshRenderer()->Get_Material()->Get_TextureMap(static_cast<TextureMapType>(i))->Get_Name()).c_str());
             else
                ImGui::Text("None");
          }
       }

       ImGui::TreePop();
    }

    if (ImGui::TreeNode("Submaps"))
    {
       if (0 != m_strSelectObjName.length())
       {
          auto pGameobject = CUR_SCENE->Get_GameObject(m_strSelectObjName);

          for (_uint i = 0; i < MAX_SUB_SRV_COUNT; ++i)
          {
             ImGui::BulletText("Submap%d : ", i);
             ImGui::SameLine();

             if (nullptr != pGameobject->Get_MeshRenderer()->Get_Material()->Get_SubMap(i))
                ImGui::Text("%s", Utils::ToString(pGameobject->Get_MeshRenderer()->Get_Material()->Get_SubMap(i)->Get_Name()).c_str());
             else
                ImGui::Text("None");
          }
       }
       ImGui::TreePop();
    }
}

void ImguiMgr::Delete_Object()
{
    ImGui::NewLine();
    ImGui::SeparatorText("Delete Object");

    if (ImGui::Button("Delete", ImVec2(80.f, 20.f)))
    {
        if (!ImGui::IsItemActive())
        {
            if (0 == m_strSelectObjName.length())
            {
                return;
            }

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

void ImguiMgr::Change_Object_Value()
{
    ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

    ImGui::NewLine();
    ImGui::SeparatorText("Control");

    ImGui::RadioButton("Trans", &m_iTypeZmoOper, 0);
    ImGui::RadioButton("Rotate", &m_iTypeZmoOper, 1);
    ImGui::RadioButton("Scale", &m_iTypeZmoOper, 2);

    if (0 == m_iTypeZmoOper)
        m_eZmoOper = ImGuizmo::OPERATION::TRANSLATE;
    else if (1 == m_iTypeZmoOper)
        m_eZmoOper = ImGuizmo::OPERATION::ROTATE;
    else if (2 == m_iTypeZmoOper)
        m_eZmoOper = ImGuizmo::OPERATION::SCALE;

    _float4x4 matWorld = {};
    memcpy(&matWorld, &m_pSampleObj->GetOrAddTransform()->Get_WorldMatrix(), sizeof(_float4x4));

    auto pUiCamera = CUR_SCENE->Get_UICamera()->Get_Camera();
    _float4x4 matView = pUiCamera->Get_ViewMat();
    _float4x4 matProj = pUiCamera->Get_ProjMat();

    ImGuizmo::Manipulate(
        matView.m[0],
        matProj.m[0],
        m_eZmoOper,
        ImGuizmo::WORLD,
        matWorld.m[0]
    );

    m_pSampleObj->GetOrAddTransform()->Set_WorldMat(matWorld);


    ImGui::NewLine();
    ImGui::SeparatorText("Position");

    _float4 vecPos = m_pSampleObj->GetOrAddTransform()->Get_State(Transform_State::POS);
    _float3 vecScale = m_pSampleObj->GetOrAddTransform()->Get_Scale();

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##PosX2", &vecPos.x, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##PosY2", &vecPos.y, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##PosZ2", &vecPos.z, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::NewLine();
    ImGui::SeparatorText("Scale");

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##ScaleX2", &vecScale.x, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##ScaleY2", &vecScale.y, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##ScaleZ2", &vecScale.z, 0.01f, 1.0f, "%.3f", eFlag);

    m_pSampleObj->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
    m_pSampleObj->GetOrAddTransform()->Scaled(vecScale);

    if (ImGui::Button("Change##Pos_Scale", ImVec2(80.f, 20.f)) || m_bIsChangeCheck)
    {
        if (!ImGui::IsItemActive() || m_bIsChangeCheck)
        {
            if (0 == m_strSelectObjName.length())
            {
                return;
            }

            auto pGameobject = CUR_SCENE->Get_GameObject(m_strSelectObjName);
            if (nullptr == pGameobject)
            {
                return;
            }

            auto transform = pGameobject->Get_Transform();

            transform->Set_WorldMat(m_pSampleObj->GetOrAddTransform()->Get_WorldMatrix());
        }
    }

    ImGui::SameLine();
    ImGui::Checkbox("Immediately", &m_bIsChangeCheck);
}

void ImguiMgr::Change_ParamDesc()
{
    ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

    ImGui::SeparatorText("Pass Index");
    ImGui::SetNextItemWidth(200);
    ImGui::InputInt("##int pass", &m_iPass, 1, 1, eFlag);

    ImGui::NewLine();
    ImGui::SeparatorText("Set Param");

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("##Set Param", tab_bar_flags))
    {
       if (ImGui::BeginTabItem("Param1"))
       {
          ImGui::BulletText("g_int_0");
          ImGui::SetNextItemWidth(200);
          ImGui::InputInt("##int param1", &m_iParam1, 1, 1, eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderInt("##int param1 slider", &m_iParam1, 0, 100, "Value = %d");

          ImGui::BulletText("g_int_1");
          ImGui::SetNextItemWidth(200);
          ImGui::InputInt("##int param2", &m_iParam2, 1, 1, eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderInt("##int param2 slider", &m_iParam2, 0, 100, "Value = %d");

          ImGui::BulletText("g_int_2");
          ImGui::SetNextItemWidth(200);
          ImGui::InputInt("##int param3", &m_iParam3, 1, 1, eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderInt("##int param3 slider", &m_iParam3, 0, 100, "Value = %d");

          ImGui::BulletText("g_int_3");
          ImGui::SetNextItemWidth(200);
          ImGui::InputInt("##int param4", &m_iParam4, 1, 1, eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderInt("##int param4 slider", &m_iParam4, 0, 100, "Value = %d");

          m_tagParamDesc.intParams[0] = m_iParam1;
          m_tagParamDesc.intParams[1] = m_iParam2;
          m_tagParamDesc.intParams[2] = m_iParam3;
          m_tagParamDesc.intParams[3] = m_iParam4;

          ImGui::BulletText("g_float_0");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##float param1", &m_tagParamDesc.floatParams[0], 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##float param1 slider", &m_tagParamDesc.floatParams[0], 0.0f, 100.0f, "Value = %.3f");
          
          ImGui::BulletText("g_float_1");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##float param2", &m_tagParamDesc.floatParams[1], 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##float param2 slider", &m_tagParamDesc.floatParams[1], 0.0f, 100.0f, "Value = %.3f");
          
          ImGui::BulletText("g_float_2");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##float param3", &m_tagParamDesc.floatParams[2], 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##float param3 slider", &m_tagParamDesc.floatParams[2], 0.0f, 100.0f, "Value = %.3f");
          
          ImGui::BulletText("g_float_3");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##float param4", &m_tagParamDesc.floatParams[3], 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##float param4 slider", &m_tagParamDesc.floatParams[3], 0.0f, 100.0f, "Value = %.3f");

          ImGui::EndTabItem();
       }
       if (ImGui::BeginTabItem("Param2"))
       {
          ImGui::BulletText("g_vec2_0 x");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec2_0 x", &m_tagParamDesc.vec2Params[0].x, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec2_0 x slider", &m_tagParamDesc.vec2Params[0].x, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec2_0 y");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec2_0 y", &m_tagParamDesc.vec2Params[0].y, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec2_0 y slider", &m_tagParamDesc.vec2Params[0].y, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec2_1 x");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec2_1 x", &m_tagParamDesc.vec2Params[1].x, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec2_1 x slider", &m_tagParamDesc.vec2Params[1].x, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec2_1 y");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec2_1 y", &m_tagParamDesc.vec2Params[1].y, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec2_1 y slider", &m_tagParamDesc.vec2Params[1].y, 0.0f, 100.0f, "Value = %.3f");
          
          ImGui::BulletText("g_vec2_2 x");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec2_2 x", &m_tagParamDesc.vec2Params[2].x, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec2_2 x slider", &m_tagParamDesc.vec2Params[2].x, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec2_2 y");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec2_2 y", &m_tagParamDesc.vec2Params[2].y, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec2_2 y slider", &m_tagParamDesc.vec2Params[2].y, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec2_3 x");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec2_3 x", &m_tagParamDesc.vec2Params[3].x, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec2_3 x slider", &m_tagParamDesc.vec2Params[3].x, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec2_3 y");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec2_3 y", &m_tagParamDesc.vec2Params[3].y, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec2_3 y slider", &m_tagParamDesc.vec2Params[3].y, 0.0f, 100.0f, "Value = %.3f");

          ImGui::EndTabItem();
       }
       if (ImGui::BeginTabItem("Param3"))
       {
          ImGui::BulletText("g_vec4_0 x");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_0 x", &m_tagParamDesc.vec4Params[0].x, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_0 x slider", &m_tagParamDesc.vec4Params[0].x, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_0 y");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_0 y", &m_tagParamDesc.vec4Params[0].y, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_0 y slider", &m_tagParamDesc.vec4Params[0].y, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_0 z");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_0 z", &m_tagParamDesc.vec4Params[0].z, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_0 z slider", &m_tagParamDesc.vec4Params[0].z, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_0 w");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_0 w", &m_tagParamDesc.vec4Params[0].w, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_0 w slider", &m_tagParamDesc.vec4Params[0].w, 0.0f, 100.0f, "Value = %.3f");
          
          ImGui::BulletText("g_vec4_1 x");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_1 x", &m_tagParamDesc.vec4Params[1].x, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_1 x slider", &m_tagParamDesc.vec4Params[1].x, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_1 y");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_1 y", &m_tagParamDesc.vec4Params[1].y, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_1 y slider", &m_tagParamDesc.vec4Params[1].y, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_1 z");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_1 z", &m_tagParamDesc.vec4Params[1].z, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_1 z slider", &m_tagParamDesc.vec4Params[1].z, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_1 w");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_1 w", &m_tagParamDesc.vec4Params[1].w, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_1 w slider", &m_tagParamDesc.vec4Params[1].w, 0.0f, 100.0f, "Value = %.3f");

          ImGui::EndTabItem();
       }
       if (ImGui::BeginTabItem("Param4"))
       {
          ImGui::BulletText("g_vec4_3 x");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_3 x", &m_tagParamDesc.vec4Params[2].x, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_3 x slider", &m_tagParamDesc.vec4Params[2].x, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_3 y");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_3 y", &m_tagParamDesc.vec4Params[2].y, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_3 y slider", &m_tagParamDesc.vec4Params[2].y, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_3 z");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_3 z", &m_tagParamDesc.vec4Params[2].z, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_3 z slider", &m_tagParamDesc.vec4Params[2].z, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_3 w");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_3 w", &m_tagParamDesc.vec4Params[2].w, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_3 w slider", &m_tagParamDesc.vec4Params[2].w, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_4 x");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_4 x", &m_tagParamDesc.vec4Params[3].x, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_4 x slider", &m_tagParamDesc.vec4Params[3].x, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_4 y");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_4 y", &m_tagParamDesc.vec4Params[3].y, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_4 y slider", &m_tagParamDesc.vec4Params[3].y, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_4 z");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_4 z", &m_tagParamDesc.vec4Params[3].z, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_4 z slider", &m_tagParamDesc.vec4Params[3].z, 0.0f, 100.0f, "Value = %.3f");

          ImGui::BulletText("g_vec4_4 w");
          ImGui::SetNextItemWidth(200);
          ImGui::InputFloat("##g_vec4_4 w", &m_tagParamDesc.vec4Params[3].w, 0.01f, 1.0f, "%.3f", eFlag);
          ImGui::SetNextItemWidth(200);
          ImGui::SliderFloat("##g_vec4_4 w slider", &m_tagParamDesc.vec4Params[3].w, 0.0f, 100.0f, "Value = %.3f");

          ImGui::EndTabItem();
       }
       ImGui::EndTabBar();
    }

    if (0 == m_strSelectObjName.length())
    {
        return;
    }

    auto pGameobject = CUR_SCENE->Get_GameObject(m_strSelectObjName);
    if (nullptr == pGameobject)
    {
        return;
    }

    auto pRenderer = pGameobject->Get_MeshRenderer();
    pRenderer->Set_Pass(m_iPass);
    pRenderer->Get_RenderParamDesc() = m_tagParamDesc;
}

void ImguiMgr::Add_Picking_Zone()
{
    ImGui::NewLine();
    ImGui::SeparatorText("Add Picking Zone");
    ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

    ImGui::RadioButton("Rect", &m_iPickTypeIndex, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Circle", &m_iPickTypeIndex, 1);

    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    if (ImGui::Button("Record", ImVec2(60.f, 20.f)))
    {
        if (!ImGui::IsItemActive())
        {
            m_bIsRecordStart = true;
            m_ptPos1 = {};
            m_ptPos2 = {};
            m_iRecordOrder = 0;
        }
    }

    ImGui::BulletText("Press the record button and follow the instructions");
    ImGui::BulletText("Rect : Click on the top left corner first. \n\t   Then, click on the bottom right corner.");
    ImGui::BulletText("Circle : Click on the center first. \n\t\t Then, click on a point anywhere in the circle");

    ImGui::NewLine();
    ImGui::SeparatorText("Mouse Position");

    POINT ptPos = INPUT.GetMousePosToPoint();
    ImGui::Text("Mouse pos: (%d, %d)", ptPos.x, ptPos.y);
    ImGui::Text("First Pick Pos: (%d, %d)", m_ptPos1.x, m_ptPos1.y);
    ImGui::Text("Second Pick Pos: (%d, %d)", m_ptPos2.x, m_ptPos2.y);


    if (ImGui::Button("Add Picking Zone##add base ui", ImVec2(150.f, 20.f)))
    {
        if (!ImGui::IsItemActive())
        {
            if (0 == m_strSelectObjName.length())
            {
                return;
            }

            auto pGameobject = CUR_SCENE->Get_GameObject(m_strSelectObjName);
            if (nullptr == pGameobject)
            {
                return;
            }

            if (nullptr != pGameobject->Get_Button())
            {
                return;
            }

            auto pTransform = pGameobject->GetOrAddTransform();
            auto BaseUi = make_shared<BaseUI>();

            switch (m_iPickTypeIndex)
            {
            case 0:
                BaseUi->Create(BaseUI::PICK_TYPE::RECT, m_ptPos1, m_ptPos2);
                break;
            case 1:
                BaseUi->Create(BaseUI::PICK_TYPE::CIRCLE, m_ptPos1, m_ptPos2);
                break;
            }

            pGameobject->Add_Component(BaseUi);

            m_ptPos1 = {};
            m_ptPos2 = {};
        }
    }


}

void ImguiMgr::Decide_FilePath()
{
    if (ImGui::Button("File Path Dialog##save load file"))
        ImGuiFileDialog::Instance()->OpenDialog("File Path", "Get File", ".dat", ".");

    if (ImGuiFileDialog::Instance()->Display("File Path"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            string strPath = ImGuiFileDialog::Instance()->GetFilePathName();
            string strFileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

            m_strSaveLoadPath = Utils::ToWString(strPath);
            m_strSaveFileName = Utils::ToWString(strFileName);

            size_t pos = m_strSaveLoadPath.find(L"Resources");
            if (pos != std::wstring::npos) {
                m_strSaveLoadPath.erase(0, pos);
            }
            m_strSaveLoadPath = L"..\\" + m_strSaveLoadPath;

            pos = m_strSaveLoadPath.rfind(m_strSaveFileName);
            if (pos != std::wstring::npos) {
                m_strSaveLoadPath = m_strSaveLoadPath.substr(0, pos);
            }

        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void ImguiMgr::Save_Ui_Desc()
{
    ImGui::NewLine();
    ImGui::SeparatorText("Save and Load");

    Decide_FilePath();

    ImGui::SameLine();
    if (ImGui::Button("Save##Save button", ImVec2(60.f, 20.f)))
    {
        if (!ImGui::IsItemActive())
        {
            if (0 == m_strSaveLoadPath.length() && 0 == m_strSaveFileName.length())
            {
                return;
            }

            shared_ptr<FileUtils> file = make_shared<FileUtils>();
            file->Open(m_strSaveLoadPath + m_strSaveFileName, FileMode::Write);

            // create ojbect count
            file->Write<_uint>(static_cast<_uint>(m_GameobjectName.size()));

            for (auto iter = m_GameobjectName.begin();
                iter != m_GameobjectName.end();
                ++iter)
            {
                auto pGameobject = CUR_SCENE->Get_GameObject(*iter);

                // object name
                file->Write<string>(Utils::ToString(*iter));

                // texture name
                file->Write<string>(Utils::ToString(pGameobject->Get_MeshRenderer()->Get_Material()->Get_TextureMap(TextureMapType::DIFFUSE)->Get_Name()));
                
                // submap name
                //file->Write<string>(Utils::ToString(pGameobject->Get_MeshRenderer()->Get_Material()->Get_TextureMap(TextureMapType::DIFFUSE)->Get_Name()));

                // worldmatrix
                file->Write<_float4x4>(pGameobject->GetOrAddTransform()->Get_WorldMatrix());

                // pass index
                file->Write<_uint>(pGameobject->Get_MeshRenderer()->Get_Pass());

                // renderer param
                file->Write<RenderParams>(pGameobject->Get_MeshRenderer()->Get_RenderParamDesc());

                // base ui type, value
                if (nullptr != pGameobject->Get_Button())
                {
                    file->Write<_bool>(true);
                    file->Write<BaseUI::BASEUIDESC>(pGameobject->Get_Button()->Get_Desc());
                }
                else
                {
                    file->Write<_bool>(false);
                }

            }
        }
    }

    Load_Ui_Desc();

    ImGui::BulletText("Data File Path");
    string strName = Utils::ToString(m_strSaveLoadPath);
    if (ImGui::InputText("##Save Load Path", strName.data(), 256, ImGuiInputTextFlags_ReadOnly)) {
    }

    strName = Utils::ToString(m_strSaveFileName);
    char inputBuffer[256] = {};
    strncpy_s(inputBuffer, strName.c_str(), sizeof(inputBuffer) - 1);

    ImGui::BulletText("Data File Name");
    if (ImGui::InputText("##Save File Name", inputBuffer, sizeof(inputBuffer))) {
        string str(inputBuffer);
        m_strSaveFileName = Utils::ToWString(str);
    }
}

void ImguiMgr::Load_Ui_Desc()
{
    ImGui::SameLine();

    if (ImGui::Button("Load##Load button", ImVec2(60.f, 20.f)))
    {
        if (!ImGui::IsItemActive())
        {
            if (0 == m_strSaveLoadPath.length() && 0 == m_strSaveFileName.length())
            {
                return;
            }

            shared_ptr<FileUtils> file = make_shared<FileUtils>();
            file->Open(m_strSaveLoadPath + m_strSaveFileName, FileMode::Read);

            _uint iSize = file->Read<_uint>();

            for (_uint i = 0; i < iSize; ++i)
            {
                auto UiObject = make_shared<GameObject>();

                wstring strObjectName = Utils::ToWString(file->Read<string>());
                UiObject->Set_Name(strObjectName);
                m_GameobjectName.push_back(strObjectName);

                shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>(RESOURCES.Get<Shader>(L"Shader_Mesh.fx"));
                auto mesh = RESOURCES.Get<Mesh>(L"Quad");
                renderer->Set_Mesh(mesh);
                auto material = make_shared<Material>();
                material->Set_TextureMap(RESOURCES.Get<Texture>(Utils::ToWString(file->Read<string>())), TextureMapType::DIFFUSE);
                //material->Set_TextureMap(RESOURCES.Get<Texture>(Utils::ToWString(file->Read<string>())), TextureMapType::DIFFUSE);

                UiObject->GetOrAddTransform()->Set_WorldMat(file->Read<_float4x4>());
                _uint iIndex = file->Read<_uint>();
                renderer->Set_Pass(iIndex);
                renderer->Get_RenderParamDesc() = file->Read<RenderParams>();
                renderer->Set_Material(material);
                UiObject->Add_Component(renderer);

                _bool bIsUseBaseUi = file->Read<_bool>();
                if (true == bIsUseBaseUi)
                {
                    auto BaseUi = make_shared<BaseUI>();
                    BaseUi->Get_Desc() = file->Read<BaseUI::BASEUIDESC>();
                    UiObject->Add_Component(BaseUi);
                }

                UiObject->Set_LayerIndex(Layer_UI);
                UiObject->Set_Instancing(false);
                CUR_SCENE->Add_GameObject(UiObject);
            }
        }
    }

}

void ImguiMgr::Record_Two_Point()
{
    if (false == m_bIsRecordStart)
    {
        return;
    }

    if (0 == m_iRecordOrder)
    {
        if (true == KEYTAP(KEY_TYPE::LBUTTON))
        {
            ++m_iRecordOrder;

            m_ptPos1 = INPUT.GetMousePosToPoint();
        }
    }
    else if (1 == m_iRecordOrder)
    {
        if (true == KEYTAP(KEY_TYPE::LBUTTON))
        {
            ++m_iRecordOrder;

            m_ptPos2 = INPUT.GetMousePosToPoint();
        }
    }

}
