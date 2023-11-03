#pragma once
#include "pch.h"

#include "Imgui_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Component_Manager.h"
#include "Monster.h"
#include "PipeLine.h"
#include "Effect.h"

#include "Cell.h"

IMPLEMENT_SINGLETON(CImgui_Manager)

HRESULT CImgui_Manager::initialize(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContextconst, GRAPHICDESC& _tagDesc)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_tagDesc.hWnd);
    ImGui_ImplDX11_Init(_pDevice, _pContextconst);

    ImGuizmo::Enable(true);
    ImGuizmo::SetRect(0.f, 0.f, g_iWinSizeX, g_iWinSizeY);

    m_pDevice = _pDevice;
    Safe_AddRef(m_pDevice);

    m_pContext = _pContextconst;
    Safe_AddRef(m_pContext);

    m_pCellShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
    if (nullptr == m_pCellShader)
    {
        MSG_RETURN(E_FAIL, "Failed to Initialize_Prototype: CNavigation");
    }

    for (_uint i = 0; i < 3; ++i)
    {
        m_vecNeviPos.push_back(_float3(0.f, 0.f, 0.f));
    }

    return S_OK;
}

void CImgui_Manager::Tick(_float _fTimeDelta)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    //ImGui::ShowDemoWindow(); // Show demo window! :)
    

    _Key_Input();
    _Start_Map_Tool();
    _Manipulate_Model();

    _Start_Effect_Tool();

}

HRESULT CImgui_Manager::Render()
{
    _Cell_Render();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

void CImgui_Manager::_Cell_Render()
{
    if (0 == m_listCell.size())
    {
        return;
    }

    GAMEINSTANCE;

    m_pCellShader->Bind_Matrix("g_ViewMatrix", pGameInstance->Get_Transform_Float4x4(TRANSFORMSTATE::VIEW));
    m_pCellShader->Bind_Matrix("g_ProjMatrix", pGameInstance->Get_Transform_Float4x4(TRANSFORMSTATE::PROJ));

    if (FAILED(m_pCellShader->Bind_RawValue("g_vColor", &m_vecColor, sizeof(_float4))))
    {
        MSG_VOID_RETURN("Failed to Render: CNavigation");
    }

    if (nullptr == m_pWorldMatrix)
    {
        CGameObject* pTerrain = pGameInstance->Find_GameObject(LEVEL::TOOL, LAYER_TERRAIN, GO_TERRAIN);
        if (nullptr == pTerrain)
        {
            MSG_VOID_RETURN("Failed to Add_GameObject: CImgui_Manager::_Create_Terrain");
        }

        CTransform* pTrans = reinterpret_cast<CTransform*>(pTerrain->Find_Component(CP_TRANSFORM));

        m_pWorldMatrix = pTrans->Get_WorldMatrixPtr();
    }

    m_pCellShader->Bind_Matrix("g_WorldMatrix", *m_pWorldMatrix);

    m_pCellShader->Begin(0);

    for (auto& Cell : m_listCell)
    {
        Cell->Render();
    }
}

void CImgui_Manager::_Start_Map_Tool()
{
    if (false == m_bMapToolRender)
    {
        return;
    }


    ImGui::Begin("Map_Tool");

    _Create_Terrain();
    _Create_Model();
    _Create_Monster();
    _Create_Nevi();
    _Create_Npc();
    _Manipulate_Value_Model();
        

    ImGui::End();
}

void CImgui_Manager::_Start_Effect_Tool()
{
    if (false == m_bEffectToolRender)
    {
        return;
    }

    ImGui::Begin("Effect_Tool");


    _Create_Effect();
    _Manipulate_Particle();
    _Manipulate_Effect();



    ImGui::End();
}

void CImgui_Manager::_Key_Input()
{
    GAMEINSTANCE;

    if (pGameInstance->Key_Down(VK_F1))
    {
        m_bMapToolRender = !m_bMapToolRender;
    }
    if (pGameInstance->Key_Down(VK_F2))
    {
        m_bEffectToolRender = !m_bEffectToolRender;
    }


    if (pGameInstance->Key_Hold(VK_CONTROL) && pGameInstance->Key_Down('1'))
    {
        m_eZmoOper = ImGuizmo::OPERATION::TRANSLATE;
    }
    if (pGameInstance->Key_Hold(VK_CONTROL) && pGameInstance->Key_Down('2'))
    {
        m_eZmoOper = ImGuizmo::OPERATION::ROTATE;
    }
    if (pGameInstance->Key_Hold(VK_CONTROL) && pGameInstance->Key_Down('3'))
    {
        m_eZmoOper = ImGuizmo::OPERATION::SCALE;
    }
    if (pGameInstance->Key_Hold(VK_CONTROL) && pGameInstance->Key_Down('4'))
    {
        m_bManipulate_On = !m_bManipulate_On;
    }

    if (pGameInstance->Key_Hold('T') && pGameInstance->Key_Down(VK_LBUTTON))
    {
        _Picking_Terrain();
    }

    if (pGameInstance->Key_Hold('M') && pGameInstance->Key_Down(VK_LBUTTON))
    {
        _Picking_Model();
    }

    if (pGameInstance->Key_Hold('N') && pGameInstance->Key_Down(VK_LBUTTON))
    {
        _Picking_Nevi();
    }

    if (pGameInstance->Key_Hold('C') && pGameInstance->Key_Down(VK_LBUTTON))
    {
        _Picking_Cell();
    }

    if (pGameInstance->Key_Hold(VK_SHIFT) && pGameInstance->Key_Down('S'))
    {
        _Save_Map();
    }
    if (pGameInstance->Key_Hold(VK_SHIFT) && pGameInstance->Key_Down('L'))
    {
        _Load_Map();
    }
    if (pGameInstance->Key_Hold('S') && pGameInstance->Key_Down('N'))
    {
        _Save_Nevi();
    }
    if (pGameInstance->Key_Hold('L') && pGameInstance->Key_Down('N'))
    {
        _Load_Nevi();
    }

    if (pGameInstance->Key_Hold('N') && pGameInstance->Key_Down(VK_DELETE))
    {
        _Delete_Nevi();
    }
    else if (nullptr != m_pPickingModel && pGameInstance->Key_Hold(VK_DELETE))
    {
        _Delete_Model();
    }




    //_Move_All_Object
    if (pGameInstance->Key_Hold('O') && pGameInstance->Key_Down(VK_LEFT))
    {
        _Move_All_Object(true, false, false);
    }
    if (pGameInstance->Key_Hold('O') && pGameInstance->Key_Down(VK_RIGHT))
    {
        _Move_All_Object(true, false, true);
    }
    if (pGameInstance->Key_Hold('O') && pGameInstance->Key_Down(VK_UP))
    {
        _Move_All_Object(false, true, true);
    }
    if (pGameInstance->Key_Hold('O') && pGameInstance->Key_Down(VK_DOWN))
    {
        _Move_All_Object(false, true, false);
    }
}

void CImgui_Manager::_Draw_Grid()
{
    GAMEINSTANCE;


    // DrawGrid(&matView._11, &matProj._11, &matWorld._11, 10.f);

}

void CImgui_Manager::_Create_Terrain()
{    
    if (ImGui::CollapsingHeader("Terrain"))
    {
        ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

        ImGui::SetNextItemWidth(50);
        ImGui::InputInt("Width", &m_iNumVerticesX, 0, 0, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        ImGui::InputInt("Height", &m_iNumVerticesZ, 0, 0, eFlag);

        ImGui::SameLine();
        if (ImGui::Button("Create Terrain", ImVec2(50.f, 20.f)))
        {
            if (!ImGui::IsItemActive())
            {
                if (0 < m_iNumVerticesX &&
                    0 < m_iNumVerticesZ)
                {
                    TERRAINDESC tagTerrainDesc;
                    tagTerrainDesc.pPath = nullptr;

                    tagTerrainDesc.iX = m_iNumVerticesX;
                    tagTerrainDesc.iZ = m_iNumVerticesZ;

                    GAMEINSTANCE;
                    CULLEVEL;

                    if (FAILED(pGameInstance->Add_GameObject(PG_TERRAIN, LEVEL::TOOL, LAYER_TERRAIN, GO_TERRAIN, &tagTerrainDesc)))
                    {
                        MSG_VOID_RETURN("Failed to Add_GameObject: CImgui_Manager::_Create_Terrain");
                    }
                }
            }           
        }

        ImGui::SameLine();
        if (ImGui::Button("Set Player Pos", ImVec2(100.f, 20.f)))
        {
            if (!ImGui::IsItemActive())
            {
                m_vecPlayerPos = m_vecPickingPos;
            }
        }
    }
}

void CImgui_Manager::_Picking_Terrain()
{
    GAMEINSTANCE;

    // find all terrain, and check collision
    CGameObject* pTerrain = pGameInstance->Find_GameObject(LEVEL::TOOL, LAYER_TERRAIN, GO_TERRAIN);
    if (nullptr == pTerrain)
    {
        MSG_VOID_RETURN("Failed to Add_GameObject: CImgui_Manager::_Create_Terrain");
    }

    CVIBuffer* pBuffer = reinterpret_cast<CVIBuffer*>(pTerrain->Find_Component(CP_VIBUFFER_TERRAIN));
    CTransform* pTrans = reinterpret_cast<CTransform*>(pTerrain->Find_Component(CP_TRANSFORM));
    if (nullptr == pBuffer ||
        nullptr == pTrans)
    {
        return;
    }

    _float3 vecPickingPos = {};
    if (true == pBuffer->Intersect_Polygons(pTrans->Get_WorldMatrix(), &vecPickingPos))
    {
        m_vecPickingPos = vecPickingPos;
    }
}

void CImgui_Manager::_Picking_Model()
{
    GAMEINSTANCE;

    _uint iIndex = _uint(m_listObject.size());
    if (0 == iIndex)
    {
        return;
    }

    for (auto& pObject : m_listObject)
    {
        CModel* pPlayerModel = reinterpret_cast<CModel*>(pObject->Find_Component(CP_MODEL));
        CTransform* pTrans = reinterpret_cast<CTransform*>(pObject->Find_Component(CP_TRANSFORM));
        if (nullptr == pPlayerModel ||
            nullptr == pTrans)
        {
            continue;
        }

        _float3 vecPickingPos = {};
        if (true == pPlayerModel->Intersect_Polygons(pTrans->Get_WorldMatrix(), &vecPickingPos))
        {
            m_pPickingModel = pObject;
            m_vecPickingPos = vecPickingPos;
            m_pControlModel = pTrans;

            break;
        }
        else
        {
            m_pControlModel = nullptr;
            m_pPickingModel = nullptr;
            continue;
        }
    }


    

}

void CImgui_Manager::_Picking_Nevi()
{
    GAMEINSTANCE;

    // find all terrain, and check collision
    CGameObject* pTerrain = pGameInstance->Find_GameObject(LEVEL::TOOL, LAYER_TERRAIN, GO_TERRAIN);
    if (nullptr == pTerrain)
    {
        MSG_VOID_RETURN("Failed to Add_GameObject: CImgui_Manager::_Create_Terrain");
    }

    CVIBuffer* pBuffer = reinterpret_cast<CVIBuffer*>(pTerrain->Find_Component(CP_VIBUFFER_TERRAIN));
    CTransform* pTrans = reinterpret_cast<CTransform*>(pTerrain->Find_Component(CP_TRANSFORM));
    if (nullptr == pBuffer ||
        nullptr == pTrans)
    {
        return;
    }

    _float3 vecPickingPos = {};
    if (true == pBuffer->Intersect_Polygons(pTrans->Get_WorldMatrix(), &vecPickingPos))
    {
        vecPickingPos = _Check_Around(vecPickingPos);

        if (0 == m_iCheckCount)
        {

        }
        else if (1 == m_iCheckCount)
        {
            m_vecNeviPos[0] = vecPickingPos;
            ++m_iCheckCount;
        }
        else if (2 == m_iCheckCount)
        {
            m_vecNeviPos[1] = vecPickingPos;
            ++m_iCheckCount;
        }
        else if (3 == m_iCheckCount)
        {
            m_vecNeviPos[2] = vecPickingPos;
            m_iCheckCount = 0;
        }
    }
}

void CImgui_Manager::_Picking_Cell()
{
    
}

_float3 CImgui_Manager::_Check_Around(_float3 _vecPickingPos)
{
    _float3 vecPointPos = {};
    _vector vecPos = {};
    _vector vecPickingPos = XMVectorSet(_vecPickingPos.x, _vecPickingPos.y, _vecPickingPos.z, 1.f);

    for (auto& Cell : m_listCell)
    {
        vecPointPos = Cell->Get_Point(CCell::POINT::POINT_A);
        vecPos = XMVectorSet(vecPointPos.x, vecPointPos.y, vecPointPos.z, 1.f);

        if (true == _Calculate_Distance(vecPickingPos, vecPos))
        {
            return vecPointPos;
        }

        vecPointPos = Cell->Get_Point(CCell::POINT::POINT_B);
        vecPos = XMVectorSet(vecPointPos.x, vecPointPos.y, vecPointPos.z, 1.f);

        if (true == _Calculate_Distance(vecPickingPos, vecPos))
        {
            return vecPointPos;
        }

        vecPointPos = Cell->Get_Point(CCell::POINT::POINT_C);
        vecPos = XMVectorSet(vecPointPos.x, vecPointPos.y, vecPointPos.z, 1.f);

        if (true == _Calculate_Distance(vecPickingPos, vecPos))
        {
            return vecPointPos;
        }
    }

    return _vecPickingPos;
}

_bool CImgui_Manager::_Calculate_Distance(_fvector _v1, _fvector _v2)
{
    _bool   bValue = false;

    _vector vDis = _v1 - _v2;

    _float fDis = XMVectorGetX(XMVector3Length(vDis));

    if (1.f > fDis)
    {
        bValue = true;
    }

    return bValue;
}

void CImgui_Manager::_Manipulate_Model()
{
    if (nullptr == m_pControlModel || false == m_bManipulate_On)
    {
        return;
    }
    
    GAMEINSTANCE;

    _float4x4 matWorld;
    memcpy(&matWorld, &m_pControlModel->Get_WorldMatrix(), sizeof(_float4x4));

    ImGuizmo::Manipulate(
        pGameInstance->Get_Transform_Float4x4(TRANSFORMSTATE::VIEW).m[0],
        pGameInstance->Get_Transform_Float4x4(TRANSFORMSTATE::PROJ).m[0],
        m_eZmoOper,
        ImGuizmo::WORLD,
        matWorld.m[0]
    );

    m_pControlModel->Set_WorldMatrix(matWorld);
}

void CImgui_Manager::_Manipulate_Value_Model()
{
    if (ImGui::CollapsingHeader("Manipulate_Model"))
    {
        ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("PosX", &m_vecInputPos.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("PosY", &m_vecInputPos.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("PosZ", &m_vecInputPos.z, 0, 0, nullptr, eFlag);

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("ScaleX", &m_vecInputScale.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("ScaleY", &m_vecInputScale.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("ScaleZ", &m_vecInputScale.z, 0, 0, nullptr, eFlag);

        ImGui::SameLine();

        if (ImGui::Button("Get Ori Pos", ImVec2(50.f, 20.f)) && nullptr != m_pControlModel)
        {
            if (!ImGui::IsItemActive())
            {
                _vector vPos = m_pControlModel->Get_State(STATE::POSITION);
                XMStoreFloat3(&m_vecInputPos, vPos);
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Set", ImVec2(50.f, 20.f)) && nullptr != m_pControlModel)
        {
            if (!ImGui::IsItemActive())
            {
                m_pControlModel->Set_Scale(m_vecInputScale);

                _vector vPos = XMVectorSet(m_vecInputPos.x, m_vecInputPos.y, m_vecInputPos.z, 1.f);
                m_pControlModel->Set_State(STATE::POSITION, vPos);
            }
        }
    }
}

_bool _Compare_PosX(const _float3& _a, const _float3& _b)
{
    if (_a.x > _b.x) return true;
    if (_a.x < _b.x) return false;

    return false;
}

_bool _Compare_PosZ(const _float3& _a, const _float3& _b)
{
    if (_a.z > _b.z) return true;
    if (_a.z < _b.z) return false;

    return false;
}

void CImgui_Manager::_Decide_Order()
{
    sort(m_vecNeviPos.begin(), m_vecNeviPos.end(), _Compare_PosZ);

    _vector Pos0 = XMLoadFloat3(&m_vecNeviPos[0]);
    _vector Pos1 = XMLoadFloat3(&m_vecNeviPos[1]);
    _vector Pos2 = XMLoadFloat3(&m_vecNeviPos[2]);

    _vector Dir1 = XMVector3Normalize(Pos1 - Pos0);
    _vector Dir2 = XMVector3Normalize(Pos2 - Pos0);

    _float fY = XMVectorGetY(XMVector3Cross(Dir1, Dir2));

    if (0.f >= fY)
    {
        _float3 vecTemp = m_vecNeviPos[1];
        m_vecNeviPos[1] = m_vecNeviPos[2];
        m_vecNeviPos[2] = vecTemp;
    }
}

void CImgui_Manager::_Move_All_Object(_bool _bIsX, _bool _bIsZ, _bool _bIsPlus)
{
    _vector vMove = {};

    if (true == _bIsX)
    {
        if (true == _bIsPlus)
        {
            vMove = XMVectorSet(1.f, 0.f, 0.f, 0.f);
        }
        else
        {
            vMove = XMVectorSet(-1.f, 0.f, 0.f, 0.f);
        }
    }
    if (true == _bIsZ)
    {
        if (true == _bIsPlus)
        {
            vMove = XMVectorSet(0.f, 0.f, 1.f, 0.f);
        }
        else
        {
            vMove = XMVectorSet(0.f, 0.f, -1.f, 0.f);
        }
    }

    for (auto& pObject : m_listObject)
    {
        CTransform* pTransform = static_cast<CTransform*>(pObject->Find_Component(CP_TRANSFORM));
        _vector vPos = pTransform->Get_State(STATE::POSITION);

        vPos += vMove;

        pTransform->Set_State(STATE::POSITION, vPos);
    }
}

void CImgui_Manager::_Save_Map()
{
    ofstream saveFile("../Bin/Resources/MapFile/2.Bin", ios::out | ios::trunc | ios::binary);
    _bool bCheck = saveFile.is_open();
    /*if (false == bCheck)
    {
        return;
    }*/

    saveFile.write(reinterpret_cast<const char*>(&m_vecPlayerPos), sizeof(_float3));

    saveFile.write(reinterpret_cast<const char*>(&m_iNumVerticesX), sizeof(_uint));
    saveFile.write(reinterpret_cast<const char*>(&m_iNumVerticesZ), sizeof(_uint));

    _uint iCount = _uint(m_listObject.size());
    saveFile.write(reinterpret_cast<const char*>(&iCount), sizeof(_uint));
    if (0 == iCount)
    {
        return;
    }
    for (auto& pObject : m_listObject)
    {
        MAPDATA mapData = {};

        mapData.iObjectID = pObject->Get_ObjectID();
        mapData.iModelID = pObject->Get_ModelID();

        CTransform* pTrans = reinterpret_cast<CTransform*>(pObject->Find_Component(CP_TRANSFORM));

        memcpy(&mapData.matWorld, &pTrans->Get_WorldMatrix(), sizeof(_float4x4));

        saveFile.write(reinterpret_cast<const char*>(&mapData), sizeof(MAPDATA));
    }

    saveFile.close();

    MSG_BOX("SAVE_COMPLETE");
}

void CImgui_Manager::_Load_Map()
{
    GAMEINSTANCE;

    ifstream mapFile("../Bin/Resources/MapFile/2.Bin", ios::in | ios::binary);

    _bool bCheck = mapFile.is_open();

    if (false == bCheck)
    {
        return;
    }

    mapFile.read(reinterpret_cast<char*>(&m_vecPlayerPos), sizeof(_float3));

    TERRAINDESC tagTerrainDesc;
    tagTerrainDesc.pPath = nullptr;

    mapFile.read(reinterpret_cast<char*>(&tagTerrainDesc.iX), sizeof(_uint));
    mapFile.read(reinterpret_cast<char*>(&tagTerrainDesc.iZ), sizeof(_uint));
    
    m_iNumVerticesX = tagTerrainDesc.iX;
    m_iNumVerticesZ = tagTerrainDesc.iZ;

    if (FAILED(pGameInstance->Add_GameObject(PG_TERRAIN, LEVEL::TOOL, LAYER_TERRAIN, GO_TERRAIN, &tagTerrainDesc)))
    {
        MSG_VOID_RETURN("Failed to Add_GameObject: CImgui_Manager::_Create_Terrain");
    }


    _uint iCount = 0;
    mapFile.read(reinterpret_cast<char*>(&iCount), sizeof(_uint));

    for (_uint i = 0; i < iCount; ++i)
    {
        MAPDATA mapData = {};

        mapFile.read(reinterpret_cast<char*>(&mapData), sizeof(MAPDATA));

        mapData.bUseMat = true;
        _uint iLayerTag = 0;

        if (PG_PLAYER == mapData.iObjectID)
        {
            iLayerTag = LAYER_PLAYER;
        }

        else if (PG_ENVIRONMENT == mapData.iObjectID)
        {
            iLayerTag = LAYER_ENVIRONMENT;
        }

        else
        {
            iLayerTag = LAYER_MONSTER;

            CGameObject* pGameObject = { nullptr };
            pGameInstance->Add_GameObject(PG_MONSTER, LEVEL::TOOL, iLayerTag, &pGameObject, &mapData);

            m_listObject.push_back(pGameObject);
            
            continue;
        }

        CGameObject* pGameObject = { nullptr };
        pGameInstance->Add_GameObject(mapData.iObjectID, LEVEL::TOOL, iLayerTag, &pGameObject, &mapData);

        m_listObject.push_back(pGameObject);
    }
    
    mapFile.close();

    MSG_BOX("LOAD_COMPLETE");
}

void CImgui_Manager::_Save_Nevi()
{
    ofstream saveFile("../Bin/Resources/Navi/1.dat", ios::out | ios::trunc | ios::binary);
    _bool bCheck = saveFile.is_open();
    /*if (false == bCheck)
    {
        return;
    }*/

    _uint iCount = _uint(m_listCell.size());
    if (0 == iCount)
    {
        return;
    }

    saveFile.write(reinterpret_cast<const char*>(&iCount), sizeof(_uint));

    _float3 vecTemp = {};
    for (auto& pCell : m_listCell)
    {
        vecTemp = pCell->Get_Point(CCell::POINT::POINT_A);
        saveFile.write(reinterpret_cast<const char*>(&vecTemp), sizeof(_float3));

        vecTemp = pCell->Get_Point(CCell::POINT::POINT_B);
        saveFile.write(reinterpret_cast<const char*>(&vecTemp), sizeof(_float3));

        vecTemp = pCell->Get_Point(CCell::POINT::POINT_C);
        saveFile.write(reinterpret_cast<const char*>(&vecTemp), sizeof(_float3));
    }

    saveFile.close();

    MSG_BOX("SAVE_COMPLETE");
}

void CImgui_Manager::_Load_Nevi()
{
    GAMEINSTANCE;

    ifstream mapFile("../Bin/Resources/Navi/1.dat", ios::in | ios::binary);

    _bool bCheck = mapFile.is_open();

    if (false == bCheck)
    {
        return;
    }

    _uint iCellCount = 0;
    mapFile.read(reinterpret_cast<char*>(&iCellCount), sizeof(_uint));
    
    if (0 == iCellCount)
    {
        return;
    }

    for (_uint i = 0; i < iCellCount; ++i)
    {
        _float3		vPoints[3];
        ZeroMemory(vPoints, sizeof(_float3) * 3);

        mapFile.read(reinterpret_cast<char*>(&m_vecNeviPos[0]), sizeof(_float3));
        mapFile.read(reinterpret_cast<char*>(&m_vecNeviPos[1]), sizeof(_float3));
        mapFile.read(reinterpret_cast<char*>(&m_vecNeviPos[2]), sizeof(_float3));

        _Decide_Order();

        vPoints[0] = m_vecNeviPos[0];
        vPoints[1] = m_vecNeviPos[1];
        vPoints[2] = m_vecNeviPos[2];

        CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_iCellCount++);

        m_listCell.push_back(pCell);
    }

    mapFile.close();
}

void CImgui_Manager::_Create_Effect()
{
    if (ImGui::CollapsingHeader("Effect"))
    {
        if (ImGui::Button("Create Particle", ImVec2(150.f, 20.f)))
        {
            if (!ImGui::IsItemActive())
            {
                GAMEINSTANCE;

                if (FAILED(pGameInstance->Add_GameObject(PG_SNOW_BLUE, LEVEL::TOOL, LAYER_BULLET, GO_EFFECT, &m_pParticle)))
                {
                    MSG_VOID_RETURN("Failed to Add_GameObject: CLevel_GamePlay::_Ready_Layer_Player");
                }
            }
        }


        if (ImGui::Button("Create Effect", ImVec2(150.f, 20.f)))
        {
            if (!ImGui::IsItemActive())
            {
                GAMEINSTANCE;

                CEffect::EFFECTDESC tagEffectDesc = {};

                tagEffectDesc.iModelID = PC_M_SLASH;
                tagEffectDesc.vecColor = m_vecEffectColor;

                if (FAILED(pGameInstance->Add_GameObject(PG_EFFECT, LEVEL::TOOL, LAYER_BULLET, GO_EFFECT, &m_pEffect, &tagEffectDesc)))
                {
                    MSG_VOID_RETURN("Failed to Add_GameObject: CLevel_GamePlay::_Ready_Layer_Player");
                }
            }
        }



    }
}

void CImgui_Manager::_Manipulate_Particle()
{
    if (ImGui::CollapsingHeader("Manipulate_Particle"))
    {
        ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("NumInstance", &m_iNumInstance, 0, 0, eFlag);



        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vScaleX", &m_vScale.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vScaleY", &m_vScale.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vScaleZ", &m_vScale.z, 0, 0, nullptr, eFlag);




        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("PivotPosX", &m_vecEffectPos.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("PivotPosY", &m_vecEffectPos.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("PivotPosZ", &m_vecEffectPos.z, 0, 0, nullptr, eFlag);



        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MaxDirRangeX", &m_vecMaxRange.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MaxDirRangeY", &m_vecMaxRange.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MaxDirRangeZ", &m_vecMaxRange.z, 0, 0, nullptr, eFlag);




        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MinDirRangeX", &m_vecMinRange.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MinDirRangeY", &m_vecMinRange.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MinDirRangeZ", &m_vecMinRange.z, 0, 0, nullptr, eFlag);




        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vecDirX", &m_vecDir.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vecDirY", &m_vecDir.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vecDirZ", &m_vecDir.z, 0, 0, nullptr, eFlag);
        

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vecMaxPosX", &m_vecPosMaxRange.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vecMaxPosY", &m_vecPosMaxRange.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vecMaxPosZ", &m_vecPosMaxRange.z, 0, 0, nullptr, eFlag);

        

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vecMinPosX", &m_vecPosMinRange.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vecMinPosY", &m_vecPosMinRange.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("vecMinPosZ", &m_vecPosMinRange.z, 0, 0, nullptr, eFlag);




        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MaxSpeed", &m_fMaxSpeed, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MinSpeed", &m_fMinSpeed, 0, 0, nullptr, eFlag);
        

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MaxLifeTime", &m_fMaxLifeTime, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("MinLifeTime", &m_fMinLifeTime, 0, 0, nullptr, eFlag);

        
        if (ImGui::Button("Set_Value", ImVec2(100.f, 20.f)) && nullptr != m_pParticle)
        {
            if (!ImGui::IsItemActive())
            {
                RECTINSTANCEDESC tagDesc = {};

                tagDesc.iNumInstance    = m_iNumInstance;
                tagDesc.vPivotPos       = m_vecEffectPos;
                tagDesc.vDirMaxRange    = m_vecMaxRange;
                tagDesc.vDirMinRange    = m_vecMinRange;
                tagDesc.fMaxSpeed       = m_fMaxSpeed;
                tagDesc.fMinSpeed       = m_fMinSpeed;
                tagDesc.vScale          = m_vScale;
                tagDesc.fMinLifeTime    = m_fMinLifeTime;
                tagDesc.fMaxLifeTime    = m_fMaxLifeTime;
                tagDesc.vPosMaxRange    = m_vecPosMaxRange;
                tagDesc.vPosMinRange    = m_vecPosMinRange;



                XMStoreFloat3(&m_vecDir, XMVector3Normalize(XMLoadFloat3(&m_vecDir)));
                tagDesc.vDir            = m_vecDir;

                static_cast<CVIBuffer_Rect_Instance*>(m_pParticle->Find_Component(CP_VIBUFFER_RECT))->Reset();
                static_cast<CVIBuffer_Rect_Instance*>(m_pParticle->Find_Component(CP_VIBUFFER_RECT))->Initialize(&tagDesc);
            }
        }

    }
}

void CImgui_Manager::_Manipulate_Effect()
{
    if (ImGui::CollapsingHeader("Manipulate_Effect"))
    {
        const char* EffectName[] = {
            "PC_M_SLASH",
            "PC_M_FIREBALL",
            "PC_M_POISON",

        };

        ImGui::BeginChild("Select Effect", ImVec2(200, 100), false);
        m_bHoverCheck = ImGui::IsWindowHovered();
        for (int i = 0; i < 3; i++)
        {
            m_bHoverCheck = ImGui::IsWindowHovered();
            if (ImGui::Selectable(EffectName[i], iSelectEffectIndex == i))
            {
                iSelectEffectIndex = i + 400;
            }
        }
        ImGui::EndChild();
        

        const char* PassName[] = {
            "Normal",
            "Move",
            "???",

        };
        ImGui::BeginChild("Select Shade", ImVec2(200, 100), false);
        m_bHoverCheck = ImGui::IsWindowHovered();
        for (int i = 0; i < 3; i++)
        {
            m_bHoverCheck = ImGui::IsWindowHovered();
            if (ImGui::Selectable(PassName[i], iSelectShadeIndex == i))
            {
                iSelectShadeIndex = i + 1;
            }
        }
        ImGui::EndChild();


        ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("ColorR", &m_vecEffectColor.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("ColorG", &m_vecEffectColor.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("ColorB", &m_vecEffectColor.z, 0, 0, nullptr, eFlag);

        if (ImGui::Button("Change Effect", ImVec2(150.f, 20.f)))
        {
            if (!ImGui::IsItemActive())
            {
                CEffect::EFFECTDESC tagEffectDesc = {};

                tagEffectDesc.iModelID = iSelectEffectIndex;
                tagEffectDesc.vecColor = m_vecEffectColor;
                tagEffectDesc.iPassIndex = iSelectShadeIndex;

                static_cast<CEffect*>(m_pEffect)->Set_Desc(&tagEffectDesc);
            }
        }
    }

    

}

void CImgui_Manager::_Create_Model()
{
    if (ImGui::CollapsingHeader("Envi"))
    {
        ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Pos:X", &m_vecPickingPos.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Pos:Y", &m_vecPickingPos.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Pos:Z", &m_vecPickingPos.z, 0, 0, nullptr, eFlag);

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Scale:X", &m_vecScale.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Scale:Y", &m_vecScale.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Scale:Z", &m_vecScale.z, 0, 0, nullptr, eFlag);

        const char* ModelName[] = {
        "PC_M_ARCH1",
        "PC_M_ARCH2",
        "PC_M_FIREPLACE1",
        "PC_M_FIREPLACE2",
        "PC_M_FLORA1",
        "PC_M_FLORA2",
        "PC_M_FLORA3",
        "PC_M_FLORA4",
        "PC_M_FLORA5",
        "PC_M_FLORA6",
        "PC_M_FLORA7",
        "PC_M_FLORA8",
        "PC_M_FLORA9",
        "PC_M_FLORA10",
        "PC_M_HOUSE1",
        "PC_M_HOUSE2",
        "PC_M_HOUSE3",
        "PC_M_HOUSE4",
        "PC_M_LAMP",
        "PC_M_OBSTACLE1",
        "PC_M_ROAD1",
        "PC_M_ROAD2",
        "PC_M_ROAD3",
        "PC_M_ROAD4",
        "PC_M_ROAD5",
        "PC_M_ROCK1",
        "PC_M_ROCK2",
        "PC_M_ROCK3",
        "PC_M_ROCK4",
        "PC_M_ROCK5",
        "PC_M_ROCK6",
        "PC_M_ROCK7",
        "PC_M_TOTEMC",
        "PC_M_TOTEML",
        "PC_M_TOTEMR",
        "PC_M_TREE1",
        "PC_M_TREE2",
        "PC_M_TREE3",
        "PC_M_TREE4",
        "PC_M_BARREL1",
        "PC_M_BARREL2",
        "PC_M_FENCE1",
        "PC_M_GRASS1",
        "PC_M_GRASS2",
        "PC_M_GRASS3",
        "PC_M_GRASS4",
        "PC_M_GROUND_SIDE1",
        "PC_M_GROUND_SIDE2",
        "PC_M_GROUND_SIDE3",
        "PC_M_GROUND_SIDE4",
        "PC_M_HOLE1",
        "PC_M_LANTERN1",
        "PC_M_CRACK",
        "PC_M_VULPIS"
        };

        ImGui::BeginChild("Select Model", ImVec2(200, 0), false);
        m_bHoverCheck = ImGui::IsWindowHovered();
        for (int i = 0; i < 54; i++)
        {
            m_bHoverCheck = ImGui::IsWindowHovered();
            if (ImGui::Selectable(ModelName[i], m_iSelectModelIndex == i))
            {
                m_iSelectModelIndex = i + 202;
            }
        }
        ImGui::EndChild();


        if (ImGui::Button("Create Model", ImVec2(100.f, 30.f)))
        {
            if (!ImGui::IsItemActive())
            {
                GAMEINSTANCE;
                CULLEVEL;

                CGameObject* pGameObject = nullptr;

                MAPDATA mapData = {};
                mapData.bUseMat = false;
                mapData.iModelID = m_iSelectModelIndex;

                if (FAILED(pGameInstance->Add_GameObject(PG_ENVIRONMENT, LEVEL::TOOL, LAYER_ENVIRONMENT, GO_ENVIRONMENT, &pGameObject, &mapData)))
                {
                    MSG_VOID_RETURN("Failed to Add_GameObject: CImgui_Manager::_Create_Model");
                }

                _vector vPos = XMVectorSet(m_vecPickingPos.x, m_vecPickingPos.y, m_vecPickingPos.z, 1.f);
                static_cast<CTransform*>(pGameObject->Find_Component(CP_TRANSFORM))->Set_State(STATE::POSITION, vPos);

                static_cast<CTransform*>(pGameObject->Find_Component(CP_TRANSFORM))->Set_Scale(m_vecScale);

                m_listObject.push_back(pGameObject);
                m_pPickingModel = pGameObject;
            }
        }

        SameLine();
        if (ImGui::Button("Create Player", ImVec2(100.f, 30.f)))
        {
            if (!ImGui::IsItemActive())
            {
                GAMEINSTANCE;
                CULLEVEL;

                CGameObject* pGameObject = nullptr;

                if (FAILED(pGameInstance->Add_GameObject(PG_PLAYER, LEVEL::TOOL, LAYER_PLAYER, GO_PLAYER, &pGameObject)))
                {
                    MSG_VOID_RETURN("Failed to Add_GameObject: CImgui_Manager::_Create_Terrain");
                }

                _vector vPos = XMVectorSet(m_vecPickingPos.x, m_vecPickingPos.y, m_vecPickingPos.z, 1.f);
                static_cast<CTransform*>(pGameObject->Find_Component(CP_TRANSFORM))->Set_State(STATE::POSITION, vPos);

                static_cast<CTransform*>(pGameObject->Find_Component(CP_TRANSFORM))->Set_Scale(m_vecScale);

            }
        }


    }
}

void CImgui_Manager::_Create_Monster()
{
    if (ImGui::CollapsingHeader("Mon"))
    {
        ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Pos:X", &m_vecPickingPos.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Pos:Y", &m_vecPickingPos.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Pos:Z", &m_vecPickingPos.z, 0, 0, nullptr, eFlag);

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Scale:X", &m_vecScale.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Scale:Y", &m_vecScale.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Scale:Z", &m_vecScale.z, 0, 0, nullptr, eFlag);

        const char* ModelName[] = {
        "PC_M_HEDGEHOG",
        "PC_M_SCARAB",
        "PC_M_SCORPION",
        "PC_M_SHOOTER",
        "PC_M_SHOOTING_PLANT",
        "PC_M_SPIDER",
        "PC_M_TENTACLE",
        "PC_M_WORM"
        };

        ImGui::BeginChild("Select Mon", ImVec2(200, 0), false);
        m_bHoverCheck = ImGui::IsWindowHovered();
        for (int i = 0; i < 8; i++)
        {
            m_bHoverCheck = ImGui::IsWindowHovered();
            if (ImGui::Selectable(ModelName[i], m_iSelectModelIndex == i))
            {
                m_iSelectModelIndex = i + 300;
                m_iSelectObjectIndex = i + 7;
            }
        }
        ImGui::EndChild();


        if (ImGui::Button("Create Mon", ImVec2(100.f, 30.f)))
        {
            if (!ImGui::IsItemActive())
            {
                GAMEINSTANCE;
                CULLEVEL;

                CGameObject* pGameObject = nullptr;

                MAPDATA mapData = {};
                mapData.bUseMat = false;
                mapData.iModelID = m_iSelectModelIndex;
                mapData.iObjectID = m_iSelectObjectIndex;

                if (FAILED(pGameInstance->Add_GameObject(PG_MONSTER, LEVEL::TOOL, LAYER_MONSTER, GO_MONSTER, &pGameObject, &mapData)))
                {
                    MSG_VOID_RETURN("Failed to Add_GameObject: CImgui_Manager::_Create_Model");
                }

                _vector vPos = XMVectorSet(m_vecPickingPos.x, m_vecPickingPos.y, m_vecPickingPos.z, 1.f);
                static_cast<CTransform*>(pGameObject->Find_Component(CP_TRANSFORM))->Set_State(STATE::POSITION, vPos);

                static_cast<CTransform*>(pGameObject->Find_Component(CP_TRANSFORM))->Set_Scale(m_vecScale);

                m_listObject.push_back(pGameObject);
            }
        }
    }
}

void CImgui_Manager::_Create_Nevi()
{
    if (ImGui::CollapsingHeader("Nevi"))
    {
        ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("1X", &m_vecNeviPos[0].x, 0, 0, nullptr, eFlag);
        ImGui::SameLine();

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("1Y", &m_vecNeviPos[0].y, 0, 0, nullptr, eFlag);
        ImGui::SameLine();

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("1Z", &m_vecNeviPos[0].z, 0, 0, nullptr, eFlag);

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("2X", &m_vecNeviPos[1].x, 0, 0, nullptr, eFlag);
        ImGui::SameLine();

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("2Y", &m_vecNeviPos[1].y, 0, 0, nullptr, eFlag);
        ImGui::SameLine();

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("2Z", &m_vecNeviPos[1].z, 0, 0, nullptr, eFlag);

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("3X", &m_vecNeviPos[2].x, 0, 0, nullptr, eFlag);
        ImGui::SameLine();

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("3Y", &m_vecNeviPos[2].y, 0, 0, nullptr, eFlag);
        ImGui::SameLine();

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("3Z", &m_vecNeviPos[2].z, 0, 0, nullptr, eFlag);

        if (ImGui::Button("Create Navi", ImVec2(100.f, 20.f)))
        {
            if (!ImGui::IsItemActive())
            {
                _float3		vPoints[3];
                ZeroMemory(vPoints, sizeof(_float3) * 3);
                
                m_vecNeviPos[0].y = 0.1f;
                m_vecNeviPos[1].y = 0.1f;
                m_vecNeviPos[2].y = 0.1f;

                _Decide_Order();
                
                vPoints[0] = m_vecNeviPos[0];
                vPoints[1] = m_vecNeviPos[1];
                vPoints[2] = m_vecNeviPos[2];

                CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_iCellCount++);

                m_listCell.push_back(pCell);

                m_vecNeviPos[0] = {};
                m_vecNeviPos[1] = {};
                m_vecNeviPos[2] = {};
                m_iCheckCount = 1;
            }
        }
    }
}

void CImgui_Manager::_Create_Npc()
{
    if (ImGui::CollapsingHeader("Npc"))
    {
        ImGuiInputTextFlags_ eFlag = ImGuiInputTextFlags_CharsDecimal;

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Pos:X", &m_vecPickingPos.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Pos:Y", &m_vecPickingPos.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Pos:Z", &m_vecPickingPos.z, 0, 0, nullptr, eFlag);

        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Scale:X", &m_vecScale.x, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Scale:Y", &m_vecScale.y, 0, 0, nullptr, eFlag);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Scale:Z", &m_vecScale.z, 0, 0, nullptr, eFlag);

        const char* ModelName[] = {
        "PC_M_NPC_WEAPON",
        "PC_M_NPC_RUNE",
        "PC_M_NPC_SKILL"
        };

        ImGui::BeginChild("Select Npc", ImVec2(200, 0), false);
        m_bHoverCheck = ImGui::IsWindowHovered();
        for (int i = 0; i < 3; i++)
        {
            m_bHoverCheck = ImGui::IsWindowHovered();
            if (ImGui::Selectable(ModelName[i], m_iSelectModelIndex == i))
            {
                m_iSelectModelIndex = i + 256;
            }
        }
        ImGui::EndChild();


        if (ImGui::Button("Create Mon", ImVec2(100.f, 30.f)))
        {
            if (!ImGui::IsItemActive())
            {
                GAMEINSTANCE;
                CULLEVEL;

                CGameObject* pGameObject = nullptr;

                MAPDATA mapData = {};
                mapData.bUseMat = false;
                mapData.iModelID = m_iSelectModelIndex;

                if (FAILED(pGameInstance->Add_GameObject(PG_NPC, LEVEL::TOOL, LAYER_NPC, GO_NPC, &pGameObject, &mapData)))
                {
                    MSG_VOID_RETURN("Failed to Add_GameObject: CImgui_Manager::_Create_Model");
                }

                _vector vPos = XMVectorSet(m_vecPickingPos.x, m_vecPickingPos.y, m_vecPickingPos.z, 1.f);
                static_cast<CTransform*>(pGameObject->Find_Component(CP_TRANSFORM))->Set_State(STATE::POSITION, vPos);

                static_cast<CTransform*>(pGameObject->Find_Component(CP_TRANSFORM))->Set_Scale(m_vecScale);

                m_listObject.push_back(pGameObject);
            }
        }
    }
}

void CImgui_Manager::_Delete_Model()
{
    for (auto iter = m_listObject.begin();
        iter != m_listObject.end();
        ++iter)
    {
        if ((*iter) == m_pPickingModel)
        {
            m_listObject.erase(iter);
            break;
        }
    }

    m_pPickingModel->Set_Delete();
    m_pPickingModel = nullptr;
    m_pControlModel = nullptr;
}

void CImgui_Manager::_Delete_Nevi()
{
    m_listCell.pop_back();
}

void CImgui_Manager::Free()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // 리스트 날리기

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
