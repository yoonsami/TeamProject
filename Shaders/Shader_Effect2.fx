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
    int bIsInverseDissolve = g_int_3;
    
    float fLifeRatio = g_float_0;
    float fAlphaGraIntensity = g_float_1;
    float fContrast = g_float_2;
    float fUseSpriteAnim = g_float_3;
    
    float2 vTexUVOffset_Gra = { g_vec4_0.x, g_vec4_0.y };
    float2 vTexUVOffset_Overlay = { g_vec4_0.z, g_vec4_0.w };
    float2 vTexUVOffset_Dissolve = { g_vec4_1.x, g_vec4_1.y };
    float2 vTexUVOffset_Distortion = { g_vec4_1.z, g_vec4_1.w };
    
    float2 vTexUVOffset_Opacity = { 0.f, 0.f };
    float2 vTexUVOffset_Diffuse = { 0.f, 0.f };
    float2 vSpriteAnim_RangeX = { 0.f, 0.f };
    float2 vSpriteAnim_RangeY = { 0.f, 0.f };
    
    if (0.f == fUseSpriteAnim)
    {
        vTexUVOffset_Opacity = g_vec2_0;
        vTexUVOffset_Diffuse = float2(g_vec4_2.x, g_vec4_2.y);
    }
    else
    {
        vSpriteAnim_RangeX = float2(g_vec4_2.x, g_vec4_2.y);
        vSpriteAnim_RangeY = float2(g_vec4_2.z, g_vec4_2.w);
    }
    
    float4 vColor_Diffuse = g_mat_0._11_12_13_14;
    float4 vColor_AlphaGra = g_mat_0._21_22_23_24;
    float4 vColor_Gra = g_mat_0._31_32_33_34;
    float4 vColor_Overlay = g_mat_0._41_42_43_44;
    
    // Get Distortion weight 
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSampler, input.uv + vTexUVOffset_Distortion);
        fDistortionWeight = vDistortion.r * 0.5f;
    }
    
    // Calc Texcoord ( diffuse, opacity, dissolve, overlay, gradation )
    float2 Opacity_Texcoord = input.uv;
    float2 Diffuse_Texcoord = input.uv;
    if (1.f == fUseSpriteAnim)
    {
        Opacity_Texcoord = float2(vSpriteAnim_RangeX.x + ((vSpriteAnim_RangeX.y - vSpriteAnim_RangeX.x) * input.uv.x),
                                  vSpriteAnim_RangeY.x + ((vSpriteAnim_RangeY.y - vSpriteAnim_RangeY.x) * input.uv.y)) + fDistortionWeight;
        Diffuse_Texcoord = Opacity_Texcoord;
    }
    else if (0.f == fUseSpriteAnim)
    {
        Opacity_Texcoord = input.uv + vTexUVOffset_Opacity + fDistortionWeight;
        Diffuse_Texcoord = input.uv + vTexUVOffset_Diffuse + fDistortionWeight;
    }
    float2 Dissolve_Texcoord = input.uv + vTexUVOffset_Dissolve;
    float2 Gra_Textoord = input.uv + vTexUVOffset_Gra + fDistortionWeight;
    float2 Overlay_Textoord = input.uv + vTexUVOffset_Overlay;
    
    // Get Diffuse Color if has Diffuse map 
    if (bHasDiffuseMap)
        vOutColor = DiffuseMap.Sample(LinearSampler, Diffuse_Texcoord);
    else
        vOutColor = vColor_Diffuse;
    
    // Opacity * Blend + Get dissolve weight
    if (bHasOpacityMap)
    {
        if (0 == iSamplerType)
        {
            vOutColor.a = OpacityMap.Sample(LinearSampler, Opacity_Texcoord).r;
            if (bHasTexturemap9)
                vOutColor.a *= TextureMap9.Sample(LinearSampler, Opacity_Texcoord).r;
            if (bHasDissolveMap)
                fDissolveWeight = DissolveMap.Sample(LinearSampler, Dissolve_Texcoord).r;
        }
        else if (1 == iSamplerType)
        {
            vOutColor.a = OpacityMap.Sample(LinearSamplerClamp, Opacity_Texcoord).r;
            if (bHasTexturemap9)
                vOutColor.a *= TextureMap9.Sample(LinearSamplerClamp, Opacity_Texcoord).r;
            if (bHasDissolveMap)
                fDissolveWeight = DissolveMap.Sample(LinearSamplerClamp, Dissolve_Texcoord).r;
        }
        else if (2 == iSamplerType)
        {
            vOutColor.a = OpacityMap.Sample(LinearSamplerMirror, Opacity_Texcoord).r;
            if (bHasTexturemap9)
                vOutColor.a *= TextureMap9.Sample(LinearSamplerMirror, Opacity_Texcoord).r;
            if (bHasDissolveMap)
                fDissolveWeight = DissolveMap.Sample(LinearSamplerMirror, Dissolve_Texcoord).r;
        }
        else if (3 == iSamplerType)
        {
            vOutColor.a = OpacityMap.Sample(LinearSamplerBorder, Opacity_Texcoord).r;
            if (bHasTexturemap9)
                vOutColor.a *= TextureMap9.Sample(LinearSamplerBorder, Opacity_Texcoord).r;
            if (bHasDissolveMap)
                fDissolveWeight = DissolveMap.Sample(LinearSamplerBorder, Dissolve_Texcoord).r;
        }
    }
    
    // For. Dissolve
    if (bHasDissolveMap)
    {
        if (bIsInverseDissolve)
        {
            if (fDissolveWeight > sin(fLifeRatio))
                vOutColor.a = 0.f;
        }
        else
        {
            if (fDissolveWeight < sin(fLifeRatio))
                vOutColor.a = 0.f;
        }
    }
   
    // For. FadeOut
        if (bUseFadeOut)
            vOutColor.a *= (1.f - fLifeRatio);
    
    // For. Alpha Gradation 
        if (vOutColor.a < fAlphaGraIntensity)
            vOutColor.rgb = (vOutColor.rgb * vOutColor.a) + (vColor_AlphaGra.rgb * (1.f - vOutColor.a));
    
    // For. Gradation by texture
        if (bHasTexturemap7)
        {
            vColor_Gra.a = TextureMap7.Sample(LinearSamplerClamp, Gra_Textoord).r;
            vOutColor = lerp(vOutColor, vColor_Gra, vColor_Gra.a);
        }
    
    // For. Overlay
        if (bIsOverlayOn)
        {
            float fOverlayIntensity = 1.f;
            float4 vFinalOverlayColor = float4(0.f, 0.f, 0.f, 0.f);
        
            if (bHasTexturemap8)
                fOverlayIntensity = TextureMap8.Sample(LinearSamplerClamp, Overlay_Textoord).r;
        
            vFinalOverlayColor.r = (vOutColor.r <= 0.5) ? 2 * vOutColor.r * vColor_Overlay.r : 1 - 2 * (1 - vOutColor.r) * (1 - vColor_Overlay.r);
            vFinalOverlayColor.g = (vOutColor.g <= 0.5) ? 2 * vOutColor.g * vColor_Overlay.g : 1 - 2 * (1 - vOutColor.g) * (1 - vColor_Overlay.g);
            vFinalOverlayColor.b = (vOutColor.b <= 0.5) ? 2 * vOutColor.b * vColor_Overlay.b : 1 - 2 * (1 - vOutColor.b) * (1 - vColor_Overlay.b);
        
            vOutColor.rgb = lerp(vOutColor.rgb, vFinalOverlayColor.rgb, fOverlayIntensity);
        }
    
    // Color Edit 
        float luminance = dot(vOutColor.rgb, float3(0.299, 0.587, 0.114));
        vOutColor.rgb = lerp(vOutColor.rgb, vOutColor.rgb * fContrast, saturate(luminance));
    
        return vOutColor;
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
  