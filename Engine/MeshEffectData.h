#pragma once
#include "ResourceBase.h"

class MeshEffectData : public ResourceBase
{
public:
	typedef struct tagMeshEffectData
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
        Color       vDiffuseColor_BaseStart;
        Color       vDiffuseColor_BaseEnd;
        Color       vDestColor_Diffuse;

        // Opacity
        string      strOpacityTexture;
        _int        iSamplerType;
        _float2     vTiling_Opacity;
        _float2     vUVSpeed_Opacity;

        // Alpha Gradation
        _float      fAlphaGraIntensity;
        Color       vBaseColor_AlphaGra;
        Color       vDestColor_AlphaGra;

        // Gradation
        string      strGraTexture;
        Color       vBaseColor_Gra;
        Color       vDestColor_Gra;
        _float2     vTiling_Gra;
        _float2     vUVSpeed_Gra;

        // Overlay
        _bool       bIsOverlayOn;
        string      strOverlayTexture;
        Color       vBaseColor_Overlay;
        _float2     vTiling_Overlay;
        _float2     vUVSpeed_Overlay;

        // Normal 
        string      strNormalTexture;

        // Dissolve
        string      strDissolveTexture;
        _float2     vTiling_Dissolve;
        _float2     vUVSpeed_Dissolve;
        _bool       bInverseDissolve;

        // Distortion
        string      strDistortionTexture;
        _float2     vTiling_Distortion;
        _float2     vUVSpeed_Distortion;

        // Blend 
        string      strBlendTexture;

        // Color Editor
        _float      fContrast;
	}DESC;

public:
	MeshEffectData();
	~MeshEffectData();

public:
    virtual void	Load(const wstring& path) override;

    /* Setter */
    void            Set_Desc(DESC tDesc);

    /* Getter */
	DESC	        Get_Desc() { return m_tDesc; }
    wstring         Get_MeshEffectDataTag() { return m_wstrTag; }

private:
    wstring m_wstrTag;
	DESC	m_tDesc;
};

