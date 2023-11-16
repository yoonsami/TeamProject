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

float4 PS_Wrap(EffectOut input) : SV_Target
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
    float2 vTexUVOffset_Blend = { g_vec4_3.x, g_vec4_3.y };
    float2 vTexUVOffset_Additional = { g_vec4_3.z, g_vec4_3.w };
    
    float fDefinition = g_mat_1._11;
    
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
    float4 vColor_DiffuseGradation = g_mat_1._31_32_33_34;
    float4 vColor_AlphaGra = g_mat_0._21_22_23_24;
    float4 vColor_Gra = g_mat_0._31_32_33_34;
    float4 vColor_Overlay = g_mat_0._41_42_43_44;
    float4 vColor_Additional = g_mat_1._21_22_23_24;
    
    // Get Distortion weight 
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSampler, input.uv + vTexUVOffset_Distortion);
        fDistortionWeight = vDistortion.r * 0.5f;
    }
    
    // Calc Texcoord ( diffuse, opacity, dissolve, overlay, gradation, Additional )
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
    float2 Blend_Texcoord = input.uv + vTexUVOffset_Blend;
    float2 Dissolve_Texcoord = input.uv + vTexUVOffset_Dissolve;
    float2 Gra_Texcoord = input.uv + vTexUVOffset_Gra + fDistortionWeight;
    float2 Overlay_Texcoord = input.uv + vTexUVOffset_Overlay;
    float2 Additional_Texcoord = input.uv + vTexUVOffset_Additional;    

    // For. Diffuse
    float4 vFianlDiffuse = vColor_Diffuse;
    if (bHasDiffuseMap)
    {
        vFianlDiffuse = DiffuseMap.Sample(LinearSampler, Opacity_Texcoord);
        vFianlDiffuse.rgb = lerp(vColor_DiffuseGradation.rgb, vFianlDiffuse.rgb, DiffuseMap.Sample(LinearSampler, Opacity_Texcoord).r);
    }
        
    // For. Opacity 
    vFianlDiffuse.a = OpacityMap.Sample(LinearSampler, Opacity_Texcoord).a;
   
    // For. Blend 
    if (bHasTexturemap9)
        vFianlDiffuse.a *= TextureMap9.Sample(LinearSampler, Blend_Texcoord).a;
    
    // For. Dissolve
    if (bHasDissolveMap)
    {
        fDissolveWeight = DissolveMap.Sample(LinearSampler, Dissolve_Texcoord).r;
        if (bIsInverseDissolve)
        {
            if (fDissolveWeight > sin(fLifeRatio))
                vFianlDiffuse.a = 0.f;
        }
        else
        {
            if (fDissolveWeight < sin(fLifeRatio))
                vFianlDiffuse.a = 0.f;
        }
    }
    
    // For. Final + Alpha Gradation 
    if (vFianlDiffuse.a < fAlphaGraIntensity)
        vFianlDiffuse.rgb = lerp(vColor_AlphaGra.rgb, vFianlDiffuse.rgb, vFianlDiffuse.a);

    // For. Additional (Base)
    if (bHasTexturemap10)
    {
        float4 vAdditional1_Sampled = vColor_Additional;
        vAdditional1_Sampled = vColor_Additional * TextureMap10.Sample(LinearSampler, Additional_Texcoord).a;
        vOutColor = lerp(vAdditional1_Sampled, vFianlDiffuse, vFianlDiffuse.a);

    }

     // For. Gradation 
    if (bHasTexturemap7)
    {
        float4 vGradationDiffuse = vColor_Gra;
        vGradationDiffuse.a = TextureMap7.Sample(LinearSampler, Diffuse_Texcoord).a;
        vFianlDiffuse.rgb = lerp(vFianlDiffuse.rgb, vGradationDiffuse.rgb, vGradationDiffuse.a);
    }
    
     // For. Overlay
    if (bIsOverlayOn)
    {
        float fOverlayIntensity = 1.f;
        float4 vFinalOverlayColor = float4(0.f, 0.f, 0.f, 0.f);
        
        if (bHasTexturemap8)
            fOverlayIntensity = TextureMap8.Sample(LinearSampler, Overlay_Texcoord).r;
        
        vFinalOverlayColor.r = (vOutColor.r <= 0.5) ? 2 * vOutColor.r * vColor_Overlay.r : 1 - 2 * (1 - vOutColor.r) * (1 - vColor_Overlay.r);
        vFinalOverlayColor.g = (vOutColor.g <= 0.5) ? 2 * vOutColor.g * vColor_Overlay.g : 1 - 2 * (1 - vOutColor.g) * (1 - vColor_Overlay.g);
        vFinalOverlayColor.b = (vOutColor.b <= 0.5) ? 2 * vOutColor.b * vColor_Overlay.b : 1 - 2 * (1 - vOutColor.b) * (1 - vColor_Overlay.b);
        
        vOutColor.rgb = lerp(vOutColor.rgb, vFinalOverlayColor.rgb, fOverlayIntensity);
    }
    
    // Color Edit 
    float luminance = dot(vOutColor.rgb, float3(0.299, 0.587, 0.114));
    vOutColor.rgb = lerp(vOutColor.rgb, vOutColor.rgb * fContrast, saturate(luminance));
    vOutColor.a *= fDefinition;

    // For. FadeOut
    if (bUseFadeOut)
        vOutColor.a *= (1.f - fLifeRatio);
        
    return vOutColor;
}

float4 PS_Clamp(EffectOut input) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 0.f);
    float fDistortionWeight = 0.f;
    float fDissolveWeight = 1.f;
    
    // For. Get Data from ganeral
    int bUseTextureColor = g_int_0;
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
    float2 vTexUVOffset_Blend = { g_vec4_3.x, g_vec4_3.y };
    float2 vTexUVOffset_Additional = { g_vec4_3.z, g_vec4_3.w };
    
    float fDefinition = g_mat_1._11;
    
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
    float4 vColor_DiffuseGradation = g_mat_1._31_32_33_34;
    float4 vColor_AlphaGra = g_mat_0._21_22_23_24;
    float4 vColor_Gra = g_mat_0._31_32_33_34;
    float4 vColor_Overlay = g_mat_0._41_42_43_44;
    float4 vColor_Additional = g_mat_1._21_22_23_24;
    
    // Get Distortion weight 
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSampler, input.uv + vTexUVOffset_Distortion);
        fDistortionWeight = vDistortion.r * 0.5f;
    }
    
    // Calc Texcoord ( diffuse, opacity, dissolve, overlay, gradation, Additional )
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
    float2 Blend_Texcoord = input.uv + vTexUVOffset_Blend;
    float2 Dissolve_Texcoord = input.uv + vTexUVOffset_Dissolve;
    float2 Gra_Texcoord = input.uv + vTexUVOffset_Gra + fDistortionWeight;
    float2 Overlay_Texcoord = input.uv + vTexUVOffset_Overlay;
    float2 Additional_Texcoord = input.uv + vTexUVOffset_Additional;

    // For. Diffuse
    float4 vFianlDiffuse = vColor_Diffuse;
    if (bHasDiffuseMap)
    {
        if (bUseTextureColor)
            vFianlDiffuse = DiffuseMap.Sample(LinearSamplerClamp, Opacity_Texcoord);
        else
        {
            vFianlDiffuse.a = DiffuseMap.Sample(LinearSamplerClamp, Opacity_Texcoord).r;
            vFianlDiffuse.rgb = lerp(vColor_DiffuseGradation.rgb, vFianlDiffuse.rgb, DiffuseMap.Sample(LinearSamplerClamp, Opacity_Texcoord).r);
        }
    }
        
    // For. Opacity 
    vFianlDiffuse.a = OpacityMap.Sample(LinearSamplerClamp, Opacity_Texcoord).a;
   
    // For. Blend 
    if (bHasTexturemap9)
        vFianlDiffuse.a *= TextureMap9.Sample(LinearSamplerClamp, Blend_Texcoord).a;
    
    // For. Dissolve
    if (bHasDissolveMap)
    {
        fDissolveWeight = DissolveMap.Sample(LinearSamplerClamp, Dissolve_Texcoord).r;
        if (bIsInverseDissolve)
        {
            if (fDissolveWeight > sin(fLifeRatio))
                vFianlDiffuse.a = 0.f;
        }
        else
        {
            if (fDissolveWeight < sin(fLifeRatio))
                vFianlDiffuse.a = 0.f;
        }
    }
    
    // For. Final + Alpha Gradation 
    if (vFianlDiffuse.a < fAlphaGraIntensity)
        vFianlDiffuse.rgb = lerp(vColor_AlphaGra.rgb, vFianlDiffuse.rgb, vFianlDiffuse.a);

    vOutColor = vFianlDiffuse;
    
    // For. Additional (Base)
    if (bHasTexturemap10)
    {
        float4 vAdditional1_Sampled = vColor_Additional;
        vAdditional1_Sampled = vColor_Additional * TextureMap10.Sample(LinearSamplerClamp, Additional_Texcoord).a;
        vOutColor = lerp(vAdditional1_Sampled, vFianlDiffuse, vFianlDiffuse.a);
    }

     // For. Gradation (front)
    if (bHasTexturemap7)
    {
        float4 vGradationDiffuse = vColor_Gra;
        vGradationDiffuse.a = TextureMap7.Sample(LinearSamplerClamp, Gra_Texcoord).a;
        vOutColor = lerp(vOutColor, vGradationDiffuse, vGradationDiffuse.a);
    }
    
     // For. Overlay
    if (bIsOverlayOn)
    {
        float fOverlayIntensity = 1.f;
        float4 vFinalOverlayColor = float4(0.f, 0.f, 0.f, 0.f);
        
        if (bHasTexturemap8)
            fOverlayIntensity = TextureMap8.Sample(LinearSamplerClamp, Overlay_Texcoord).r;
        
        vFinalOverlayColor.r = (vOutColor.r <= 0.5) ? 2 * vOutColor.r * vColor_Overlay.r : 1 - 2 * (1 - vOutColor.r) * (1 - vColor_Overlay.r);
        vFinalOverlayColor.g = (vOutColor.g <= 0.5) ? 2 * vOutColor.g * vColor_Overlay.g : 1 - 2 * (1 - vOutColor.g) * (1 - vColor_Overlay.g);
        vFinalOverlayColor.b = (vOutColor.b <= 0.5) ? 2 * vOutColor.b * vColor_Overlay.b : 1 - 2 * (1 - vOutColor.b) * (1 - vColor_Overlay.b);
        
        vOutColor.rgb = lerp(vOutColor.rgb, vFinalOverlayColor.rgb, fOverlayIntensity);
    }
    
    // Color Edit 
    float luminance = dot(vOutColor.rgb, float3(0.299, 0.587, 0.114));
    vOutColor.rgb = lerp(vOutColor.rgb, vOutColor.rgb * fContrast, saturate(luminance));
    vOutColor.a *= fDefinition;

    // For. FadeOut
    if (bUseFadeOut)
        vOutColor.a *= (1.f - fLifeRatio);
        
    return vOutColor;
}

float4 PS_Mirror(EffectOut input) : SV_Target
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
    float2 vTexUVOffset_Blend = { g_vec4_3.x, g_vec4_3.y };
    float2 vTexUVOffset_Additional = { g_vec4_3.z, g_vec4_3.w };
    
    float fDefinition = g_mat_1._11;
    
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
    float4 vColor_DiffuseGradation = g_mat_1._31_32_33_34;
    float4 vColor_AlphaGra = g_mat_0._21_22_23_24;
    float4 vColor_Gra = g_mat_0._31_32_33_34;
    float4 vColor_Overlay = g_mat_0._41_42_43_44;
    float4 vColor_Additional = g_mat_1._21_22_23_24;
    
    // Get Distortion weight 
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSamplerMirror, input.uv + vTexUVOffset_Distortion);
        fDistortionWeight = vDistortion.r * 0.5f;
    }
    
    // Calc Texcoord ( diffuse, opacity, dissolve, overlay, gradation, Additional )
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
    float2 Blend_Texcoord = input.uv + vTexUVOffset_Blend;
    float2 Dissolve_Texcoord = input.uv + vTexUVOffset_Dissolve;
    float2 Gra_Texcoord = input.uv + vTexUVOffset_Gra + fDistortionWeight;
    float2 Overlay_Texcoord = input.uv + vTexUVOffset_Overlay;
    float2 Additional_Texcoord = input.uv + vTexUVOffset_Additional;

    // For. Additional (base)
    vOutColor = vColor_Additional;
    if (bHasTexturemap10)
        vOutColor = vColor_Additional * TextureMap10.Sample(LinearSamplerMirror, Additional_Texcoord).a;

    // For. Diffuse
    float4 vFianlDiffuse = vColor_Diffuse;
    if (bHasDiffuseMap)
        vFianlDiffuse = DiffuseMap.Sample(LinearSamplerMirror, Diffuse_Texcoord);
        
    // For. Opacity 
    vFianlDiffuse.a = OpacityMap.Sample(LinearSamplerMirror, Opacity_Texcoord).a;
    vFianlDiffuse.rgb = lerp(vColor_DiffuseGradation.rgb, vFianlDiffuse.rgb, OpacityMap.Sample(LinearSamplerMirror, Opacity_Texcoord).r);
    
    // For. Gradation 
    float4 vGradationDiffuse = vColor_Gra;
    if (bHasTexturemap7)  
        vGradationDiffuse.a = TextureMap7.Sample(LinearSamplerMirror, Diffuse_Texcoord).a;
    vFianlDiffuse.rgb = lerp(vFianlDiffuse.rgb, vGradationDiffuse.rgb, vGradationDiffuse.a);
    
    // For. Blend 
    if (bHasTexturemap9)
        vFianlDiffuse.a *= TextureMap9.Sample(LinearSamplerMirror, Blend_Texcoord).a;
    
    // For. Dissolve
    if (bHasDissolveMap)
    {
        fDissolveWeight = DissolveMap.Sample(LinearSamplerMirror, Dissolve_Texcoord).r;
        if (bIsInverseDissolve)
        {
            if (fDissolveWeight > sin(fLifeRatio))
                vFianlDiffuse.a = 0.f;
        }
        else
        {
            if (fDissolveWeight < sin(fLifeRatio))
                vFianlDiffuse.a = 0.f;
        }
    }
    
    // For. Final + Alpha Gradation 
    if (vFianlDiffuse.a < fAlphaGraIntensity)
        vFianlDiffuse.rgb = lerp(vColor_AlphaGra.rgb, vFianlDiffuse.rgb, vFianlDiffuse.a);
    
    // For. Base + Final Diffuse 
    vOutColor = lerp(vOutColor, vFianlDiffuse, vFianlDiffuse.a);
    
    // For. Overlay
    if (bIsOverlayOn)
    {
        float fOverlayIntensity = 1.f;
        float4 vFinalOverlayColor = float4(0.f, 0.f, 0.f, 0.f);
        
        if (bHasTexturemap8)
            fOverlayIntensity = TextureMap8.Sample(LinearSamplerMirror, Overlay_Texcoord).r;
        
        vFinalOverlayColor.r = (vOutColor.r <= 0.5) ? 2 * vOutColor.r * vColor_Overlay.r : 1 - 2 * (1 - vOutColor.r) * (1 - vColor_Overlay.r);
        vFinalOverlayColor.g = (vOutColor.g <= 0.5) ? 2 * vOutColor.g * vColor_Overlay.g : 1 - 2 * (1 - vOutColor.g) * (1 - vColor_Overlay.g);
        vFinalOverlayColor.b = (vOutColor.b <= 0.5) ? 2 * vOutColor.b * vColor_Overlay.b : 1 - 2 * (1 - vOutColor.b) * (1 - vColor_Overlay.b);
        
        vOutColor.rgb = lerp(vOutColor.rgb, vFinalOverlayColor.rgb, fOverlayIntensity);
    }

    // Color Edit 
    float luminance = dot(vOutColor.rgb, float3(0.299, 0.587, 0.114));
    vOutColor.rgb = lerp(vOutColor.rgb, vOutColor.rgb * fContrast, saturate(luminance));
    vOutColor.a *= fDefinition;

    // For. FadeOut
    if (bUseFadeOut)
        vOutColor.a *= (1.f - fLifeRatio);
        
    return vOutColor;
}

float4 PS_Border(EffectOut input) : SV_Target
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
    float2 vTexUVOffset_Blend = { g_vec4_3.x, g_vec4_3.y };
    float2 vTexUVOffset_Additional = { g_vec4_3.z, g_vec4_3.w };
    
    float fDefinition = g_mat_1._11;
    
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
    float4 vColor_DiffuseGradation = g_mat_1._31_32_33_34;
    float4 vColor_AlphaGra = g_mat_0._21_22_23_24;
    float4 vColor_Gra = g_mat_0._31_32_33_34;
    float4 vColor_Overlay = g_mat_0._41_42_43_44;
    float4 vColor_Additional = g_mat_1._21_22_23_24;
    
    // Get Distortion weight 
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSamplerBorder, input.uv + vTexUVOffset_Distortion);
        fDistortionWeight = vDistortion.r * 0.5f;
    }
    
    // Calc Texcoord ( diffuse, opacity, dissolve, overlay, gradation, Additional )
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
    float2 Blend_Texcoord = input.uv + vTexUVOffset_Blend;
    float2 Dissolve_Texcoord = input.uv + vTexUVOffset_Dissolve;
    float2 Gra_Texcoord = input.uv + vTexUVOffset_Gra + fDistortionWeight;
    float2 Overlay_Texcoord = input.uv + vTexUVOffset_Overlay;
    float2 Additional_Texcoord = input.uv + vTexUVOffset_Additional;

    // For. Additional (base)
    vOutColor = vColor_Additional;
    if (bHasTexturemap10)
        vOutColor = vColor_Additional * TextureMap10.Sample(LinearSamplerBorder, Additional_Texcoord).a;

    // For. Diffuse
    float4 vFianlDiffuse = vColor_Diffuse;
    if (bHasDiffuseMap)
        vFianlDiffuse = DiffuseMap.Sample(LinearSamplerBorder, Diffuse_Texcoord);
        
    // For. Opacity 
    vFianlDiffuse.a = OpacityMap.Sample(LinearSamplerBorder, Opacity_Texcoord).a;
    vFianlDiffuse.rgb = lerp(vColor_DiffuseGradation.rgb, vFianlDiffuse.rgb, OpacityMap.Sample(LinearSamplerBorder, Opacity_Texcoord).r);
    
    // For. Gradation 
    float4 vGradationDiffuse = vColor_Gra;
    if (bHasTexturemap7)  
        vGradationDiffuse.a = TextureMap7.Sample(LinearSamplerBorder, Diffuse_Texcoord).a;
    vFianlDiffuse.rgb = lerp(vFianlDiffuse.rgb, vGradationDiffuse.rgb, vGradationDiffuse.a);
    
    // For. Blend 
    if (bHasTexturemap9)
        vFianlDiffuse.a *= TextureMap9.Sample(LinearSamplerBorder, Blend_Texcoord).a;
    
    // For. Dissolve
    if (bHasDissolveMap)
    {
        fDissolveWeight = DissolveMap.Sample(LinearSamplerBorder, Dissolve_Texcoord).r;
        if (bIsInverseDissolve)
        {
            if (fDissolveWeight > sin(fLifeRatio))
                vFianlDiffuse.a = 0.f;
        }
        else
        {
            if (fDissolveWeight < sin(fLifeRatio))
                vFianlDiffuse.a = 0.f;
        }
    }
    
    // For. Final + Alpha Gradation 
    if (vFianlDiffuse.a < fAlphaGraIntensity)
        vFianlDiffuse.rgb = lerp(vColor_AlphaGra.rgb, vFianlDiffuse.rgb, vFianlDiffuse.a);
    
    // For. Base + Final Diffuse 
    vOutColor = lerp(vOutColor, vFianlDiffuse, vFianlDiffuse.a);
    
    // For. Overlay
    if (bIsOverlayOn)
    {
        float fOverlayIntensity = 1.f;
        float4 vFinalOverlayColor = float4(0.f, 0.f, 0.f, 0.f);
        
        if (bHasTexturemap8)
            fOverlayIntensity = TextureMap8.Sample(LinearSamplerBorder, Overlay_Texcoord).r;
        
        vFinalOverlayColor.r = (vOutColor.r <= 0.5) ? 2 * vOutColor.r * vColor_Overlay.r : 1 - 2 * (1 - vOutColor.r) * (1 - vColor_Overlay.r);
        vFinalOverlayColor.g = (vOutColor.g <= 0.5) ? 2 * vOutColor.g * vColor_Overlay.g : 1 - 2 * (1 - vOutColor.g) * (1 - vColor_Overlay.g);
        vFinalOverlayColor.b = (vOutColor.b <= 0.5) ? 2 * vOutColor.b * vColor_Overlay.b : 1 - 2 * (1 - vOutColor.b) * (1 - vColor_Overlay.b);
        
        vOutColor.rgb = lerp(vOutColor.rgb, vFinalOverlayColor.rgb, fOverlayIntensity);
    }

    // Color Edit 
    float luminance = dot(vOutColor.rgb, float3(0.299, 0.587, 0.114));
    vOutColor.rgb = lerp(vOutColor.rgb, vOutColor.rgb * fContrast, saturate(luminance));
    vOutColor.a *= fDefinition;

    // For. FadeOut
    if (bUseFadeOut)
        vOutColor.a *= (1.f - fLifeRatio);
        
    return vOutColor;
}

technique11 T0 // 0
{
    pass pass_Wrap // 0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Wrap()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }

    pass pass_Clamp // 1
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Clamp()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }

    pass pass_Mirror // 2
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Mirror()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }

    pass pass_Border // 3
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Border()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
};
  