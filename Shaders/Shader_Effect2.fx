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
    float fLifeTimeRatio = g_vec2_0.x / g_vec2_0.y;
    float4 vStartColor = g_vec4_0;
    float4 vDestColor = g_vec4_1;
    float4 vGradationColor = g_vec4_2;
    float fGradationIntensity = g_float_0;
    
    // For. Opacity Map
    if(bHasOpacityMap)
        output.a = OpacityMap.Sample(LinearSampler, input.uv).r;
    
    // For. Dissolve
    if(bHasDissolveMap)
    {
        float fDissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (fDissolve < fLifeTimeRatio)
            discard;
    }
        
    // For. Gradation with Gradation color + GradationIntensity
    
    // For. Gradation with Bright Intensity 
    
    // For. Gradation with Dark Intensity
    
    
    // For. Changing Color to DestColor 
    output = lerp(vStartColor, vDestColor, fLifeTimeRatio);
       
    return output;
}


technique11 T0 // 0
{
    pass pass_Default // 0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Main()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
};
