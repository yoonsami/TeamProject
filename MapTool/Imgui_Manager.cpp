#include "pch.h"

#include "Imgui_Manager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImGuizmo.h"

#include "FileUtils.h"
#include "Utils.h"
#include "PickingMgr.h"
#include "Light.h"

#include "Model.h"
#include "ModelRenderer.h"

#include "Camera.h"
#include "MapObjectScript.h"

ImGuizmo::OPERATION m_eGuizmoType = { ImGuizmo::TRANSLATE };
namespace fs = std::filesystem;

ImGui_Manager::~ImGui_Manager()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGui_Manager::ImGui_SetUp()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

    ImGuizmo::Enable(true);
    ImGuizmo::SetRect(0.f, 0.f, g_iWinSizeX, g_iWinSizeY);

    Load_MapObjectBase();
}

void ImGui_Manager::ImGui_Tick()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    Frame_DirectionalLight();
    Frame_ObjectBase();
    Frame_Objects();
    
    Picking_Object();

    Show_Gizmo();
}

void ImGui_Manager::ImGui_Render()
{
    ImGui::Render();
   
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGui_Manager::Show_Gizmo()
{
    // 기즈모 출력
    ImGuizmo::BeginFrame();
    if (m_strObjectName.size() != 0)
    {
        // 기즈모를 위해 카메라의 뷰,투영을 가져옴.
        shared_ptr<Camera> CurCam = CUR_SCENE->Get_Camera(L"Default")->Get_Camera();
        _float4x4 matView = CurCam->Get_ViewMat();
        _float4x4 matProj = CurCam->Get_ProjMat();

        // 기즈모 상태 적용
        _float4x4 matGuizmo = m_pMapObjects[m_iObjects].get()->Get_Transform()->Get_WorldMatrix();

        ImGuizmo::Manipulate((float*)&matView,
            (float*)&matProj,
            m_eGuizmoType,
            ImGuizmo::LOCAL,
            (float*)&matGuizmo);

        m_pMapObjects[m_iObjects].get()->Get_Transform()->Set_WorldMat(matGuizmo);
    }
}

void ImGui_Manager::Frame_ObjectBase()
{
    ImGui::Begin("Frame_ObjectBase"); // 글자 맨윗줄

    ImGui::ListBox("##ObjectBase", &m_iObjectBaseIndex, m_strObjectBaseNameList.data(), (int)m_strObjectBaseNameList.size(), 10);

    ImGui::InputFloat("UVWeight", &m_fUVWeight);

    ImGui::InputFloat3("PickingPos", (_float*)&m_PickingPos);

    if (ImGui::Button("Create") || KEYTAP(KEY_TYPE::Z))
        if (FAILED(Create_MapObject()))
            MSG_BOX("Fail : Create_MapObject");

    ImGui::SameLine();
    ImGui::Text("Press Z");

    ImGui::End();
}

void ImGui_Manager::Frame_Objects()
{
    ImGui::Begin("Frame_Objects"); // 글자 맨윗줄

    ImGui::ListBox("##Objects", &m_iObjects, m_strObjectName.data(), (_int)m_strObjectName.size(), 10);

    if (m_strObjectName.size() > 0)
    {
        // 전체옵션 false로 초기화
        for (size_t i = 0; i < GizmoEND; i++)
        {
            m_bGizmoOp[i] = false;
            // 현재옵션이랑 같으면 true
            if (m_eGizmoOp == i)
                m_bGizmoOp[i] = true;
        }
        // 포지션, 회전, 스케일 선택
        if (ImGui::RadioButton("Tr", m_bGizmoOp[GizmoTR]))
            m_eGizmoOp = GizmoTR;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rt", m_bGizmoOp[GizmoRT]))
            m_eGizmoOp = GizmoRT;
        ImGui::SameLine();
        if (ImGui::RadioButton("Sc", m_bGizmoOp[GizmoSC]))
            m_eGizmoOp = GizmoSC;
        // 현재 기즈모 옵션 적용
        switch (m_eGizmoOp)
        {
        case GizmoTR:
            m_eGuizmoType = ImGuizmo::TRANSLATE;
            break;
        case GizmoRT:
            m_eGuizmoType = ImGuizmo::ROTATE;
            break;
        case GizmoSC:
            m_eGuizmoType = ImGuizmo::SCALE;
            break;
        default:
            break;
        }

        // 포지션, 회전, 스케일 조정 및 출력
        _float4x4 tempFloat4x4 = m_pMapObjects[m_iObjects].get()->Get_Transform()->Get_WorldMatrix();
        _float4x4* pTempObj = &tempFloat4x4;
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents((float*)pTempObj, matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation);
        ImGui::InputFloat3("Rt", matrixRotation);
        ImGui::InputFloat3("Sc", matrixScale);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, (float*)pTempObj);
        m_pMapObjects[m_iObjects]->Get_Transform()->Set_WorldMat(*(_float4x4*)pTempObj);

        if (ImGui::Button("Delete"))
            if (FAILED(Delete_MapObject()))
                MSG_BOX("Fail : Delete_MapObject");
    }

    // 세이브로드
    ImGui::NewLine();
    ImGui::InputText("##SaveFileName", m_szSaveFileName, sizeof(m_szSaveFileName));
    if (ImGui::Button("Save Object"))
        if (FAILED(Save_MapObject()))
            MSG_BOX("Fail : Save_MapObject");
        else
            MSG_BOX("Complete_Save");
    ImGui::SameLine();
    if (ImGui::Button("Load Object"))
        if (FAILED(Load_MapObject()))
            MSG_BOX("Fail : Load_MapObject");
        else
            MSG_BOX("Complete_Load");

    ImGui::End();
}

void ImGui_Manager::Frame_DirectionalLight()
{
    ImGui::Begin("Frame_Directionalight"); // 글자 맨윗줄

    // 포지션
    _float4& Pos = (_float4&)CUR_SCENE->Get_Light()->Get_Transform()->Get_WorldMatrix().m[3];
    ImGui::DragFloat3("Position", (_float*)&Pos);
    _float3& LookDir = CUR_SCENE->Get_Light()->Get_Transform()->Get_CurrentDir();
    ImGui::DragFloat3("LookDir", (_float*)&LookDir);
    CUR_SCENE->Get_Light()->Get_Transform()->Set_LookDir(LookDir);
    LightInfo& DirectionalLightInfo = CUR_SCENE->Get_Light()->Get_Light()->Get_LightInfo();
    _float4& Diffuse = DirectionalLightInfo.color.diffuse;
    ImGui::ColorEdit4("Diffuse", (_float*)&Diffuse);
    _float4& Ambient = DirectionalLightInfo.color.ambient;
    ImGui::ColorEdit4("Ambient", (_float*)&Ambient);
    _float4& Specular = DirectionalLightInfo.color.specular;
    ImGui::ColorEdit4("Specular", (_float*)&Specular);
    _float4& Emissive = DirectionalLightInfo.color.emissive;
    ImGui::ColorEdit4("Emissive", (_float*)&Emissive);

    ImGui::End();
}

void ImGui_Manager::Picking_Object()
{
    // 마우스 우클릭 시 메시피킹
    if (KEYTAP(KEY_TYPE::RBUTTON))
    {
        POINT MousePos;
        ::GetCursorPos(&MousePos);
        ::ScreenToClient(g_hWnd, &MousePos);
        _float2 ScreenPos{ (_float)MousePos.x, (_float)MousePos.y };

        shared_ptr<GameObject> PickObject = PickingMgr::GetInstance().Pick_Mesh(ScreenPos, CUR_SCENE->Get_Camera(L"Default")->Get_Camera(), CUR_SCENE->Get_Objects(), m_PickingPos);

        // 선택된 오브젝트 번호 바꾸기
        for (_int i = 0; i < m_pMapObjects.size(); ++i)
        {
            if (PickObject == m_pMapObjects[i])
                m_iObjects = i;
        }
    }
}

HRESULT ImGui_Manager::Load_MapObjectBase()
{
    wstring path = L"..\\Resources\\Models\\MapObject\\";

    for (auto& entry : fs::recursive_directory_iterator(path))
    {
        // 맵오브젝트 폴더내부의 폴더이름들을 순회하며 베이스오브젝트 리스트를 만듦.
        if (!entry.is_directory())
            continue;

        // 파일의 이름을 가져옴
        wstring fileName = entry.path().filename().wstring();
        WCHAR szTempName[MAX_PATH];
        lstrcpy(szTempName, fileName.c_str());

        // char 형식으로 변환
        shared_ptr<char[]> pChar = shared_ptr<char[]>(new char[MAX_PATH]);
        WideCharToMultiByte(CP_ACP, 0, szTempName, -1, pChar.get(), MAX_PATH, 0, 0);

        // 베이스오브젝트 이름을 포인터 벡터에 추가
        m_strObjectBaseNamePtr.push_back(pChar);
        // 베이스오브젝트 이름을 리스트UI에 추가
        m_strObjectBaseNameList.push_back(pChar.get());
    }
    return S_OK;
}

HRESULT ImGui_Manager::Create_MapObject()
{
    // 1. 현재 선택된 베이스오브젝트이름 가져오기
    wstring strCurObjBase = Utils::ToWString(m_strObjectBaseNameList[m_iObjectBaseIndex]);

    // 2. 게임오브젝트 동적할당하여 현재씬에 추가
    shared_ptr<GameObject> CreateObject = make_shared<GameObject>();
    CreateObject->Add_Component(make_shared<Transform>());
    CreateObject->Get_Transform()->Set_State(Transform_State::POS, _float4(m_PickingPos.x, m_PickingPos.y, m_PickingPos.z, 1.f));
    {
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
        shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
        {
            // 맵오브젝트 패스변경 + UV가중치적용
            shared_ptr<MapObjectScript> MapObjSc = make_shared<MapObjectScript>(renderer, ModelRenderer::PASS_MAPOBJECT, m_fUVWeight);
            CreateObject->Add_Component(MapObjSc);
        }
        {
            // 베이스 오브젝트의 이름을 사용하여 오브젝트 생성
            shared_ptr<Model> model = RESOURCES.Get<Model>(strCurObjBase);
            renderer->Set_Model(model);
        }
        //모델렌더러 컴포넌트
        CreateObject->Add_Component(renderer);
    }
    CUR_SCENE->Add_GameObject(CreateObject);
    m_pMapObjects.push_back(CreateObject);

    // 3. 현재 설치된 오브젝트 목록에 추가
    m_strObjectName.push_back(m_strObjectBaseNameList[m_iObjectBaseIndex]);

    // 4. 새로 생성된 오브젝트를 타겟팅(기즈모)하기
    m_iObjects = (_int)(m_pMapObjects.size() - 1);

    return S_OK;
}

HRESULT ImGui_Manager::Delete_MapObject()
{
    // 1. 현재 선택된 오브젝트를 씬에서 삭제
    CUR_SCENE->Remove_GameObject(m_pMapObjects[m_iObjects]);
    // 2. 현재 선택된 오브젝트를 벡터에서 삭제
    {
        auto iter = m_pMapObjects.begin();
        for (_int i = 0; i < m_iObjects; ++i)
            ++iter;
        // 리스트벡터에서 해당내용삭제
        if (m_iObjects > 0)
            iter = m_pMapObjects.erase(iter);
        else
            m_pMapObjects.erase(m_pMapObjects.begin());
    }
    // 3. 현재 배치된 오브젝트 List 이름 제거
    {
        auto iter = m_strObjectName.begin();
        for (_int i = 0; i < m_iObjects; ++i)
            ++iter;
        // 리스트벡터에서 해당내용삭제
        if (m_iObjects > 0)
        {
            iter = m_strObjectName.erase(iter);
            // 리스트의 지정위치 변경
            --m_iObjects;
        }
        else
        {
            m_strObjectName.erase(m_strObjectName.begin());
        }
    }

    return S_OK;
}

HRESULT ImGui_Manager::Save_MapObject()
{
    // 세이브 파일 이름으로 저장하기
    string strFileName = m_szSaveFileName;
    string strFilePath = "..\\Resources\\Data\\";
    strFilePath += strFileName + ".dat";

    shared_ptr<FileUtils> file = make_shared<FileUtils>();
    file->Open(Utils::ToWString(strFilePath), FileMode::Write);

    // 1. 오브젝트 개수 저장
    file->Write<_int>((_int)m_pMapObjects.size());
    
    // 2. 오브젝트의 이름과 UVWeight, 월드매트릭스 저장
    for (_uint i = 0; i < m_pMapObjects.size(); ++i)
    {
        // 이름저장
        string ObjectName(m_strObjectName[i]);
        file->Write<string>(ObjectName);
        // UVWeight저장
        file->Write<_float>(m_pMapObjects[i]->Get_Script<MapObjectScript>()->Get_UVWeight());
        // 월드행렬 저장
        file->Write<_float4x4>(m_pMapObjects[i].get()->Get_Transform()->Get_WorldMatrix());
    }

    return S_OK;
}

HRESULT ImGui_Manager::Load_MapObject()
{
    // 현재갖고있는 오브젝트개수
    _uint iSize = (_int)m_pMapObjects.size();
    // 1. 기존의 모든오브젝트 삭제 및 Clear
    for (_uint i = 0; i< iSize; ++i)
    {
        // 모든 오브젝트를 씬에서 삭제 ( 맨뒤부터 )
        CUR_SCENE->Remove_GameObject(m_pMapObjects.back());
        m_pMapObjects.pop_back();
        m_strObjectName.pop_back();
    }
    m_pMapObjects.clear();
    m_strObjectName.clear();
    m_strObjectNamePtr.clear();
    m_iObjects = 0;

    // 세이브 파일 이름으로 저장하기
    string strFileName = m_szSaveFileName;
    string strFilePath = "..\\Resources\\Data\\";
    strFilePath += strFileName + ".dat";
    // 2. 오브젝트 개수 불러오기
    shared_ptr<FileUtils> file = make_shared<FileUtils>();
    file->Open(Utils::ToWString(strFilePath), FileMode::Read);
    _int iNumObjects = file->Read<_int>();
    
    for (_int i = 0; i < iNumObjects; ++i)
    {
        // 3. 이름과 매트릭스를 읽어오기
        wstring strObjectName = Utils::ToWString(file->Read<string>());
        _float fUVWeight = file->Read<_float>();
        _float4x4 matWorld = file->Read<_float4x4>();

        // 4. 가져온 정보를 바탕으로 맵오브젝트를 생성하여 현재씬에 추가
        shared_ptr<GameObject> CreateObject = make_shared<GameObject>();
        CreateObject->Add_Component(make_shared<Transform>());
        CreateObject->Get_Transform()->Set_WorldMat(matWorld);
        {
            shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
            shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
            {
                // 맵오브젝트 패스변경 + UV가중치적용
                shared_ptr<MapObjectScript> MapObjSc = make_shared<MapObjectScript>(renderer, ModelRenderer::PASS_MAPOBJECT, fUVWeight);
                CreateObject->Add_Component(MapObjSc);
            }
            {
                // 베이스 오브젝트의 이름을 사용하여 오브젝트 생성
                shared_ptr<Model> model = RESOURCES.Get<Model>(strObjectName);
                renderer->Set_Model(model);
            }
            //애니메이터 컴포넌트
            CreateObject->Add_Component(renderer);
        }
        // 4-1 현재씬 + 오브젝트벡터에 추가
        CUR_SCENE->Add_GameObject(CreateObject);
        m_pMapObjects.push_back(CreateObject);
        // 4-2. 현재 설치된 오브젝트 이름 UI에 추가
        WCHAR szTempName[MAX_PATH];
        lstrcpy(szTempName, strObjectName.c_str());
        // char 형식으로 변환
        shared_ptr<char[]> pChar = shared_ptr<char[]>(new char[MAX_PATH]);
        WideCharToMultiByte(CP_ACP, 0, szTempName, -1, pChar.get(), MAX_PATH, 0, 0);
        m_strObjectNamePtr.push_back(pChar);
        m_strObjectName.push_back(pChar.get());
    }

    return S_OK;
}