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
    int iChangingColorOn = g_int_1;
    int iFadeOutOn = g_int_2;
    
    float fLifeTimeRatio = g_float_0;
    float fGradationIntensity = g_float_1;
    
    float4 vDestBaseColor = float4(g_vec2_0.x, g_vec2_0.y, g_vec2_1.x, g_vec2_1.y );
    float4 vDestGradationColor = float4(g_vec2_2.x, g_vec2_2.y, g_vec2_3.x, g_vec2_3.y);
    
    float4 vBaseColor = g_vec4_0;
    float4 vGradationColor = g_vec4_1;
    float4 vOverlayColor_Start = g_vec4_2;
    float4 vOverlayColor_End = g_vec4_3;
    
    if (iChangingColorOn)
    {
        vBaseColor = lerp(vBaseColor, vDestBaseColor, fLifeTimeRatio);
        vGradationColor = lerp(vGradationColor, vDestBaseColor, fLifeTimeRatio);
    }
    
     // For. Dissolve
    if (bHasDissolveMap)
    {
        float fDissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (fDissolve < fLifeTimeRatio)
            discard;
    }
    
    // For. Base 
    output = vBaseColor;
    
    // For. Opacity Map
    if(bHasOpacityMap)
        output.a = OpacityMap.Sample(LinearSampler, input.uv).r;
    
    // For. FadeOut
    if (iFadeOutOn)
        output.a *= fLifeTimeRatio;
    
    // For. Gradation 
    if (output.a <= fGradationIntensity)
        output.rgb = output.rgb * output.a + vDestBaseColor.rgb * (1.f - output.a);
    
    // For. Overlay
    if (iOverlayOn)
    {
        if (output.a < 0.5f)
        {
            float4 vOverlay = lerp(vOverlayColor_Start, vOverlayColor_End, output.a * 2.f);
            output.rgb = 2.f * vBaseColor.rgb * vOverlay.rgb;
        }
        else
            output.rgb = (1.0 - 2.0 * (1.0 - vBaseColor.rgb) * (1.0 - vOverlayColor_Start.rgb));
    }
       
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
