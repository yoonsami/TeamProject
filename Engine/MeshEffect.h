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

        // Textures
        string      strTextures[8];           

        // Color 
        Color       vBaseColor;       

        _bool       bGradationOn;
        _float      fGradationIntensity;
        Color       vGradationColor;

        _bool       bOverlayOn;
        Color       vOverlayColor_Start;
        Color       vOverlayColor_End;

        // Changing Color
        _bool       bChangingColorOn;
        Color       vDestBaseColor;
        Color       vDestGradationColor;
        Color       vDestOverlayColor_Start;
        Color       vDestOverlayColor_End;

        // Texture UV
        _float2     vTexTiling;
        _float2     vTexUVSpeed;

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
    shared_ptr<Shader>  Get_Shader() { return m_pShader; }

private:
    void            Init_RenderParams();

    void            Bind_RenderParams_ToShader();

private:
    DESC                    m_tDesc;

    _float                  m_fCurrAge = { 0.f };
    _float2                 m_vCurrTexUVOffset = { 0.f, 0.f };

    /* Component */
    shared_ptr<Shader>      m_pShader = { nullptr };
    shared_ptr<Mesh>        m_pMesh = { nullptr };
    shared_ptr<Model>       m_pModel = { nullptr };
    shared_ptr<Material>    m_pMaterial = { nullptr };   

    /* Data to bind Shader */
    RenderParams            m_RenderParams{};
};

