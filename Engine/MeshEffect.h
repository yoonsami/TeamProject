#pragma once
#include "Component.h"
#include "MeshEffectData.h"

class MeshEffect : public Component
{
public:
    MeshEffect(shared_ptr<Shader> shader);
    virtual ~MeshEffect();

public:
    virtual void            Init(void* pArg);
    virtual void            Tick() override;
    virtual void            Final_Tick() override;
    void                    Render();

    void                    Update_Desc();

    void                    InitialTransform(_float4x4 mParentWorldMatrix, const _float3& vInitPos_inGroup, const _float3& vInitScale_inGroup, const _float3& vInitRotation_inGroup);

    /* Setter */
    void                    Set_Desc(MeshEffectData::DESC tDesc) { m_tDesc = tDesc; }
    void                    Set_TransformDesc(void* pArg);
    void                    Set_Lock(_bool bIsLocked) { m_bIsLocked = bIsLocked; }
    void                    Set_Loop(_bool bIsLoop) { m_tDesc.bIsLoop = bIsLoop; }

    /* Getter */
    shared_ptr<Material>    Get_Material() { return m_pMaterial; }
    shared_ptr<Shader>      Get_Shader() { return m_pShader; }
    _bool                   Get_IsFollowGroup_OnlyTranslate() { return m_tDesc.bIsFollowingGroup_OnlyTranslate; }
    _bool                   Get_IsFollowGroup_LooKSameDir() { return m_tDesc.bIsFollowingGroup_LooKSameDir; }
    _bool                   Get_IsLoop() { return m_tDesc.bIsLoop; }
    _float4x4               Get_InGroupMatrix() { return m_mInGroupWorldMatrix; }

    void                    Translate();
    void                    Scaling();
    void                    Turn();
    _float4x4               Get_LocalMatrix();
    void                    BillBoard();

private:
    void                    Run_SpriteAnimation();

    void                    Init_RenderParams();

    void                    Bind_UpdatedColor_ToShader();
    void                    Bind_UpdatedTexUVOffset_ToShader();
    void                    Bind_RenderParams_ToShader();

    _float                  CalcSpeed();
private:
    MeshEffectData::DESC            m_tDesc;
    MeshEffectData::Transform_Desc  m_tTransform_Desc;

    _float4x4                       m_mInGroupWorldMatrix;
    
    _float                          m_fDuration = { 0.f };
    _float                          m_fCurrAge = { 0.f };
    _float                          m_fLifeTimeRatio = { 0.f };
    _float                          m_fTimeAcc_SpriteAnimation = { 0.f };
    _bool                           m_bIsPlayFinished = { false };
    _bool                           m_bIsLocked = { false };

    /* Tex UV offset */
    _float2                         m_vCurrTexUVOffset_Op1 = { 0.f, 0.f };
    _float2                         m_vCurrTexUVOffset_Op2 = { 0.f, 0.f };
    _float2                         m_vCurrTexUVOffset_Op3 = { 0.f, 0.f };
    _float2                         m_vCurrTexUVOffset_Blend = { 0.f, 0.f };
    _float2                         m_vCurrTexUVOffset_Overlay = { 0.f, 0.f };
    _float2                         m_vCurrTexUVOffset_Dissolve = { 0.f, 0.f };
    _float2                         m_vCurrTexUVOffset_Distortion = { 0.f, 0.f };

    /* Spline */
    _float                          m_SplineInput_Dissolve[8];
    _float                          m_fCurrDissolveWeight = { 0.f };
    _float                          m_SplineInput_RimLight[8];
    _float                          m_fCurrRimLightIntensity = { 0.f };

    /* Sprite Animation */
    _float2                         m_vSpriteSize = { 0.f, 0.f };
    _int                            m_iNumSprite = { 0 };
    _int                            m_iCurrSpriteIndex = { 0 };
    _float2                         m_UVTexRangeX = { 0.f, 0.f };
    _float2                         m_UVTexRangeY = { 0.f, 0.f };

    /* Initalize Transform */
    _float3                         m_vStartPos;
    _float3                         m_vStartScale;
    _float3                         m_vStartRotation;

    /* Translate */
    _int                            m_iTranslateOption;
    _float                          m_fTranslateSpeed;
    _float3                         m_vEndPos;
    _float                          m_SplineInput_Force[8];
    _float                          m_fCurrYspeed = { 0.f };

    _float3                         m_vEndScale;

    _int                            m_iTurnOption;
    _float                          m_fTurnSpeed;
    _float3                         m_vRandomAxis;

    /* Component */
    shared_ptr<Shader>              m_pShader = { nullptr };
    shared_ptr<Mesh>                m_pMesh = { nullptr };
    shared_ptr<Model>               m_pModel = { nullptr };
    shared_ptr<Material>            m_pMaterial = { nullptr };

    /* Data to bind Shader */
    RenderParams                    m_RenderParams{};

    //
    _float3                         m_vLocalPos = _float3(0.f);
    _float3                         m_vLocalScale = _float3(1.f);
    Quaternion                      m_qRotation = Quaternion();


    _bool                           m_bToolMode_On = false;
};
