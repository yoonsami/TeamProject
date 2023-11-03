#pragma once

#include "Base.h"

BEGIN(Engine)
class CCell;
class CShader;
class CTransform;
class CGameObject;
class CVIBuffer_Rect_Instance;
END

BEGIN(Tool)

class CImgui_Manager final : public CBase
{
    DECLARE_SINGLETON(CImgui_Manager)

private:
    CImgui_Manager()        DEFAULT;
    ~CImgui_Manager()       DEFAULT;

public:
    HRESULT                 initialize(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContextconst, GRAPHICDESC& _tagDesc);
    void                    Tick(_float _fTimeDelta);
    HRESULT                 Render();

private:
    void                    _Cell_Render();
    void                    _Start_Map_Tool();
    void                    _Key_Input();
    void                    _Draw_Grid();

    void                    _Create_Terrain();
    void                    _Create_Model();
    void                    _Create_Monster();
    void                    _Create_Nevi();
    void                    _Create_Npc();
    //void                    _Delete_Terrain();
    void                    _Delete_Model();
    void                    _Delete_Nevi();

    void                    _Picking_Terrain();
    void                    _Picking_Model();
    void                    _Picking_Nevi();
    void                    _Picking_Cell();
    //void                    _Set_PlayerPos();

    _float3                 _Check_Around(_float3 _vecPickingPos);
    _bool                   _Calculate_Distance(_fvector _v1, _fvector _v2);


    void                    _Manipulate_Model();
    void                    _Manipulate_Value_Model();

    void                    _Decide_Order();
    void                    _Move_All_Object(_bool _bIsX, _bool _bIsZ, _bool _bIsPlus);

    void                    _Save_Map();
    void                    _Load_Map();

    void                    _Save_Nevi();
    void                    _Load_Nevi();

// Effect
    void                    _Start_Effect_Tool();


    void                    _Create_Effect();

    void                    _Manipulate_Particle();
    void                    _Manipulate_Effect();



private:
    ID3D11Device*           m_pDevice               = { nullptr };
    ID3D11DeviceContext*    m_pContext              = { nullptr };

    _bool                   m_bMapToolRender        = { false };
    _int                    m_iNumVerticesX         = { 0 };
    _int                    m_iNumVerticesZ         = { 0 };

    _bool                   m_bEffectToolRender     = { false };

    _float3                 m_vecPlayerPos          = {};
    _float3                 m_vecPickingPos         = {};
    _float3                 m_vecScale              = {};
    
    _float3                 m_vecInputPos           = {};
    _float3                 m_vecInputScale         = {};
    
    _bool                   m_bHoverCheck           = { false };
    _uint                   m_iSelectModelIndex     = { 0 };
    _uint                   m_iSelectObjectIndex    = { 0 };

    _bool                   m_bManipulate_On        = { false };
    CGameObject*            m_pPickingModel         = { nullptr };
    CTransform*             m_pControlModel         = { nullptr };
    ImGuizmo::OPERATION     m_eZmoOper              = { ImGuizmo::OPERATION::TRANSLATE };
    
    CShader*                m_pCellShader           = { nullptr };
    const _float4x4*        m_pWorldMatrix          = { nullptr };
    _float4					m_vecColor              = { 0.f, 1.f, 0.f, 1.f };
    _uint                   m_iCheckCount           = { 1 };
    _uint                   m_iCellCount            = { 1 };
    vector<_float3>         m_vecNeviPos;
    
    
    list<CGameObject*>      m_listObject;
    list<CCell*>            m_listCell;


// Particle
    CGameObject*            m_pParticle             = { nullptr };

    _int                    m_iNumInstance          = { 0 };
    _float4                 m_vecEffectPos          = { 0.f, 0.f, 0.f, 1.f};
    _float3                 m_vecMaxRange           = {};
    _float3                 m_vecMinRange           = {};
    _float3                 m_vecPosMaxRange        = {};
    _float3                 m_vecPosMinRange        = {};
    _float                  m_fMaxSpeed             = {};
    _float                  m_fMinSpeed             = {};
    _float3                 m_vecDir                = {};
    _float3                 m_vScale                = {};
    _float		            m_fMinLifeTime          = {};
    _float		            m_fMaxLifeTime          = {};

// Effect
    CGameObject*            m_pEffect               = { nullptr };

    _float4                 m_vecEffectColor        = { 0.f, 0.f, 0.f, 1.f };
    _uint                   iSelectEffectIndex      = { 400 };
    _uint                   iSelectShadeIndex       = { 1 };



public:
    virtual void            Free() override;
};

END