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
    float4 vOutColor = float4(0.f, 0.f, 0.f, 0.f);
    float fDistortionWeight = 0.f;
    float fDissolveWeight = 1.f;
    
    // For. Get Data from ganeral
    int iSamplerType = g_int_0;
    int bUseFadeOut = g_int_1;
    int bIsOverlayOn = g_int_2;
    float fLifeRatio = g_float_0;
    float fAlphaGraIntensity = g_float_1;
    
    float2 vTexUVOffset_Opacity = g_vec2_0;
    float2 vTexUVOffset_Gra = { g_vec4_0.x, g_vec4_0.y };
    float2 vTexUVOffset_Overlay = { g_vec4_0.z, g_vec4_0.w };
    float2 vTexUVOffset_Dissolve = { g_vec4_1.x, g_vec4_1.y };
    float2 vTexUVOffset_Distortion = { g_vec4_1.z, g_vec4_1.w };
    
    float4 vColor_Diffuse = g_mat_0._11;
    float4 vColor_AlphaGra = g_mat_0._11;
    float4 vColor_Gra = g_mat_0._31;
    float4 vColor_Overlay = g_mat_0._41;
    
    
    // Get Distortion weight 
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSampler, input.uv + vTexUVOffset_Distortion);
        fDistortionWeight = vDistortion.r * 0.5f;
    }
    
    // Get Diffuse Color if has Diffuse map 
    if (bHasDiffuseMap)
        vColor_Diffuse = DiffuseMap.Sample(LinearSampler, input.uv + fDistortionWeight).r;

    // Opacity + Get dissolve weight
    if (bHasOpacityMap)
    {
        if (0 == iSamplerType)
        {
            vOutColor.a = OpacityMap.Sample(LinearSampler, input.uv + vTexUVOffset_Opacity + fDistortionWeight).r;
            if (bHasDissolveMap)
                fDissolveWeight = DissolveMap.Sample(LinearSampler, input.uv + vTexUVOffset_Dissolve).r;
        }
        else if (1 == iSamplerType)
        {
            vOutColor.a = OpacityMap.Sample(LinearSamplerClamp, input.uv + vTexUVOffset_Opacity + fDistortionWeight).r;
            if (bHasDissolveMap)
                fDissolveWeight = DissolveMap.Sample(LinearSamplerClamp, input.uv + vTexUVOffset_Dissolve).r;
        }
        else if (2 == iSamplerType)
        {
            vOutColor.a = OpacityMap.Sample(LinearSamplerMirror, input.uv + vTexUVOffset_Opacity + fDistortionWeight).r;
            if (bHasDissolveMap)
                fDissolveWeight = DissolveMap.Sample(LinearSamplerMirror, input.uv + vTexUVOffset_Dissolve).r;
        }
        else if (3 == iSamplerType)
        {
            vOutColor.a = OpacityMap.Sample(LinearSamplerBorder, input.uv + vTexUVOffset_Opacity + fDistortionWeight).r;
            if (bHasDissolveMap)
                fDissolveWeight = DissolveMap.Sample(LinearSamplerBorder, input.uv + vTexUVOffset_Dissolve).r;
        }
    }
    
    // For. Dissolve
    if (fDissolveWeight < sin(fLifeRatio))
        discard;
   
    // For. FadeOut
    if (bUseFadeOut)
        vOutColor.a *= (1.f - fLifeRatio);
    
    // For. Alpha Gradation 
    if (vOutColor.a < fAlphaGraIntensity)
        vOutColor.rgb = (vOutColor.rgb * vOutColor.a) + (vColor_AlphaGra.rgb * (1.f - vOutColor.a));
    
     // For. Overlay
    if (bIsOverlayOn)
    {       
        float fOverlayWeight = 1.f;
        float4 vFinalOverlayColor = float4(0.f, 0.f, 0.f, 0.f);
        
        if (bHasTexturemap8)
            fOverlayWeight = DistortionMap.Sample(LinearSampler, input.uv + vTexUVOffset_Overlay);
        
        vFinalOverlayColor= (vOutColor.r <= 0.5) ? 2 * vOutColor.r * vColor_Overlay.r : 1 - 2 * (1 - vOutColor.r) * (1 - vColor_Overlay.r);
        vFinalOverlayColor= (vOutColor.g <= 0.5) ? 2 * vOutColor.g * vColor_Overlay.g : 1 - 2 * (1 - vOutColor.g) * (1 - vColor_Overlay.g);
        vFinalOverlayColor= (vOutColor.b <= 0.5) ? 2 * vOutColor.b * vColor_Overlay.b : 1 - 2 * (1 - vOutColor.b) * (1 - vColor_Overlay.b);
        
        vOutColor = vOutColor * (1.f - fOverlayWeight) + vFinalOverlayColor * fOverlayWeight;
    }
    
    return vOutColor;
}

technique11 T0 // 0
{
    pass pass_Clamp // 0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Main()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
};
  