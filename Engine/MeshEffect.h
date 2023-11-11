#pragma once
#include "Component.h"
#include "MeshEffectData.h"

class MeshEffect : public Component 
{ 
public:
    MeshEffect(shared_ptr<Shader> shader);
    virtual ~MeshEffect();

public:
    virtual void    Init(void* pArg);
    virtual void    Tick() override;
    virtual void    Final_Tick() override;  // CS
    void            Render();               // VS, PS

    void            Update_Desc();          // use in tool to fix 

    /* Getter */
    shared_ptr<Material> Get_Material() { return  m_pMaterial; }
    shared_ptr<Shader>   Get_Shader() { return m_pShader; }

    /* Setter */
    void            Set_IsImmortal(_bool bState) { m_bIsImmortal = bState; }
    void            Set_IsPlayOnce(_bool bState) { m_bIsPlayOnce = bState; }
    void            Set_IsPlayLoop(_bool bState) { m_bIsPlayLoop = bState; }
    void            Set_IsAlwaysShowFirstTick(_bool bState) { m_bIsAlwaysShowFirstTick = bState; }
    void            Set_Desc(MeshEffectData::DESC tDesc) { m_tDesc = tDesc; }

public:
    MeshEffectData::DESC    m_tDesc;    // use in tool to fix 

private:
    void                    Init_RenderParams();

    void                    Bind_RenderParams_ToShader();

private:
    Color                   m_vDiffuseColor_Base;

    _float                  m_fCurrAge = { 0.f };

    _float2                 m_vCurrTexUVOffset_Opacity      = { 0.f, 0.f };
    _float2                 m_vCurrTexUVOffset_Gra          = { 0.f, 0.f };
    _float2                 m_vCurrTexUVOffset_Overlay      = { 0.f, 0.f };
    _float2                 m_vCurrTexUVOffset_Dissolve     = { 0.f, 0.f };
    _float2                 m_vCurrTexUVOffset_Distortion   = { 0.f, 0.f };

    _bool                   m_bIsPlayFinished = { false };              
    _bool                   m_bIsPlayOnce = { false };      
    _bool                   m_bIsPlayLoop = { true };
    _bool                   m_bIsAlwaysShowFirstTick = { false };
    _bool                   m_bIsImmortal = { false };                 // normaly use in tool 

    /* Component */
    shared_ptr<Shader>      m_pShader = { nullptr };
    shared_ptr<Mesh>        m_pMesh = { nullptr };
    shared_ptr<Model>       m_pModel = { nullptr };
    shared_ptr<Material>    m_pMaterial = { nullptr };   

    /* Data to bind Shader */
    RenderParams            m_RenderParams{};
};

