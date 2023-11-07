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
        
        // Mesh 
        string      strVfxMesh;

        // Textures
        pair<_int, string>   pTexture[7] = { {(_int)TextureMapType::END, "None"} };

        // Color 
        Color               vRangeStartColor;       // to deside start color
        Color               vRangeEndColor;         // to deside start color

        _int                iChangingColorOption;   // linear, curve
        Color               vDestColor;             

        _float              fGradationIntensity;    // The larger the value, the greater the percentage of GradationColor.
        Color               vGradationColor;
        Color               vDestGradationColor;

        _bool               bUseFadeOut;

    }DESC;

public:
    MeshEffect(shared_ptr<Shader> shader);
    virtual ~MeshEffect();

public:
    virtual HRESULT Init(void* pArg);
    virtual void    Tick() override;
    virtual void    Final_Tick() override;  // CS
    void            Render();               // VS, PS

private:
    void            Init_RenderParams();

    void            Bind_RenderParams_ToShader();

private:
    DESC                    m_tDesc;

    _float                  m_fCurrAge = { 0.f };

    /* Component */
    shared_ptr<Shader>      m_pShader = { nullptr };
    shared_ptr<Mesh>        m_pMesh = { nullptr };
    shared_ptr<Model>       m_pModel = { nullptr };
    shared_ptr<Material>    m_pMaterial = { nullptr };   

    /* Data to bind Shader */
    RenderParams            m_RenderParams{};
};

