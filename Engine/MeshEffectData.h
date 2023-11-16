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
        _int        iSamplerType;

        // Mesh 
        string      strVfxMesh;

        /* Sprite Animation */
        _bool       bUseSpriteAnim;
        _int        iNumSprite_Col;
        _int        iNumSprite_Row;
        _float      fAnimSpeed;

        /* Coloring Options */
        _bool       bColorChangingOn;

        // Option1
        string      strTexture_Op1;
        _int        bIsUseTextureColor_Op1;
        Color       vBaseColor1_Op1;
        Color       vBaseColor2_Op1;
        Color       vDestColor1_Op1;
        Color       vDestColor2_Op1;
        _float2     vTiling_Op1;
        _float2     vUVSpeed_Op1;
        _float      fContrast_Op1;
        _float      fAlphaOffset_Op1;

        // Option2
        string      strTexture_Op2;
        _int        bIsUseTextureColor_Op2;
        Color       vBaseColor1_Op2;
        Color       vBaseColor2_Op2;
        Color       vDestColor1_Op2;
        Color       vDestColor2_Op2;
        _float2     vTiling_Op2;
        _float2     vUVSpeed_Op2;
        _float      fContrast_Op2;
        _float      fAlphaOffset_Op2;

        // Option3
        string      strTexture_Op3;
        _int        bIsUseTextureColor_Op3;
        Color       vBaseColor1_Op3;
        Color       vBaseColor2_Op3;
        Color       vDestColor1_Op3;
        Color       vDestColor2_Op3;
        _float2     vTiling_Op3;
        _float2     vUVSpeed_Op3;
        _float      fContrast_Op3;
        _float      fAlphaOffset_Op3;

        // Option4
        string      strTexture_Op4;
        _int        bIsUseTextureColor_Op4;
        Color       vBaseColor1_Op4;
        Color       vBaseColor2_Op4;
        Color       vDestColor1_Op4;
        Color       vDestColor2_Op4;
        _float2     vTiling_Op4;
        _float2     vUVSpeed_Op4;
        _float      fContrast_Op4;
        _float      fAlphaOffset_Op4;

        // Overlay
        _bool       bIsOverlayOn;
        string      strOverlayTexture;
        Color       vBaseColor_Overlay;
        Color       vDestColor_Overlay;
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
    virtual void   Load(const wstring& path) override;

    /* Setter */
    void            Set_Desc(DESC tDesc);
    void            Set_TransformDesc(Transform_Desc tDesc) { m_tTransformDesc = tDesc; }

    /* Getter */
    wstring         Get_MeshEffectDataTag() { return m_wstrTag; }
    DESC           Get_Desc() { return m_tDesc; }
    Transform_Desc  Get_TransformDesc() { return m_tTransformDesc; }

private:
    wstring         m_wstrTag;
    DESC           m_tDesc;
    Transform_Desc  m_tTransformDesc;
};
