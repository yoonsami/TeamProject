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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

    ImGuizmo::Enable(true);
    ImGuizmo::SetRect(0.f, 0.f, g_iWinSizeX, g_iWinSizeY);

    m_pSampleObj = make_shared<GameObject>();
    m_pSampleObj->GetOrAddTransform();

    return S_OK;
}

void ImguiMgr::Tick()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    //ImGui::ShowDemoWindow(); // Show demo window! :)


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

    Select_Texture();
    Decide_Transform_Value();
    Create_Object();

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
    // open Dialog Simple
    ImGui::SeparatorText("File Dialog");

    if (ImGui::Button("Open File Dialog"))
        ImGuiFileDialog::Instance()->OpenDialog("ddsFileDlgKey", "dds File Dialog", ".dds", ".");

    ImGui::BulletText("You can only add dds");

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
            path = L"..\\" + path;

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

    ImGui::SameLine();
    if (0 != m_strKeyTexture.length())
    {
       ImGui::Begin("Texture Ui");
       ImGui::Image((static_pointer_cast<Texture>(texture.find(m_strKeyTexture)->second)->Get_SRV().Get()), ImVec2(300, 300));
       ImGui::End();
    }

}

void ImguiMgr::Decide_Transform_Value()
{
    ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

    ImGui::NewLine();
    //ImGui::Text("Texture", IMGUI_VERSION, IMGUI_VERSION_NUM);
    ImGui::SeparatorText("Selected Texture Name");

    string strName = Utils::ToString(m_strKeyTexture);
    //ImGui::SetNextItemWidth(200);
    if (ImGui::InputText("##Selected Texture Name", strName.data(), 256, ImGuiInputTextFlags_ReadOnly)) {
    }

    ImGui::NewLine();
    ImGui::SeparatorText("Position");


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
    //ImGui::Text("Scale", IMGUI_VERSION, IMGUI_VERSION_NUM);
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

    ImGui::NewLine();
    ImGui::SetNextItemWidth(200);

    ImGui::NewLine();
    ImGui::SeparatorText("Input Object Name");

    char inputBuffer[256] = {};
    if (ImGui::InputText("##Object Name", inputBuffer, sizeof(inputBuffer))) {
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
            UiObject->GetOrAddTransform()->Scaled(_float3(m_vecScale.x, m_vecScale.y, m_vecScale.z));
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

                m_pSampleObj->GetOrAddTransform()->Set_WorldMat(CUR_SCENE->Get_GameObject(m_strSelectObjName)->GetOrAddTransform()->Get_WorldMatrix());
            }

            ++iIndex;
        }

        ImGui::EndListBox();
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
    //ImGui::Text("Scale", IMGUI_VERSION, IMGUI_VERSION_NUM);
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
    /*

    g_int_0
       0 : 기본
       1 : UV 비율로 색 차이
             -> g_float_0 사용
       2 : 원형 100이상일때 기본동작, 100 미만일 때
             -> g_float_0 사용
       3 : 100 미만 일때
             -> g_float_0 사용
       4 : g_float_0 / 100 값이 uv.x 작을때 렌더
             -> g_float_0 사용

    */

    // renderer->Get_RenderParamDesc().SetVec4(0, _float4(1));

    ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

    ImGui::NewLine();
    ImGui::SeparatorText("Change Param");
    ImGui::SetNextItemWidth(200);
    ImGui::InputInt("##int pass_o", &m_iPass_0, 1, 1, eFlag);

    ImGui::SetNextItemWidth(200);
    ImGui::InputFloat("##float g_float_o", &m_float_0, 0.01f, 1.0f, "%.3f", eFlag);

    ImGui::SetNextItemWidth(200);
    ImGui::SliderFloat("##float slider", &m_float_0, 0.0f, 100.0f, "Value = %.3f");

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
    pRenderer->Get_RenderParamDesc().SetInt(0, m_iPass_0);
    pRenderer->Get_RenderParamDesc().SetFloat(0, m_float_0);
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

    // 기록 버튼 누르면 다음 두번의 클릭은 피킹존 위치 생성
    // begintabitem

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

    // display
    if (ImGuiFileDialog::Instance()->Display("File Path"))
    {
        // action if OK
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

        // close
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

                // worldmatrix
                file->Write<_float4x4>(pGameobject->GetOrAddTransform()->Get_WorldMatrix());

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

                UiObject->GetOrAddTransform()->Set_WorldMat(file->Read<_float4x4>());
                renderer->Get_RenderParamDesc() = file->Read<RenderParams>();
                renderer->Set_Material(material);
                renderer->Set_Pass(MeshRenderer::PASS_INFO::Default_UI);
                UiObject->Add_Component(renderer);

                _bool bIsUseBaseUi = file->Read<_bool>();
                if (true == bIsUseBaseUi)
                {
                    auto BaseUi = make_shared<BaseUI>();
                    BaseUi->Get_Desc() = file->Read<BaseUI::BASEUIDESC>();
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
