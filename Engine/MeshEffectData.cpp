#include "pch.h"
#include "MeshEffectData.h"

#include "Utils.h"
#include "FileUtils.h"

MeshEffectData::MeshEffectData()
    :ResourceBase(ResourceType::MeshEffectData)
{
}

MeshEffectData::~MeshEffectData()
{
}

void MeshEffectData::Load(const wstring& path)
{
    _float2 tiling = { 0.f, 0.f };
    _float2 UVSpeed = { 0.f, 0.f };

    shared_ptr<FileUtils> file = make_shared<FileUtils>();
    file->Open(path, FileMode::Read);

    // For. Load Desc 
    /* Property */
    string strTag = file->Read<string>();
    m_tDesc.pszTag = strTag.c_str();
    m_wstrTag = Utils::ToWString(strTag);
    m_tDesc.fDuration = file->Read<_float>();
    m_tDesc.bBlurOn = file->Read<_bool>();
    m_tDesc.bUseFadeOut = file->Read<_bool>();
    m_tDesc.iMeshCnt = file->Read<_int>();
    m_tDesc.fCreateInterval = file->Read<_float>();
    m_tDesc.vParticleDuration = file->Read<_float2>();
    m_tDesc.iSamplerType = file->Read<_int>();

    /* Mesh */
    m_tDesc.strVfxMesh = file->Read<string>();

    /* Sprite Animation */
    m_tDesc.bUseSpriteAnim = file->Read<_bool>();
    m_tDesc.iNumSprite_Col = file->Read<_int>();
    m_tDesc.iNumSprite_Row = file->Read<_int>();
    m_tDesc.fAnimSpeed = file->Read<_float>();

    /* Coloring Option */
    m_tDesc.bColorChangingOn = file->Read<_bool>();

    /* Option1 */
    m_tDesc.strTexture_Op1 = file->Read<string>();
    m_tDesc.bIsUseTextureColor_Op1 = file->Read<_int>();
    m_tDesc.vBaseColor1_Op1 = file->Read<_float4>();
    m_tDesc.vBaseColor2_Op1 = file->Read<_float4>();
    m_tDesc.vDestColor1_Op1 = file->Read<_float4>();
    m_tDesc.vDestColor2_Op1 = file->Read<_float4>();
    m_tDesc.vTiling_Op1 = file->Read<_float2>();
    m_tDesc.vUVSpeed_Op1 = file->Read<_float2>();
    m_tDesc.fContrast_Op1 = file->Read<_float>();
    m_tDesc.fAlphaOffset_Op1 = file->Read<_float>();

    /* Option2 */
    m_tDesc.strTexture_Op2 = file->Read<string>();
    m_tDesc.bIsUseTextureColor_Op2 = file->Read<_int>();
    m_tDesc.vBaseColor1_Op2 = file->Read<_float4>();
    m_tDesc.vBaseColor2_Op2 = file->Read<_float4>();
    m_tDesc.vDestColor1_Op2 = file->Read<_float4>();
    m_tDesc.vDestColor2_Op2 = file->Read<_float4>();
    m_tDesc.vTiling_Op2 = file->Read<_float2>();
    m_tDesc.vUVSpeed_Op2 = file->Read<_float2>();
    m_tDesc.fContrast_Op2 = file->Read<_float>();
    m_tDesc.fAlphaOffset_Op2 = file->Read<_float>();

    /* Option3 */
    m_tDesc.strTexture_Op3 = file->Read<string>();
    m_tDesc.bIsUseTextureColor_Op3 = file->Read<_int>();
    m_tDesc.vBaseColor1_Op3 = file->Read<_float4>();
    m_tDesc.vBaseColor2_Op3 = file->Read<_float4>();
    m_tDesc.vDestColor1_Op3 = file->Read<_float4>();
    m_tDesc.vDestColor2_Op3 = file->Read<_float4>();
    m_tDesc.vTiling_Op3 = file->Read<_float2>();
    m_tDesc.vUVSpeed_Op3 = file->Read<_float2>();
    m_tDesc.fContrast_Op3 = file->Read<_float>();
    m_tDesc.fAlphaOffset_Op3 = file->Read<_float>();

    /* Blend */
    m_tDesc.strTexture_Blend = file->Read<string>();
    m_tDesc.vTiling_Blend = file->Read<_float2>();
    m_tDesc.vUVSpeed_Blend = file->Read<_float2>();
    m_tDesc.fAlphaOffset_Blend = file->Read<_float>();

    /* Overlay */
    m_tDesc.bIsOverlayOn = file->Read<_bool>();
    m_tDesc.strOverlayTexture = file->Read<string>();
    m_tDesc.vBaseColor_Overlay = file->Read<_float4>();
    m_tDesc.vDestColor_Overlay = file->Read<_float4>();
    m_tDesc.vTiling_Overlay = file->Read<_float2>();
    m_tDesc.vUVSpeed_Overlay = file->Read<_float2>();

    /* Normal */
    m_tDesc.strNormalTexture = file->Read<string>();

    /* Dissolve */
    m_tDesc.strDissolveTexture = file->Read<string>();
    m_tDesc.vTiling_Dissolve = file->Read<_float2>();
    m_tDesc.vUVSpeed_Dissolve = file->Read<_float2>();
    m_tDesc.bInverseDissolve = file->Read<_bool>();

    /* Distortion */
    m_tDesc.strDistortionTexture = file->Read<string>();
    m_tDesc.vTiling_Distortion = file->Read<_float2>();
    m_tDesc.vUVSpeed_Distortion = file->Read<_float2>();

    // For. Load Transform_Desc 
    /* Init Position */
    m_tTransformDesc.vPosRange = file->Read<_float3>();

    /* Init scale */
    m_tTransformDesc.vInitScale_Min = file->Read<_float3>();
    m_tTransformDesc.vInitScale_Max = file->Read<_float3>();

    /* Init Rotation */
    m_tTransformDesc.vInitRotation_Min = file->Read<_float3>();
    m_tTransformDesc.vInitRotation_Max = file->Read<_float3>();

    /* Translate */
    m_tTransformDesc.iTranslateOption = file->Read<_int>();
    m_tTransformDesc.fTranslateSpeed = file->Read<_float>();
    m_tTransformDesc.vEndPosOffset_Min = file->Read<_float3>();
    m_tTransformDesc.vEndPosOffset_Max = file->Read<_float3>();

    /* Scaling */
    m_tTransformDesc.iScalingOption = file->Read<_int>();
    m_tTransformDesc.vEndScale = file->Read<_float3>();

    /* Turn */
    m_tTransformDesc.iTurnOption = file->Read<_int>();
    m_tTransformDesc.fTurnSpeed = file->Read<_float>();
    m_tTransformDesc.vRandomAxis_Min = file->Read<_float3>();
    m_tTransformDesc.vRandomAxis_Max = file->Read<_float3>();
}

void MeshEffectData::Set_Desc(DESC tDesc)
{
    m_tDesc = tDesc;
    m_wstrTag = Utils::ToWString(m_tDesc.pszTag);
}