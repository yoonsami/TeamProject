#include "pch.h"
#include "MeshEffect.h"

#include "Model.h"
#include "Utils.h"
#include "MathUtils.h"
#include "Camera.h"
#include "ModelMesh.h"

MeshEffect::MeshEffect(shared_ptr<Shader> shader)
    : Component(COMPONENT_TYPE::MeshEffect)
    , m_pShader(shader)
{

}

MeshEffect::~MeshEffect()
{
}

void MeshEffect::Init(void* pArg)
{
    // For. Setting basic info  
    MeshEffectData::DESC* pDesc = (MeshEffectData::DESC*)pArg;
    m_tDesc = *pDesc;

    Update_Desc();
}

void MeshEffect::Tick()
{
    // For. Transform
    Translate();
    Scaling();
    Turn();
}

void MeshEffect::Final_Tick()
{
    if (m_bIsPlayFinished)
        return;
    
    m_fCurrAge += fDT;
    m_fLifeTimeRatio = m_fCurrAge / m_tDesc.fDuration;
    m_fTimeAcc_SpriteAnimation += fDT;

    if ("None" != m_tDesc.strTexture_Blend)
    {
        _float output[4];
        Utils::Spline(m_SplineInput_Dissolve, 4, 1, m_fLifeTimeRatio, output);
        m_fCurrDissolveWeight = output[0];
    }

    // For. Check is dead 
    if (m_fCurrAge >= m_fDuration)
    {
        CUR_SCENE->Remove_GameObject(Get_Owner());
        return;
    }
}

void MeshEffect::Render()
{
    if (m_pModel == nullptr)
        return;

    // Bones
    shared_ptr<BoneDesc> boneDesc = make_shared<BoneDesc>();

    const _uint boneCount = m_pModel->Get_BoneCount();

    for (_uint i = 0; i < boneCount; ++i)
    {
        shared_ptr<ModelBone> bone = m_pModel->Get_BoneByIndex(i);
        boneDesc->transform[i] = bone->transform *Utils::m_matPivot;
    }
    m_pShader->Push_BoneData(*boneDesc);

    // For. Bind data to shader 
    auto world = Get_Transform()->Get_WorldMatrix();
    m_pShader->Push_TransformData(TransformDesc{ world });

    m_pShader->Push_GlobalData(Camera::Get_View(), Camera::Get_Proj());
    Bind_RenderParams_ToShader();

    // For. Draw meshes 
    const auto& meshes = m_pModel->Get_Meshes();
    for (auto& mesh : meshes)
    {
        // For. Material tick 
        if (!mesh->material.expired())
        {
            mesh->vertexBuffer->Push_Data();
            mesh->indexBuffer->Push_Data();
        }
        m_pMaterial->Tick();

        m_pShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

        // For. Setting Context Topology
        CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // For. Draw call
        m_pShader->DrawIndexed(0, m_tDesc.iSamplerType, mesh->indexBuffer->Get_IndicesNum(), 0, 0);
    }
}

void MeshEffect::Update_Desc()
{
    m_fDuration = MathUtils::Get_RandomFloat(m_tDesc.vParticleDuration.x, m_tDesc.vParticleDuration.y);

    Init_RenderParams();

    if (m_tDesc.bUseSpriteAnim)
    {
        m_vSpriteSize.x = 1.f / m_tDesc.iNumSprite_Col;
        m_vSpriteSize.y = 1.f / m_tDesc.iNumSprite_Row;
        m_iCurrSpriteIndex = -1;
        m_iNumSprite = m_tDesc.iNumSprite_Col * m_tDesc.iNumSprite_Row;
        m_UVTexRangeX = _float2(0.f, m_vSpriteSize.x);
        m_UVTexRangeY = _float2(0.f, m_vSpriteSize.y);
    }    
    else
    {
        m_vCurrTexUVOffset_Op1 = m_tDesc.vTiling_Op1;
        m_vCurrTexUVOffset_Op2 = m_tDesc.vTiling_Op2;
        m_vCurrTexUVOffset_Op3 = m_tDesc.vTiling_Op3;
    }
    m_vCurrTexUVOffset_Blend = m_tDesc.vTiling_Blend;
    m_vCurrTexUVOffset_Overlay = m_tDesc.vTiling_Overlay;
    m_vCurrTexUVOffset_Dissolve = m_tDesc.vTiling_Dissolve;
    m_vCurrTexUVOffset_Distortion = m_tDesc.vTiling_Distortion;
    
    // For. Init Spline input
    for (int i = 0; i < 4; ++i) {
        // Dissolve 
        m_SplineInput_Dissolve[i * 2 + 0] = m_tDesc.vCurvePoint_Dissolve[i].x;
        m_SplineInput_Dissolve[i * 2 + 1] = m_tDesc.vCurvePoint_Dissolve[i].y;
    }


    // For. Model Components
    m_pModel = RESOURCES.Get<Model>(Utils::ToWString(m_tDesc.strVfxMesh));

    // For. Material Components
    m_pMaterial = make_shared<Material>();
    m_pMaterial->Set_Shader(m_pShader);

    wstring wstrKey = Utils::ToWString(m_tDesc.strTexture_Op1);
    wstring wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
    if (TEXT("None") != wstrKey)
        m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE7);
    
    wstrKey = Utils::ToWString(m_tDesc.strTexture_Op2);
    wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
    if (TEXT("None") != wstrKey)
        m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE8);
    
    wstrKey = Utils::ToWString(m_tDesc.strTexture_Op3);
    wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
    if (TEXT("None") != wstrKey)
        m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE9);
    
    wstrKey = Utils::ToWString(m_tDesc.strTexture_Blend);
    wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
    if (TEXT("None") != wstrKey)
        m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE10);
    
    wstrKey = Utils::ToWString(m_tDesc.strOverlayTexture);
    wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
    if (TEXT("None") != wstrKey)
        m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::TEXTURE11);
    
    wstrKey = Utils::ToWString(m_tDesc.strDissolveTexture);
    wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
    if (TEXT("None") != wstrKey)
        m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::DISSOLVE);   
    
    wstrKey = Utils::ToWString(m_tDesc.strDistortionTexture);
    wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
    if (TEXT("None") != wstrKey)
        m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::DISTORTION);   
    
    wstrKey = Utils::ToWString(m_tDesc.strNormalTexture);
    wstrPath = TEXT("../Resources/Textures/Universal/") + wstrKey;
    if (TEXT("None") != wstrKey)
        m_pMaterial->Set_TextureMap(RESOURCES.Load<Texture>(wstrKey, wstrPath), TextureMapType::NORMAL);   
}

void MeshEffect::InitialTransform()
{
    // For. Position, Scale, Rotation 
    m_vStartPos += _float3(Get_Transform()->Get_State(Transform_State::POS));
    m_vStartScale *= Get_Transform()->Get_Scale();

    m_vEndScale += m_vStartScale;
    m_vEndPos += m_vStartPos;

    Get_Transform()->Set_State(Transform_State::POS, _float4(m_vStartPos, 1.f));
    Get_Transform()->Scaled(m_vStartScale);
    Get_Transform()->Set_Rotation(m_vStartRotation);
    Get_Transform()->Set_Quaternion(Quaternion::CreateFromRotationMatrix(_float4x4::CreateRotationY(XM_PI * 0.5f) * _float4x4::CreateFromQuaternion(Get_Transform()->Get_Rotation())));
}

void MeshEffect::Set_TransformDesc(void* pArg)
{
    // For. Setting basic info  
    MeshEffectData::Transform_Desc* pDesc = (MeshEffectData::Transform_Desc*)pArg;
    m_tTransform_Desc = *pDesc;

    // For. Initial Transform 
    m_vStartPos = _float3(
        MathUtils::Get_RandomFloat(pDesc->vPosRange.x / 2.f * (-1.f), pDesc->vPosRange.x / 2.f),
        MathUtils::Get_RandomFloat(pDesc->vPosRange.y / 2.f * (-1.f), pDesc->vPosRange.y / 2.f),
        MathUtils::Get_RandomFloat(pDesc->vPosRange.z / 2.f * (-1.f), pDesc->vPosRange.z / 2.f)
    );

    m_vStartScale = _float3(
        MathUtils::Get_RandomFloat(pDesc->vInitScale_Min.x, pDesc->vInitScale_Max.x),
        MathUtils::Get_RandomFloat(pDesc->vInitScale_Min.y, pDesc->vInitScale_Max.y),
        MathUtils::Get_RandomFloat(pDesc->vInitScale_Min.z, pDesc->vInitScale_Max.z)
    );

    m_vStartRotation = _float3(
        MathUtils::Get_RandomFloat(pDesc->vInitRotation_Min.x, pDesc->vInitRotation_Max.x),
        MathUtils::Get_RandomFloat(pDesc->vInitRotation_Min.y, pDesc->vInitRotation_Max.y),
        MathUtils::Get_RandomFloat(pDesc->vInitRotation_Min.z, pDesc->vInitRotation_Max.z)
    );
    m_vStartRotation.x *= 3.141592f / 180.f;
    m_vStartRotation.y *= 3.141592f / 180.f;
    m_vStartRotation.z *= 3.141592f / 180.f;

    // For. Translate
    m_iTranslateOption = pDesc->iTranslateOption;
    m_fTranslateSpeed = pDesc->fTranslateSpeed;
    m_vEndPos = m_vStartPos + _float3(
        MathUtils::Get_RandomFloat(pDesc->vEndPosOffset_Min.x, pDesc->vEndPosOffset_Max.x),
        MathUtils::Get_RandomFloat(pDesc->vEndPosOffset_Min.y, pDesc->vEndPosOffset_Max.y),
        MathUtils::Get_RandomFloat(pDesc->vEndPosOffset_Min.z, pDesc->vEndPosOffset_Max.z)
    );

    m_vEndScale = pDesc->vEndScale;

    m_iTurnOption = pDesc->iTurnOption;
    m_fTurnSpeed = pDesc->fTurnSpeed;
    m_vRandomAxis = _float3(
        MathUtils::Get_RandomFloat(pDesc->vRandomAxis_Min.x, pDesc->vRandomAxis_Max.x),
        MathUtils::Get_RandomFloat(pDesc->vRandomAxis_Min.y, pDesc->vRandomAxis_Max.y),
        MathUtils::Get_RandomFloat(pDesc->vRandomAxis_Min.z, pDesc->vRandomAxis_Max.z)
    );

    // For. Init Spline input
    for (int i = 0; i < 4; ++i) {
        // Force 
        m_SplineInput_Force[i * 2 + 0] = m_tTransform_Desc.vCurvePoint_Force[i].x;
        m_SplineInput_Force[i * 2 + 1] = m_tTransform_Desc.vCurvePoint_Force[i].y;
    }
}

void MeshEffect::Translate()
{
    switch (m_iTranslateOption)
    {
    case 0:
        break;
    case 1: // Move to target position
    case 2: // Move to random target position
        Get_Transform()->Set_State(Transform_State::POS, _float4(XMVectorLerp(m_vStartPos, m_vEndPos, m_fCurrAge), 0.f));
        break;
    case 3:
        Get_Transform()->Go_Straight();
        break;
    case 4:
        Get_Transform()->Go_Backward();
        break;
    case 5:
        Get_Transform()->Go_Left();
        break;
    case 6:
        Get_Transform()->Go_Right();
        break;
    case 7: // Fountain
    {
        _float output[4];
        Utils::Spline(m_SplineInput_Force, 4, 1, m_fLifeTimeRatio, output);
        _float4 vPos = Get_Transform()->Get_State(Transform_State::POS);
        Get_Transform()->Set_State(Transform_State::POS, _float4(vPos.x, vPos.y + output[0] * fDT, vPos.z, 1.f));
    }
        break;
    case 8:
        // TODO: Scattered embers
        break;
    }
}

void MeshEffect::Scaling()
{
    _float3 vScale = XMVectorLerp(m_vStartScale, m_vEndScale, m_fCurrAge);
    Get_Transform()->Scaled(vScale);
}

void MeshEffect::Turn()
{
    if (m_vRandomAxis.x == 0.f && m_vRandomAxis.y == 0.f && m_vRandomAxis.z == 0.f)
        return;
    Get_Transform()->Turn(m_vRandomAxis, m_fTurnSpeed);
}

void MeshEffect::Run_SpriteAnimation()
{
    if (m_fTimeAcc_SpriteAnimation > m_tDesc.fAnimSpeed)
    {
        m_iCurrSpriteIndex++;

        if (m_iCurrSpriteIndex >= m_iNumSprite)
            m_iCurrSpriteIndex = 0;

        _int iIndexX = m_iCurrSpriteIndex % m_tDesc.iNumSprite_Col;
        _int iIndexY = m_iCurrSpriteIndex / m_tDesc.iNumSprite_Col;
        m_UVTexRangeX.x = (_float)iIndexX / (_float)m_tDesc.iNumSprite_Col;
        m_UVTexRangeX.y = ((_float)iIndexX + 1) / (_float)m_tDesc.iNumSprite_Col;
        m_UVTexRangeY.x = (_float)iIndexY / (_float)m_tDesc.iNumSprite_Row;
        m_UVTexRangeY.y = ((_float)iIndexY + 1) / (_float)m_tDesc.iNumSprite_Row;

        m_fTimeAcc_SpriteAnimation = 0.f;
    }
}

void MeshEffect::Init_RenderParams()
{
    _float2 vTemp2;
    _float4 vTemp4;
    _float4x4 vTemp4x4;
    
    /* Int */
    m_RenderParams.SetInt(0, m_tDesc.bUseFadeOut);
    
    m_RenderParams.SetInt(2, (_int)m_tDesc.bUseSpriteAnim);
    
    /* Float */
    m_RenderParams.SetFloat(0, m_fCurrAge / m_tDesc.fDuration);
    m_RenderParams.SetFloat(1, m_fCurrDissolveWeight);

    /* Float2 */
    vTemp2 = _float2(m_tDesc.fContrast_Op1, m_tDesc.fAlphaOffset_Op1);
    m_RenderParams.SetVec2(0, vTemp2);
    vTemp2 = _float2(m_tDesc.fContrast_Op2, m_tDesc.fAlphaOffset_Op2);
    m_RenderParams.SetVec2(1, vTemp2);
    vTemp2 = _float2(m_tDesc.fContrast_Op3, m_tDesc.fAlphaOffset_Op3);
    m_RenderParams.SetVec2(2, vTemp2);
    vTemp2 = _float2(m_vCurrTexUVOffset_Distortion.x, m_vCurrTexUVOffset_Distortion.y);
    m_RenderParams.SetVec2(3, vTemp2);

    /* Float4 */
    vTemp4 = _float4(m_vCurrTexUVOffset_Op1.x, m_vCurrTexUVOffset_Op1.y, m_vCurrTexUVOffset_Op2.x, m_vCurrTexUVOffset_Op2.y);
    m_RenderParams.SetVec4(0, vTemp4);
    vTemp4 = _float4(m_vCurrTexUVOffset_Op3.x, m_vCurrTexUVOffset_Op3.y, m_vCurrTexUVOffset_Blend.x, m_vCurrTexUVOffset_Blend.y);
    m_RenderParams.SetVec4(1, vTemp4);
    vTemp4 = _float4(m_vCurrTexUVOffset_Overlay.x, m_vCurrTexUVOffset_Overlay.y, m_vCurrTexUVOffset_Dissolve.x, m_vCurrTexUVOffset_Dissolve.y);
    m_RenderParams.SetVec4(2, vTemp4);
    vTemp4 = _float4(m_UVTexRangeX.x, m_UVTexRangeX.y, m_UVTexRangeY.x, m_UVTexRangeY.y);
    m_RenderParams.SetVec4(3, vTemp4);

    /* Mat4x4 */
    vTemp4x4 = _float4x4(
        m_tDesc.vBaseColor1_Op1, 
        m_tDesc.vBaseColor2_Op1,
        m_tDesc.vBaseColor1_Op2,
        m_tDesc.vBaseColor2_Op2
    );
    m_RenderParams.SetMatrix(0, vTemp4x4);
    vTemp4x4 = _float4x4(
        m_tDesc.vBaseColor1_Op3,
        m_tDesc.vBaseColor2_Op3,
        m_tDesc.vBaseColor_Overlay,
        _float4((_float)m_tDesc.bIsUseTextureColor_Op1, (_float)m_tDesc.bIsUseTextureColor_Op2, (_float)m_tDesc.bIsUseTextureColor_Op3, 0.f)
    );
    m_RenderParams.SetMatrix(1, vTemp4x4);
}

void MeshEffect::Bind_UpdatedColor_ToShader()
{
    _float4x4 vTemp4x4;

    Color vFinalColor1_Op1 = XMVectorLerp(m_tDesc.vBaseColor1_Op1, m_tDesc.vDestColor1_Op1, m_fLifeTimeRatio);
    Color vFinalColor2_Op1 = XMVectorLerp(m_tDesc.vBaseColor2_Op1, m_tDesc.vDestColor2_Op1, m_fLifeTimeRatio);
    Color vFinalColor1_Op2 = XMVectorLerp(m_tDesc.vBaseColor1_Op2, m_tDesc.vDestColor1_Op2, m_fLifeTimeRatio);
    Color vFinalColor2_Op2 = XMVectorLerp(m_tDesc.vBaseColor2_Op2, m_tDesc.vDestColor2_Op2, m_fLifeTimeRatio);
    Color vFinalColor1_Op3 = XMVectorLerp(m_tDesc.vBaseColor1_Op3, m_tDesc.vDestColor1_Op3, m_fLifeTimeRatio);
    Color vFinalColor2_Op3 = XMVectorLerp(m_tDesc.vBaseColor2_Op3, m_tDesc.vDestColor2_Op3, m_fLifeTimeRatio);
    Color vFinalColor_Overlay = XMVectorLerp(m_tDesc.vBaseColor_Overlay, m_tDesc.vDestColor_Overlay, m_fLifeTimeRatio);
    vTemp4x4 = _float4x4(
        vFinalColor1_Op1,
        vFinalColor2_Op1,
        vFinalColor1_Op2,
        vFinalColor2_Op2
    );
    m_RenderParams.SetMatrix(0, vTemp4x4);
    vTemp4x4 = _float4x4(
        vFinalColor1_Op3,
        vFinalColor2_Op3,
        vFinalColor_Overlay,
        _float4((_float)m_tDesc.bIsUseTextureColor_Op1, (_float)m_tDesc.bIsUseTextureColor_Op2, (_float)m_tDesc.bIsUseTextureColor_Op3, 0.f)
    );
    m_RenderParams.SetMatrix(1, vTemp4x4);
}

void MeshEffect::Bind_UpdatedTexUVOffset_ToShader()
{
    _float2 vTemp2;
    _float4 vTemp4;

    // For. Update UV Offset
    if (m_tDesc.bUseSpriteAnim)
        Run_SpriteAnimation();
    else
    {
        m_vCurrTexUVOffset_Op1 += m_tDesc.vUVSpeed_Op1 * fDT;
        m_vCurrTexUVOffset_Op2 += m_tDesc.vUVSpeed_Op2 * fDT;
        m_vCurrTexUVOffset_Op3 += m_tDesc.vUVSpeed_Op3 * fDT;
        m_vCurrTexUVOffset_Blend += m_tDesc.vUVSpeed_Blend * fDT;
    }
    m_vCurrTexUVOffset_Overlay += m_tDesc.vUVSpeed_Overlay * fDT;
    m_vCurrTexUVOffset_Dissolve += m_tDesc.vUVSpeed_Dissolve * fDT;
    m_vCurrTexUVOffset_Distortion += m_tDesc.vUVSpeed_Distortion * fDT;

    vTemp2 = _float2(m_vCurrTexUVOffset_Distortion.x, m_vCurrTexUVOffset_Distortion.y);
    m_RenderParams.SetVec2(3, vTemp2);
    vTemp4 = _float4(m_vCurrTexUVOffset_Op1.x, m_vCurrTexUVOffset_Op1.y, m_vCurrTexUVOffset_Op2.x, m_vCurrTexUVOffset_Op2.y);
    m_RenderParams.SetVec4(0, vTemp4);
    vTemp4 = _float4(m_vCurrTexUVOffset_Op3.x, m_vCurrTexUVOffset_Op3.y, m_vCurrTexUVOffset_Blend.x, m_vCurrTexUVOffset_Blend.y);
    m_RenderParams.SetVec4(1, vTemp4);
    vTemp4 = _float4(m_vCurrTexUVOffset_Overlay.x, m_vCurrTexUVOffset_Overlay.y, m_vCurrTexUVOffset_Dissolve.x, m_vCurrTexUVOffset_Dissolve.y);
    m_RenderParams.SetVec4(2, vTemp4);
    vTemp4 = _float4(m_UVTexRangeX.x, m_UVTexRangeX.y, m_UVTexRangeY.x, m_UVTexRangeY.y);
    m_RenderParams.SetVec4(3, vTemp4);
}

void MeshEffect::Bind_RenderParams_ToShader()
{
    m_RenderParams.SetFloat(0, m_fLifeTimeRatio);    
    m_RenderParams.SetFloat(1, m_fCurrDissolveWeight);

    Bind_UpdatedColor_ToShader();
    Bind_UpdatedTexUVOffset_ToShader();

    // For. Bind Data 
    m_pShader->Push_RenderParamData(m_RenderParams);
}