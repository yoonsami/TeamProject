#include "Render.fx"
#include "Light.fx"
// OutLine
float4 g_LineColor;
float g_LineThickness;
float4x4 g_preView;

struct MotionBlurOutput
{
    float4 position : SV_Position;
    float4 vDir : Position1;
};
struct OutlineOutput
{
    float4 diffuseColor : SV_Target3;
    float4 specularColor : SV_Target4;
    float4 emissiveColor : SV_Target5;
    float4 blurColor : SV_Target6;
};

// VS_Model
MeshOutput VS_NonAnim(VTXModel input)
{
    MeshOutput output;
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    

    return output;
}

MeshOutput VS_MapObject(VTXModel input)
{
    MeshOutput output;
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, VP);
    output.uv = input.uv * g_float_3;
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    

    return output;
}

MeshOutput VS_Anim(VTXModel input)
{
    MeshOutput output;
    
    matrix m = GetAnimationMatrix(input);

    output.position = mul(float4(input.position, 1.f), m);

    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) m);
    output.viewNormal = mul(output.viewNormal, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));

    return output;
}

MeshInstancingOutput VS_NonAnimInstancing(VTXModelInstancing input)
{
    MeshInstancingOutput output;
    
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, input.world);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    output.id = input.instanceID;
    
    return output;
}

MeshOutput VS_MapObject_Instancing(VTXModelInstancing input)
{
    MeshOutput output;
    
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, input.world);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

    output.position = mul(output.position, VP);
    output.uv = input.uv * InstanceRenderParams[input.instanceID].g_float_3;
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));

    return output;
}

MeshInstancingOutput VS_AnimInstancing(VTXModelInstancing input)
{
    MeshInstancingOutput output;

    matrix m = GetAnimationMatrix_Instance(input);
    
    output.position = mul(float4(input.position, 1.f), m);
    output.position = mul(output.position, input.world);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) m);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    output.id = input.instanceID;
    
    return output;
}

// VS_OutLine
MeshOutput VS_NonAnimOutline(VTXModel input)
{
    MeshOutput output;
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) W));
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
   // output.position += float4(normalize(output.viewNormal) * g_LineThickness, 0.f);
    
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, V);
    output.position += float4(normalize(output.viewNormal) * g_LineThickness * output.viewPosition.z, 0.f);
    output.position = mul(output.position, P);
    
    output.uv = input.uv;
    
   // output.viewPosition = output.viewPosition + output.viewNormal * g_LineThickness;

    return output;
}

MeshOutput VS_AnimOutline(VTXModel input)
{
    MeshOutput output;
    
    matrix m = GetAnimationMatrix(input);

    output.position = mul(float4(input.position, 1.f), m);
    output.position = mul(output.position, W);
    output.viewNormal = normalize(mul(input.normal, (float3x3) m));
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) W));
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));

   // output.position += float4(normalize(output.viewNormal) * g_LineThickness, 0.f);

    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, V);
    output.position += float4(normalize(output.viewNormal) * g_LineThickness * output.viewPosition.z, 0.f);
    output.position = mul(output.position, P);

    output.uv = input.uv;
    
   // output.viewPosition = output.viewPosition + output.viewNormal * g_LineThickness;
    
    return output;
}

MeshInstancingOutput VS_NonAnimInstancingOutline(VTXModelInstancing input)
{
    MeshInstancingOutput output;
    
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, input.world);
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    //output.position += float4(output.viewNormal * g_LineThickness, 0.f);
    
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, V);
    output.position += float4(normalize(output.viewNormal) * g_LineThickness * output.viewPosition.z, 0.f);
    output.position = mul(output.position, P);

    output.uv = input.uv;
    output.id = input.instanceID;
  //  output.viewPosition = output.viewPosition + output.viewNormal * g_LineThickness;
    return output;
}

MeshInstancingOutput VS_AnimInstancingOutline(VTXModelInstancing input)
{
    MeshInstancingOutput output;

    matrix m = GetAnimationMatrix_Instance(input);
    
    output.position = mul(float4(input.position, 1.f), m);
    output.position = mul(output.position, input.world);
    output.viewNormal = mul(input.normal, (float3x3) m);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    //output.position += float4(output.viewNormal * g_LineThickness, 0.f);

    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

        output.position = mul(output.position, V);
    output.position += float4(normalize(output.viewNormal) * g_LineThickness * output.viewPosition.z, 0.f);
    output.position = mul(output.position, P);
    output.uv = input.uv;
   // output.viewPosition = output.viewPosition + output.viewNormal * g_LineThickness;
    output.id = input.instanceID;
    
    return output;
}

// VS_Shadow
ShadowOutput VS_Shadow_NonAnim(VTXModel input)
{
    ShadowOutput output = (ShadowOutput) 0.f;
    
    output.pos = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.pos = mul(output.pos, W);
    output.worldPos = output.pos;
    output.pos = mul(output.pos, VP);
    output.clipPos = output.pos;
    output.uv = input.uv;
    return output;
}

ShadowOutput VS_Shadow_Anim(VTXModel input)
{
    ShadowOutput output = (ShadowOutput) 0.f;
    
    matrix m = GetAnimationMatrix(input);
    
    output.pos = mul(float4(input.position, 1.f), m);
    
    output.pos = mul(output.pos, W);
    output.worldPos = output.pos;
    output.pos = mul(output.pos, VP);
    output.clipPos = output.pos;
    output.uv = input.uv;
    return output;
}

ShadowInstanceOutput VS_Shadow_NonAnim_Instancing(VTXModelInstancing input)
{
    ShadowInstanceOutput output = (ShadowInstanceOutput) 0.f;
    
    output.pos = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.pos = mul(output.pos, input.world);
    output.worldPos = output.pos;
    output.pos = mul(output.pos, VP);
    output.clipPos = output.pos;
    output.uv = input.uv;
    output.id = input.instanceID;
    return output;
}

ShadowInstanceOutput VS_Shadow_Anim_Instancing(VTXModelInstancing input)
{
    ShadowInstanceOutput output = (ShadowInstanceOutput) 0.f;
    
    matrix m = GetAnimationMatrix_Instance(input);
    
    output.pos = mul(float4(input.position, 1.f), m);
    
    output.pos = mul(output.pos, input.world);
    output.worldPos = output.pos;
    output.pos = mul(output.pos, VP);
    output.clipPos = output.pos;
    output.uv = input.uv;
    output.id = input.instanceID;
    return output;
}

// VS_MotionBlur
MotionBlurOutput VS_NonAnimMotionBlur(VTXModel input)
{
    MotionBlurOutput output;
    
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.position = mul(output.position, V);
    float3 viewNormal = normalize(mul(input.normal, (float3x3) BoneTransform[BoneIndex]));
    viewNormal = normalize(mul(viewNormal, (float3x3) W));
    viewNormal = normalize(mul(viewNormal, (float3x3) V));
    float4 vOldPos = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    vOldPos = mul(vOldPos, preW);
    vOldPos = mul(vOldPos, g_preView);
    
    float4 vNewPos = output.position;
    
    float3 vDir = vNewPos.xyz - vOldPos.xyz;
    
    float a = dot(normalize(vDir), viewNormal);
    if (a < 0.f)
        output.position = vOldPos;
    else
        output.position = vNewPos;
    
    output.position = mul(output.position, P);
    
    //[-2~2]
    float2 velocity = (vNewPos.xy / vNewPos.w) - (vOldPos.xy / vOldPos.w);
    
    //[-1~1]
    velocity.xy *= 0.5f;
    velocity.y *= -1.f;
   // output.vDir.xy = (velocity + 2.f) * 0.25f;
    output.vDir.xy = velocity;
    output.vDir.z = output.position.z;
    output.vDir.w = output.position.w;
    
    return output;
}

MotionBlurOutput VS_AnimMotionBlur(VTXModel input)
{
    MotionBlurOutput output;
    
    matrix m = GetAnimationMatrix(input);
    matrix preM = GetPreAnimationMatrix(input);
    
    output.position = mul(float4(input.position, 1.f), m);
    output.position = mul(output.position, W);
    output.position = mul(output.position, V);
    
    float3 viewNormal = normalize(mul(input.normal, (float3x3) m));
    viewNormal = normalize(mul(viewNormal, (float3x3) W));
    viewNormal = normalize(mul(viewNormal, (float3x3) V));
    
    float4 vOldPos = mul(float4(input.position, 1.f), preM);
    vOldPos = mul(vOldPos, preW);
    vOldPos = mul(vOldPos, g_preView);
    
    float4 vNewPos = output.position;
    
    float3 vDir = vNewPos.xyz - vOldPos.xyz;
    
    float a = dot(normalize(vDir), viewNormal);
    if (a < 0.f)
        output.position = vOldPos;
    else
        output.position = vNewPos;
    
    output.position = mul(output.position, P);
    
   //[-2~2]
    float2 velocity = (vNewPos.xy / vNewPos.w) - (vOldPos.xy / vOldPos.w);
    
   //[-1~1]
    velocity.xy *= 0.5f;
    velocity.y *= -1.f;
   // output.vDir.xy = (velocity + 2.f) * 0.25f;
    output.vDir.xy = velocity;
    output.vDir.z = output.position.z;
    output.vDir.w = output.position.w;
    
    return output;
}

MotionBlurOutput VS_NonAnim_Instancing_MotionBlur(VTXModelInstancing input)
{
    MotionBlurOutput output;
    
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, input.world);
    output.position = mul(output.position, V);
    float3 viewNormal = normalize(mul(input.normal, (float3x3) BoneTransform[BoneIndex]));
    viewNormal = normalize(mul(viewNormal, (float3x3) input.world));
    viewNormal = normalize(mul(viewNormal, (float3x3) V));
    float4 vOldPos = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    vOldPos = mul(vOldPos, input.preWorld);
    vOldPos = mul(vOldPos, g_preView);
    
    float4 vNewPos = output.position;
    
    float3 vDir = vNewPos.xyz - vOldPos.xyz;
    
    float a = dot(normalize(vDir), viewNormal);
    if (a < 0.f)
        output.position = vOldPos;
    else
        output.position = vNewPos;
    
    output.position = mul(output.position, P);
    
    //[-2~2]
    float2 velocity = (vNewPos.xy / vNewPos.w) - (vOldPos.xy / vOldPos.w);
    
    //[-1~1]
    velocity.xy *= 0.5f;
    velocity.y *= -1.f;
   // output.vDir.xy = (velocity + 2.f) * 0.25f;
    output.vDir.xy = velocity;
    output.vDir.z = output.position.z;
    output.vDir.w = output.position.w;
    
    return output;
}

MotionBlurOutput VS_Anim_Instancing_MotionBlur(VTXModelInstancing input)
{
    MotionBlurOutput output;
    
    matrix m = GetAnimationMatrix_Instance(input);
    matrix preM = GetPreAnimationMatrix_Instance(input);
    
    output.position = mul(float4(input.position, 1.f), m);
    output.position = mul(output.position, input.world);
    output.position = mul(output.position, V);
    
    float3 viewNormal = normalize(mul(input.normal, (float3x3) m));
    viewNormal = normalize(mul(viewNormal, (float3x3) input.world));
    viewNormal = normalize(mul(viewNormal, (float3x3) V));
    
    float4 vOldPos = mul(float4(input.position, 1.f), preM);
    vOldPos = mul(vOldPos, input.preWorld);
    vOldPos = mul(vOldPos, g_preView);
    
    float4 vNewPos = output.position;
    
    float3 vDir = vNewPos.xyz - vOldPos.xyz;
    
    float a = dot(normalize(vDir), viewNormal);
    if (a < 0.f)
        output.position = vOldPos;
    else
        output.position = vNewPos;
    
    output.position = mul(output.position, P);
    
   //[-2~2]
    float2 velocity = (vNewPos.xy / vNewPos.w) - (vOldPos.xy / vOldPos.w);
    
    //[-1~1]
    velocity.xy *= 0.5f;
    velocity.y *= -1.f;
   // output.vDir.xy = (velocity + 2.f) * 0.25f;
    output.vDir.xy = velocity;
    output.vDir.z = output.position.z;
    output.vDir.w = output.position.w;
    
    return output;
}

// VS_Motion Trail
MeshOutput VS_Anim_MotionTrail(VTXModel input)
{
    MeshOutput output;
    
    matrix m = GetAnimationMatrix(input);

    output.position = mul(float4(input.position, 1.f), m);

    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) m);
    output.viewNormal = mul(output.viewNormal, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));

    return output;
}

MeshInstancingOutput VS_Anim_MotionTrail_Instancing(VTXModelInstancing input)
{
    MeshInstancingOutput output;

    matrix m = GetAnimationMatrix_Instance(input);
    
    output.position = mul(float4(input.position, 1.f), m);
    output.position = mul(output.position, input.world);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) m);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    output.id = input.instanceID;
    
    return output;
}

MeshOutput VS_NonAnim_MotionTrail(VTXModel input)
{
    MeshOutput output;
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    

    return output;
}

// PS_Model
PS_OUT_Deferred PS_Deferred(MeshOutput input)
{
    PS_OUT_Deferred output = (PS_OUT_Deferred) 0.f;
    float4 diffuseColor;
    float4 specularColor;
    float4 emissiveColor;
    
    float2 distortedUV = input.uv;
    
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < g_float_0)
            discard;
    }
    
    
    if (bHasNormalMap)
        ComputeNormalMapping_ViewSpace(input.viewNormal, input.viewTangent, input.uv);

    if (bHasDiffuseMap)
        diffuseColor = DiffuseMap.Sample(LinearSampler, input.uv);
    else
        diffuseColor = Material.diffuse;
    
    if(diffuseColor.a <= 0.1f)
        discard;

    diffuseColor.a = 1.f;
    
    if (bHasSpecularMap)
        specularColor = SpecularMap.Sample(LinearSampler, input.uv);
    else
        specularColor = Material.specular;
   
    if (bHasEmissiveMap)
        emissiveColor = EmissiveMap.Sample(LinearSampler, input.uv);
    else
        emissiveColor = Material.emissive;
    
    diffuseColor = diffuseColor;
    
    output.position = float4(input.viewPosition.xyz, 0.f);
    output.normal = float4(input.viewNormal.xyz, 0.f);
    output.depth = input.position.z;
    output.diffuseColor = diffuseColor;
    output.specularColor = specularColor;
    output.emissiveColor = emissiveColor;
    return output;
}

PS_OUT_Deferred PS_Deferred_Instancing(MeshInstancingOutput input)
{
    PS_OUT_Deferred output = (PS_OUT_Deferred) 0.f;
    float4 diffuseColor;
    float4 specularColor;
    float4 emissiveColor;
    
    float2 distortedUV = input.uv;
    
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < InstanceRenderParams[input.id].g_float_0)
            discard;
    }
    
    if (bHasNormalMap)
        ComputeNormalMapping_ViewSpace(input.viewNormal, input.viewTangent, input.uv);

    if (bHasDiffuseMap)
        diffuseColor = DiffuseMap.Sample(LinearSampler, input.uv);
    else
        diffuseColor = Material.diffuse;

    if (bHasSpecularMap)
        specularColor = SpecularMap.Sample(LinearSampler, input.uv);
    else
        specularColor = Material.specular;
    
    if (bHasEmissiveMap)
        emissiveColor = EmissiveMap.Sample(LinearSampler, input.uv);
    else
        emissiveColor = Material.emissive;

    diffuseColor = diffuseColor;
    
    output.position = float4(input.viewPosition.xyz, 0.f);
    output.normal = float4(input.viewNormal.xyz, 0.f);
        output.depth = input.position.z;
    output.diffuseColor = diffuseColor;
    output.specularColor = specularColor;
    output.emissiveColor = emissiveColor;
    return output;
}

// PS_OutLine
OutlineOutput PS_Deferred_Outline(MeshOutput input)
{
    OutlineOutput output = (OutlineOutput) 0.f;
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < g_float_0)
            discard;
    }
    output.diffuseColor = g_LineColor;
    output.specularColor = g_LineColor;
    output.emissiveColor = g_LineColor;
    output.blurColor = g_vec4_0;
    
    return output;
}

OutlineOutput PS_Deferred_Outline_Instancing(MeshInstancingOutput input)
{
    OutlineOutput output = (OutlineOutput) 0.f;
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < InstanceRenderParams[input.id].g_float_0)
            discard;
    }
    output.diffuseColor = g_LineColor;
    output.specularColor = g_LineColor;
    output.emissiveColor = g_LineColor;
    output.blurColor = InstanceRenderParams[input.id].g_vec4_0;
    return output;
}

// PS_Shadow
float4 PS_Shadow(ShadowOutput input) : SV_Target
{
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < g_float_0)
            discard;
    }
    return float4(input.clipPos.z / input.clipPos.w, 0.f, 0.f, 0.f);
}

float4 PS_ShadowInstancing(ShadowInstanceOutput input) : SV_Target
{
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < InstanceRenderParams[input.id].g_float_0)
            discard;
    }
    return float4(input.clipPos.z / input.clipPos.w, 0.f, 0.f, 0.f);
}

// PS_MotionBlur
float4 PS_MotionBlur(MotionBlurOutput input) : SV_Target
{
    float4 output = (float4) 0.f;
    
    output.xy = input.vDir.xy / input.vDir.w;
    output.z = 1.f;
    output.w = input.vDir.z / input.vDir.w;
    
    
    return output;
}

// PS_MotionTrail
float4 PS_MotionTrail(MeshOutput input) :SV_Target
{
    float4 output = (float4) 0.f;
    float4 diffuseColor;
    float4 emissiveColor = g_vec4_0;
    if (bHasNormalMap)
        ComputeNormalMapping_ViewSpace(input.viewNormal, input.viewTangent, input.uv);


    //diffuseColor = Material.diffuse;
    
    float3 eyeDir = normalize(input.viewPosition);
    
    output = emissiveColor * pow(smoothstep(0.f, 1.f, 1.f - saturate(dot(-eyeDir, input.viewNormal))), 2);
    
    return output;
}

float4 PS_MotionTrail_Instancing(MeshInstancingOutput input) : SV_Target
{
    float4 output = (float4) 0.f;
    float4 diffuseColor;
    float4 emissiveColor = InstanceRenderParams[input.id].g_vec4_0;
    if (bHasNormalMap)
        ComputeNormalMapping_ViewSpace(input.viewNormal, input.viewTangent, input.uv);


    //diffuseColor = Material.diffuse;
    
    float3 eyeDir = normalize(input.viewPosition);
    
    output = emissiveColor * pow(smoothstep(0.f, 1.f, 1.f - saturate(dot(-eyeDir, input.viewNormal))), 2);
    
    return output;
}

// PS_Debug
float4 PS_FRAME(VertexOutput input) : SV_TARGET
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    return color;
}

technique11 T0_ModelRender
{
    pass NonAnim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
    pass NonAnim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimInstancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Instancing()));
    }
    pass Anim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Anim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
    pass Anim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_AnimInstancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Instancing()));
    }
    pass NonAnim_NonInstancing_CullNone
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
    pass NonAnim_Instancing_CullNone
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimInstancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Instancing()));
    }
    pass NonAnimShadow
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Shadow_NonAnim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Shadow()));
    }
    pass NonAnimShadowInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Shadow_NonAnim_Instancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_ShadowInstancing()));
    }
    pass AnimShadow
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Shadow_Anim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Shadow()));
    }
    pass AnimShadowInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Shadow_Anim_Instancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_ShadowInstancing()));
    }
// 10. �ʿ�����Ʈ UV����, �ù���NONE, �����׽�Ʈ
    pass MapObject
    {
        SetVertexShader(CompileShader(vs_5_0, VS_MapObject()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
// 11. �ʿ�����Ʈ UV����, �ù���NONE, �����׽�Ʈ + �ν��Ͻ�
    pass MapObject_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_MapObject_Instancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }

    PASS_RS_VP(P4_WIREFRAME, FillModeWireFrame, VS_NonAnim, PS_FRAME)
    PASS_RS_VP(P6_WIREFRAME, FillModeWireFrame, VS_NonAnimInstancing, PS_FRAME)
};

technique11 T1_Outline
{
    pass NonAnim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimOutline()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CW);
        SetDepthStencilState(DSS_LESS_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Outline()));
    }
    pass NonAnim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimInstancingOutline()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CW);
        SetDepthStencilState(DSS_LESS_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Outline_Instancing()));
    }
    pass Anim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_AnimOutline()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CW);
        SetDepthStencilState(DSS_LESS_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Outline()));
    }
    pass Anim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_AnimInstancingOutline()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CW);
        SetDepthStencilState(DSS_LESS_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Outline_Instancing()));
    }
};

technique11 T2_MotionBlur
{
    pass NonAnimMotionBlur
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimMotionBlur()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_MotionBlur()));
    }
    pass NonAnim_Instancing_MotionBlur
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnim_Instancing_MotionBlur()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_MotionBlur()));
    }
    pass AnimMotionBlur
    {
        SetVertexShader(CompileShader(vs_5_0, VS_AnimMotionBlur()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_MotionBlur()));
    }
    pass Anim_Instancing_MotionBlur
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Anim_Instancing_MotionBlur()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_MotionBlur()));
    }
   
};

technique11 T3_MotionTrail
{
    pass Anim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Anim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_MotionTrail()));
    }
    pass NonAnim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_MotionTrail()));
    }
};