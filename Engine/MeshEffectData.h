#pragma once
#include "ResourceBase.h"

class MeshEffectData : public ResourceBase
{
public:
	typedef struct tagMeshEffectVData
	{
        // Property
        const char* pszTag;
        _float      fDuration;
        _bool       bBlurOn;
        _bool       bUseFadeOut;
        _int        iMeshCnt;
        _float      fCreateInterval;
        _float2     vParticleDuration;

        // Mesh 
        string      strVfxMesh;

        /* Sprite Animation */
        _bool       bUseSpriteAnim;
        _int        iNumSprite_Col;
        _int        iNumSprite_Row;
        _float      fAnimSpeed;

        /* Coloring Options */
        _bool       bColorChangingOn;

        // Diffuse
        string      strDiffuseTexture;
        Color       vDiffuseColor_BaseStart;
        Color       vDiffuseColor_BaseEnd;
        Color       vDestColor_Diffuse;
        _float2     vTiling_Diffuse;
        _float2     vUVSpeed_Diffuse;

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
        _float2     vTiling_Blend;
        _float2     vUVSpeed_Blend;

        // Color Editor
        _float      fContrast;
	}DESC;

    typedef struct tagMeshEffectTransformData
    {
        // Init position
        _float3     vPosRange;

        // Init scale
        _float3     vInitScale_Min;
        _float3     vInitScale_Max;

        // Init rotation
        _float3     vInitRotation_Min;
        _float3     vInitRotation_Max;

        // Translate
        _int        iTranslateOption;
        _float      fTranslateSpeed;
        _float3     vEndPosOffset_Min;
        _float3     vEndPosOffset_Max;

        // Scaling 
        _int        iScalingOption;
        _float3     vEndScale;

        // Turn 
        _int        iTurnOption;
        _float      fTurnSpeed;
        _float3     vRandomAxis_Min;
        _float3     vRandomAxis_Max;

    }Transform_Desc;
public:
	MeshEffectData();
	~MeshEffectData();

public:
    virtual void	Load(const wstring& path) override;

    /* Setter */
    void            Set_Desc(DESC tDesc);
    void            Set_TransformDesc(Transform_Desc tDesc) { m_tTransformDesc = tDesc; }

    /* Getter */
    wstring         Get_MeshEffectDataTag() { return m_wstrTag; }
	DESC	        Get_Desc() { return m_tDesc; }
    Transform_Desc  Get_TransformDesc() { return m_tTransformDesc; }

private:
    wstring         m_wstrTag;
	DESC	        m_tDesc;
    Transform_Desc  m_tTransformDesc;
};

