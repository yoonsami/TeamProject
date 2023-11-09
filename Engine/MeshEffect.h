#pragma once
#include "Component.h"

class MeshEffect : public Component 
{ 
public:
    typedef struct tagEffect
    {
        // Property
        const char* pszTag;
        _float      fDuration;
        _bool       bBlurOn;
        _bool       bUseFadeOut;
        
        // Mesh 
        string      strVfxMesh;

        /* Coloring Options */
        _bool       bColorChangingOn;

        // Diffuse
        string      strDiffuseTexture;
        Color       BaseColor_Diffuse;
        Color       DestColor_Diffuse;

        // Opacity
        string      strOpacityTexture;
        _int        iSamplerType;
        _float2     vTiling_Opacity;
        _float2     vUVSpeed_Opacity;

        // Alpha Gradation
        _float      fAlphaGraIntensity;
        Color       BaseColor_AlphaGra;
        Color       DestColor_AlphaGra;

        // Gradation
        string      strGraTexture;
        Color       BaseColor_Gra;
        Color       DestColor_Gra;
        _float2     vTiling_Gra;
        _float2     vUVSpeed_Gra;

        // Overlay
        _bool       bIsOverlayOn;
        string      strOverlayTexture;
        Color       BaseColor_Overlay;
        _float2     vTiling_Overlay;
        _float2     vUVSpeed_Overlay;

        // Normal 
        string      strNormalTexture;

        // Dissolve
        string      strDissolveTexture;
        _float2     vTiling_Dissolve;
        _float2     vUVSpeed_Dissolve;

        // Distortion
        string      strDistortionTexture;
        _float2     vTiling_Distortion;
        _float2     vUVSpeed_Distortion;
    }DESC;

public:
    MeshEffect(shared_ptr<Shader> shader);
    virtual ~MeshEffect();

public:
    virtual HRESULT Init(void* pArg);
    virtual void    Tick() override;
    virtual void    Final_Tick() override;  // CS
    void            Render();               // VS, PS

    /* Getter */
    shared_ptr<Material> Get_Material() { return  m_pMaterial; }
    shared_ptr<Shader>   Get_Shader() { return m_pShader; }

private:
    void            Init_RenderParams();

    void            Bind_RenderParams_ToShader();

private:
    DESC                    m_tDesc;

    _float                  m_fCurrAge = { 0.f };

    _float2                 m_vCurrTexUVOffset_Opacity      = { 0.f, 0.f };
    _float2                 m_vCurrTexUVOffset_Gra          = { 0.f, 0.f };
    _float2                 m_vCurrTexUVOffset_Overlay      = { 0.f, 0.f };
    _float2                 m_vCurrTexUVOffset_Dissolve     = { 0.f, 0.f };
    _float2                 m_vCurrTexUVOffset_Distortion   = { 0.f, 0.f };

    /* Component */
    shared_ptr<Shader>      m_pShader = { nullptr };
    shared_ptr<Mesh>        m_pMesh = { nullptr };
    shared_ptr<Model>       m_pModel = { nullptr };
    shared_ptr<Material>    m_pMaterial = { nullptr };   

    /* Data to bind Shader */
    RenderParams            m_RenderParams{};
};

