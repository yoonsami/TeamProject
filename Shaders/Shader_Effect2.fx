#include "Render.fx"
#include "Light.fx"

struct EffectOut
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION1;
    float3 viewPosition : POSITION2;
    float2 uv : TEXCOORD;
    float2 originUV : TEXCOORD1;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
};

EffectOut VS_Main(VTXModel input)
{
    EffectOut output;
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, VP);
    output.uv = input.uv + uvSlidingAcc;
    output.originUV = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) W);
    output.viewNormal = mul(output.viewNormal, (float3x3) V);
    
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = mul(output.viewTangent, (float3x3) V);

    return output;
}

float4 PS_Main(EffectOut input) : SV_Target
{
    float4 output = (float4) 0.f;
    
    int iOverlayOn = g_int_0;
    int iFadeOutOn = g_int_1;
    int iGradationOn = g_int_2;
    
    float fLifeTimeRatio = g_float_0;
    float fGradationIntensity = g_float_1;
    
    float fOpacityMap_TexUVOffset = g_vec2_0;
    
    float4 vBaseColor = g_vec4_0;
    float4 vGradationColor = g_vec4_1;
    float4 vOverlayColor_Start = g_vec4_2;
    float4 vOverlayColor_End = g_vec4_3;
    
    float4 vOverlay = vOverlayColor_Start;
    output = vBaseColor;
    
     // For. Distortion
    float fDistortionWeight = 0.f;
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSampler, input.uv);
        fDistortionWeight = vDistortion.r * 0.5f;
    }
    
    // For. Opacity Map
    if (bHasOpacityMap)
    {
        output.a = OpacityMap.Sample(LinearSampler, input.uv + fDistortionWeight).r;
        vOverlay.a = OpacityMap.Sample(LinearSampler, input.uv + fDistortionWeight).r;
    }
   
     // For. Dissolve
    if (bHasDissolveMap)
    {
        float fDissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (fDissolve < sin(fLifeTimeRatio))
            discard;
    }
    
    // For. FadeOut
    if (iFadeOutOn)
        output.a *= (1.f - fLifeTimeRatio);
    
    // For. Gradation 
    if (1 == iGradationOn && output.a < fGradationIntensity)
        output.rgb = (output.rgb * output.a) + (vGradationColor.rgb * (1.f - output.a));

    // For. Overlay
    if (iOverlayOn)
    {
        output.r = (output.r <= 0.5) ? 2 * output.r * vOverlay.r : 1 - 2 * (1 - output.r) * (1 - vOverlay.r);
        output.g = (output.g <= 0.5) ? 2 * output.g * vOverlay.g : 1 - 2 * (1 - output.g) * (1 - vOverlay.g);
        output.b = (output.b <= 0.5) ? 2 * output.b * vOverlay.b : 1 - 2 * (1 - output.b) * (1 - vOverlay.b);
    }
    
    return output;
}


technique11 T0 // 0
{
    pass pass_Default // 0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Main()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
};
  