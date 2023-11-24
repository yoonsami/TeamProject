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
    float4 vOutColor = float4(1.f, 1.f, 1.f, 1.f);
    
    /* Get Shared Data */
    bool bUseFadeOut = (bool) g_int_0;
    bool bUseRimLight = (bool) g_int_1;
    bool bUseSpriteAnim = (bool) g_int_2;
    
    float fLifeTimeRatio = g_float_0;
    float fDissolveWeight = g_float_1;
    float fRimLightIntensity = g_float_2;
    
    float2 vColorOptions_Op[3] = { g_vec2_0, g_vec2_1, g_vec2_2 };
    
    float4 vBaseColor1_Op1 = g_mat_0._11_12_13_14;
    float4 vBaseColor2_Op1 = g_mat_0._21_22_23_24;
    float4 vBaseColor1_Op2 = g_mat_0._31_32_33_34;
    float4 vBaseColor2_Op2 = g_mat_0._41_42_43_44;
    float4 vBaseColor1_Op3 = g_mat_1._11_12_13_14;
    float4 vBaseColor2_Op3 = g_mat_1._21_22_23_24;
    float4 vBaseColor_Overlay = g_mat_1._31_32_33_34;
    float4 vBaseColor_RimLight = g_mat_1._41_42_43_44;
    
    float bUseTexColor_Op[3] = { g_mat_2._11, g_mat_2._12, g_mat_2._13 };
    int iFilpOPtion_Op[3] = { g_mat_2._21, g_mat_2._22, g_mat_2._23 };
    
    /* Calc Texcoord */
    float fDistortionWeight = 0.f;
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSampler, input.uv + g_vec2_3);
        fDistortionWeight = vDistortion.r * 0.5f;
    }

    float2 vTexcoord_Op[3] =
    {
        input.uv + float2(g_vec4_0.x, g_vec4_0.y) + fDistortionWeight,
                               input.uv + float2(g_vec4_0.z, g_vec4_0.w) + fDistortionWeight,
                               input.uv + float2(g_vec4_1.x, g_vec4_1.y) + fDistortionWeight
    };
    
    float2 vTexcoord_Blend = input.uv + float2(g_vec4_1.z, g_vec4_1.w) + fDistortionWeight;
    float2 vTexcoord_Overlay = input.uv + float2(g_vec4_2.x, g_vec4_2.y);
    float2 vTexcoord_Dissolve = input.uv + float2(g_vec4_2.z, g_vec4_2.w);
    if (bUseSpriteAnim)
    {
        float2 vSpriteAnim_RangeX = float2(g_vec4_3.x, g_vec4_3.y);
        float2 vSpriteAnim_RangeY = float2(g_vec4_3.z, g_vec4_3.w);
        vTexcoord_Op[0] = float2(vSpriteAnim_RangeX.x + ((vSpriteAnim_RangeX.y - vSpriteAnim_RangeX.x) * input.uv.x),
                              vSpriteAnim_RangeY.x + ((vSpriteAnim_RangeY.y - vSpriteAnim_RangeY.x) * input.uv.y)) + fDistortionWeight;
    }
    
    // For. Flip option 
    for (int i = 0; i < 3; i++)
    {
        if (iFilpOPtion_Op[i] == 1) // flip up/down
            vTexcoord_Op[i].y = 1.f - vTexcoord_Op[i].y;
        else if (iFilpOPtion_Op[i] == 2) // flip left/right 
            vTexcoord_Op[i].x = 1.f - vTexcoord_Op[i].x;
        else if (iFilpOPtion_Op[i] == 3) // flip up/down and left/right 
        {
            vTexcoord_Op[i].x = 1.f - vTexcoord_Op[i].x;
            vTexcoord_Op[i].y = 1.f - vTexcoord_Op[i].y;
        }
        else if (iFilpOPtion_Op[i] == 4)
        {
            float2 vTemp = vTexcoord_Op[i];
            vTexcoord_Op[i].x = 1.f - vTemp.y;
            vTexcoord_Op[i].y = vTemp.x;
        }
        else if (iFilpOPtion_Op[i] == 5)
        {
            float2 vTemp = vTexcoord_Op[i];
            vTexcoord_Op[i].x = vTemp.y;
            vTexcoord_Op[i].y = 1 - vTemp.x;
        }
    }
  
    /* Sampled from textures */    
    float4 vSample_Op1 = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Op2 = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Op3 = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Blend = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Overlay = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Dissolve = { 0.f, 0.f, 0.f, 0.f };
    if (bHasTexturemap7)
    {
        if (0.f == bUseTexColor_Op[0])
        {
            vSample_Op1.a = TextureMap7.Sample(LinearSampler, vTexcoord_Op[0]).r;
            vSample_Op1.rgb = lerp(vBaseColor2_Op1, vBaseColor1_Op1, vSample_Op1.a);
        }
        else
        {
            vSample_Op1 = TextureMap7.Sample(LinearSampler, vTexcoord_Op[0]);
            vSample_Op1.rgb = pow(vSample_Op1.rgb, GAMMA);
        }
        
        float luminance = dot(vSample_Op1.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op1.rgb = lerp(vSample_Op1.rgb, vSample_Op1.rgb * vColorOptions_Op[0].x, saturate(luminance));
        vSample_Op1.a = saturate(vSample_Op1.a * vColorOptions_Op[0].y);
    }
    if (bHasTexturemap8)
    {
        if (0.f == bUseTexColor_Op[1])
        {
            vSample_Op2.a = TextureMap8.Sample(LinearSampler, vTexcoord_Op[1]).r;
            vSample_Op2.rgb = lerp(vBaseColor2_Op2, vBaseColor1_Op2, vSample_Op2.a);
        }
        else
        {
            vSample_Op2 = TextureMap8.Sample(LinearSampler, vTexcoord_Op[1]);
            vSample_Op2.rgb = pow(vSample_Op2.rgb, GAMMA);
        }
        
        float luminance = dot(vSample_Op2.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op2.rgb = lerp(vSample_Op2.rgb, vSample_Op2.rgb * vColorOptions_Op[1].x, saturate(luminance));
        vSample_Op2.a = saturate(vSample_Op2.a * vColorOptions_Op[1].y);
    }
    if (bHasTexturemap9)
    {
        if (0.f == bUseTexColor_Op[2])
        {
            vSample_Op3.a = TextureMap9.Sample(LinearSampler, vTexcoord_Op[2]).r;
            vSample_Op3.rgb = lerp(vBaseColor2_Op3, vBaseColor1_Op3, vSample_Op3.a);
        }
        else
        {
            vSample_Op3 = TextureMap9.Sample(LinearSampler, vTexcoord_Op[2]);
            vSample_Op3.rgb = pow(vSample_Op3.rgb, GAMMA);
        }

        float luminance = dot(vSample_Op3.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op3.rgb = lerp(vSample_Op3.rgb, vSample_Op3.rgb * vColorOptions_Op[2].x, saturate(luminance));
        vSample_Op3.a = saturate(vSample_Op3.a * vColorOptions_Op[2].y);
    }
    if (bHasTexturemap10)
        vSample_Blend = TextureMap10.Sample(LinearSampler, vTexcoord_Blend);
    if (bHasTexturemap11)
        vSample_Overlay = TextureMap11.Sample(LinearSampler, vTexcoord_Overlay);
    if (bHasDissolveMap)
        vSample_Dissolve = DissolveMap.Sample(LinearSampler, vTexcoord_Dissolve);
    
    /* Mix four option textures */
    vOutColor = vSample_Op1;
    vOutColor = lerp(vOutColor, vSample_Op2, vSample_Op2.a);
    vOutColor = lerp(vOutColor, vSample_Op3, vSample_Op3.a);
    
    /* Gamma collection */
    if (bUseTexColor_Op[0] || bUseTexColor_Op[1] || bUseTexColor_Op[2])
        vOutColor.rgb = pow(vOutColor.rgb, 1.f / GAMMA);

    /* Blend */
    if (bHasTexturemap10)
        vOutColor.a *= vSample_Blend.r;
    
    /* DissolveMap */
    if (bHasDissolveMap)
    {
        float fDissolve = vSample_Dissolve.r;
        if (fDissolve < sin(fDissolveWeight))//sin(fLifeTimeRatio))
            vOutColor.a = 0.f;
    }
    
    /* Overlay */
    if (bHasTexturemap11)
    {
        float fOverlayIntensity = vSample_Overlay.r;
        float4 vFinalOverlayColor = float4(0.f, 0.f, 0.f, 0.f);

        vFinalOverlayColor.r = (vOutColor.r <= 0.5) ? 2 * vOutColor.r * vSample_Overlay.r : 1 - 2 * (1 - vOutColor.r) * (1 - vSample_Overlay.r);
        vFinalOverlayColor.g = (vOutColor.g <= 0.5) ? 2 * vOutColor.g * vSample_Overlay.g : 1 - 2 * (1 - vOutColor.g) * (1 - vSample_Overlay.g);
        vFinalOverlayColor.b = (vOutColor.b <= 0.5) ? 2 * vOutColor.b * vSample_Overlay.b : 1 - 2 * (1 - vOutColor.b) * (1 - vSample_Overlay.b);
        
        vOutColor.rgb = lerp(vOutColor.rgb, vFinalOverlayColor.rgb, fOverlayIntensity);
    }
    
    /* Rim Light */
        // TODO 
    
    /* Fade Out */
    if (bUseFadeOut)
        vOutColor.a *= (1.f - fLifeTimeRatio);
    
    if (vOutColor.a < 0.1f)
        discard;
    
    return vOutColor;
}

float4 PS_Clamp(EffectOut input) : SV_Target
{
    float4 vOutColor = float4(1.f, 1.f, 1.f, 1.f);
    
    /* Get Shared Data */
    bool bUseFadeOut = (bool)g_int_0;
    bool bInverseDissolve = (bool) g_int_1;
    bool bUseSpriteAnim = (bool) g_int_2;
    
    float fLifeTimeRatio = g_float_0;
    float fDissolveWeight = g_float_1;
    
    float2 vColorOptions_Op[3] = { g_vec2_0, g_vec2_1, g_vec2_2 };
    
    float4 vBaseColor1_Op1 = g_mat_0._11_12_13_14;
    float4 vBaseColor2_Op1 = g_mat_0._21_22_23_24;
    float4 vBaseColor1_Op2 = g_mat_0._31_32_33_34;
    float4 vBaseColor2_Op2 = g_mat_0._41_42_43_44;
    float4 vBaseColor1_Op3 = g_mat_1._11_12_13_14;
    float4 vBaseColor2_Op3 = g_mat_1._21_22_23_24;
    float4 vBaseColor_Overlay = g_mat_1._31_32_33_34;
    float4 vBaseColor_RimLight = g_mat_1._41_42_43_44;
    
    float bUseTexColor_Op[3] = { g_mat_2._11, g_mat_2._12, g_mat_2._13 };
    int iFilpOPtion_Op[3] = { g_mat_2._21, g_mat_2._22, g_mat_2._23 };
    
    /* Calc Texcoord */
    float fDistortionWeight = 0.f;
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSampler, input.uv + g_vec2_3);
        fDistortionWeight = vDistortion.r * 0.5f;
    }

    float2 vTexcoord_Op[3] = { input.uv + float2(g_vec4_0.x, g_vec4_0.y) + fDistortionWeight,
                               input.uv + float2(g_vec4_0.z, g_vec4_0.w) + fDistortionWeight,
                               input.uv + float2(g_vec4_1.x, g_vec4_1.y) + fDistortionWeight
                             };
    
    float2 vTexcoord_Blend = input.uv + float2(g_vec4_1.z, g_vec4_1.w) + fDistortionWeight;
    float2 vTexcoord_Overlay    = input.uv + float2(g_vec4_2.x, g_vec4_2.y);
    float2 vTexcoord_Dissolve   = input.uv + float2(g_vec4_2.z, g_vec4_2.w);
    if (bUseSpriteAnim)
    {
        float2 vSpriteAnim_RangeX = float2(g_vec4_3.x, g_vec4_3.y);
        float2 vSpriteAnim_RangeY = float2(g_vec4_3.z, g_vec4_3.w);
        vTexcoord_Op[0] = float2(vSpriteAnim_RangeX.x + ((vSpriteAnim_RangeX.y - vSpriteAnim_RangeX.x) * input.uv.x),
                              vSpriteAnim_RangeY.x + ((vSpriteAnim_RangeY.y - vSpriteAnim_RangeY.x) * input.uv.y)) + fDistortionWeight;
    }
    
    // For. Flip option 
    for (int i = 0; i < 3; i++)
    {
        if (iFilpOPtion_Op[i] == 1) // flip up/down
            vTexcoord_Op[i].y = 1.f - vTexcoord_Op[i].y;
        else if (iFilpOPtion_Op[i] == 2) // flip left/right 
            vTexcoord_Op[i].x = 1.f - vTexcoord_Op[i].x;
        else if (iFilpOPtion_Op[i] == 3) // flip up/down and left/right 
        {
            vTexcoord_Op[i].x = 1.f - vTexcoord_Op[i].x;
            vTexcoord_Op[i].y = 1.f - vTexcoord_Op[i].y;
        }
        else if(iFilpOPtion_Op[i] == 4)
        {
            float2 vTemp = vTexcoord_Op[i];
            vTexcoord_Op[i].x = 1.f - vTemp.y;
            vTexcoord_Op[i].y = vTemp.x;
        }
        else if (iFilpOPtion_Op[i] == 5)
        {
            float2 vTemp = vTexcoord_Op[i];
            vTexcoord_Op[i].x = vTemp.y;
            vTexcoord_Op[i].y = 1 - vTemp.x;
        }
    }
  
    /* Sampled from textures */    
        float4 vSample_Op1 = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Op2 = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Op3 = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Blend = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Overlay = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Dissolve = { 0.f, 0.f, 0.f, 0.f };
    if (bHasTexturemap7)
    {        
        if (0.f == bUseTexColor_Op[0])
        {
            vSample_Op1.a = TextureMap7.Sample(LinearSamplerClamp, vTexcoord_Op[0]).r;
            vSample_Op1.rgb = lerp(vBaseColor2_Op1, vBaseColor1_Op1, vSample_Op1.a);
        }
        else
        {
            vSample_Op1 = TextureMap7.Sample(LinearSamplerClamp, vTexcoord_Op[0]);
            vSample_Op1.rgb = pow(vSample_Op1.rgb, GAMMA);
        }
        
        float luminance = dot(vSample_Op1.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op1.rgb = lerp(vSample_Op1.rgb, vSample_Op1.rgb * vColorOptions_Op[0].x, saturate(luminance));
        vSample_Op1.a = saturate(vSample_Op1.a * vColorOptions_Op[0].y);
    }
    if (bHasTexturemap8)
    {
        if (0.f == bUseTexColor_Op[1])
        {
            vSample_Op2.a = TextureMap8.Sample(LinearSamplerClamp, vTexcoord_Op[1]).r;
            vSample_Op2.rgb = lerp(vBaseColor2_Op2, vBaseColor1_Op2, vSample_Op2.a);
        }
        else
        {
            vSample_Op2 = TextureMap8.Sample(LinearSamplerClamp, vTexcoord_Op[1]);
            vSample_Op2.rgb = pow(vSample_Op2.rgb, GAMMA);
        }
        
        float luminance = dot(vSample_Op2.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op2.rgb = lerp(vSample_Op2.rgb, vSample_Op2.rgb * vColorOptions_Op[1].x, saturate(luminance));
        vSample_Op2.a = saturate(vSample_Op2.a * vColorOptions_Op[1].y);
    }
    if (bHasTexturemap9)
    {
        if (0.f == bUseTexColor_Op[2])
        {
            vSample_Op3.a = TextureMap9.Sample(LinearSamplerClamp, vTexcoord_Op[2]).r;
            vSample_Op3.rgb = lerp(vBaseColor2_Op3, vBaseColor1_Op3, vSample_Op3.a);
        }
        else
        {
            vSample_Op3 = TextureMap9.Sample(LinearSamplerClamp, vTexcoord_Op[2]);
            vSample_Op3.rgb = pow(vSample_Op3.rgb, GAMMA);
        }

        float luminance = dot(vSample_Op3.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op3.rgb = lerp(vSample_Op3.rgb, vSample_Op3.rgb * vColorOptions_Op[2].x, saturate(luminance));
        vSample_Op3.a = saturate(vSample_Op3.a * vColorOptions_Op[2].y);
    }
    if (bHasTexturemap10)
        vSample_Blend = TextureMap10.Sample(LinearSamplerClamp, vTexcoord_Blend);
    if (bHasTexturemap11)
        vSample_Overlay = TextureMap11.Sample(LinearSamplerClamp, vTexcoord_Overlay);
    if (bHasDissolveMap)
        vSample_Dissolve = DissolveMap.Sample(LinearSamplerClamp, vTexcoord_Dissolve);
    
    /* Mix four option textures */
    vOutColor = vSample_Op1;
    vOutColor = lerp(vOutColor, vSample_Op2, vSample_Op2.a);
    vOutColor = lerp(vOutColor, vSample_Op3, vSample_Op3.a);
    
    /* Gamma collection */
    if (bUseTexColor_Op[0] || bUseTexColor_Op[1] || bUseTexColor_Op[2])
        vOutColor.rgb = pow(vOutColor.rgb, 1.f / GAMMA);

    /* Blend */
    if(bHasTexturemap10)
        vOutColor.a *= vSample_Blend.r;
    
    /* DissolveMap */
    if (bHasDissolveMap)
    {
        float fDissolve = vSample_Dissolve.r;
        if (bInverseDissolve)
        {
            if (fDissolve > sin(fDissolveWeight)) // sin(fLifeTimeRatio))
                vOutColor.a = 0.f;
        }
        else
        {
            if (fDissolve < sin(fDissolveWeight))//sin(fLifeTimeRatio))
                vOutColor.a = 0.f;
        }
    }
    
    /* Overlay */
    if (bHasTexturemap11)
    {
        float fOverlayIntensity = vSample_Overlay.r;
        float4 vFinalOverlayColor = float4(0.f, 0.f, 0.f, 0.f);

        vFinalOverlayColor.r = (vOutColor.r <= 0.5) ? 2 * vOutColor.r * vSample_Overlay.r : 1 - 2 * (1 - vOutColor.r) * (1 - vSample_Overlay.r);
        vFinalOverlayColor.g = (vOutColor.g <= 0.5) ? 2 * vOutColor.g * vSample_Overlay.g : 1 - 2 * (1 - vOutColor.g) * (1 - vSample_Overlay.g);
        vFinalOverlayColor.b = (vOutColor.b <= 0.5) ? 2 * vOutColor.b * vSample_Overlay.b : 1 - 2 * (1 - vOutColor.b) * (1 - vSample_Overlay.b);
        
        vOutColor.rgb = lerp(vOutColor.rgb, vFinalOverlayColor.rgb, fOverlayIntensity);
    }

    /* Fade Out */
    if (bUseFadeOut)
        vOutColor.a *= (1.f - fLifeTimeRatio);
    
    if(vOutColor.a < 0.1f)
        discard;
    
    return vOutColor;
}

float4 PS_LightEffect(EffectOut input) :SV_Target
{
    float4 vOutColor = float4(1.f, 1.f, 1.f, 1.f);
    
    /* Get Shared Data */
    bool bUseFadeOut = (bool) g_int_0;
    bool bUseRimLight = (bool) g_int_1;
    bool bUseSpriteAnim = (bool) g_int_2;
    
    float fLifeTimeRatio = g_float_0;
    float fDissolveWeight = g_float_1;
    float fRimLightIntensity = g_float_2;
    
    float2 vColorOptions_Op[3] = { g_vec2_0, g_vec2_1, g_vec2_2 };
    
    float4 vBaseColor1_Op1 = g_mat_0._11_12_13_14;
    float4 vBaseColor2_Op1 = g_mat_0._21_22_23_24;
    float4 vBaseColor1_Op2 = g_mat_0._31_32_33_34;
    float4 vBaseColor2_Op2 = g_mat_0._41_42_43_44;
    float4 vBaseColor1_Op3 = g_mat_1._11_12_13_14;
    float4 vBaseColor2_Op3 = g_mat_1._21_22_23_24;
    float4 vBaseColor_Overlay = g_mat_1._31_32_33_34;
    float4 vBaseColor_RimLight = g_mat_1._41_42_43_44;
    
    float bUseTexColor_Op[3] = { g_mat_2._11, g_mat_2._12, g_mat_2._13 };
    int iFilpOPtion_Op[3] = { g_mat_2._21, g_mat_2._22, g_mat_2._23 };
    
    /* Calc Texcoord */
    float fDistortionWeight = 0.f;
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSampler, input.uv + g_vec2_3);
        fDistortionWeight = vDistortion.r * 0.5f;
    }

    float2 vTexcoord_Op[3] =
    {
        input.uv + float2(g_vec4_0.x, g_vec4_0.y) + fDistortionWeight,
                               input.uv + float2(g_vec4_0.z, g_vec4_0.w) + fDistortionWeight,
                               input.uv + float2(g_vec4_1.x, g_vec4_1.y) + fDistortionWeight
    };
    
    float2 vTexcoord_Blend = input.uv + float2(g_vec4_1.z, g_vec4_1.w) + fDistortionWeight;
    float2 vTexcoord_Overlay = input.uv + float2(g_vec4_2.x, g_vec4_2.y);
    float2 vTexcoord_Dissolve = input.uv + float2(g_vec4_2.z, g_vec4_2.w);
    if (bUseSpriteAnim)
    {
        float2 vSpriteAnim_RangeX = float2(g_vec4_3.x, g_vec4_3.y);
        float2 vSpriteAnim_RangeY = float2(g_vec4_3.z, g_vec4_3.w);
        vTexcoord_Op[0] = float2(vSpriteAnim_RangeX.x + ((vSpriteAnim_RangeX.y - vSpriteAnim_RangeX.x) * input.uv.x),
                              vSpriteAnim_RangeY.x + ((vSpriteAnim_RangeY.y - vSpriteAnim_RangeY.x) * input.uv.y)) + fDistortionWeight;
    }
    
    // For. Flip option 
    for (int i = 0; i < 3; i++)
    {
        if (iFilpOPtion_Op[i] == 1) // flip up/down
            vTexcoord_Op[i].y = 1.f - vTexcoord_Op[i].y;
        else if (iFilpOPtion_Op[i] == 2) // flip left/right 
            vTexcoord_Op[i].x = 1.f - vTexcoord_Op[i].x;
        else if (iFilpOPtion_Op[i] == 3) // flip up/down and left/right 
        {
            vTexcoord_Op[i].x = 1.f - vTexcoord_Op[i].x;
            vTexcoord_Op[i].y = 1.f - vTexcoord_Op[i].y;
        }
        else if (iFilpOPtion_Op[i] == 4)
        {
            float2 vTemp = vTexcoord_Op[i];
            vTexcoord_Op[i].x = 1.f - vTemp.y;
            vTexcoord_Op[i].y = vTemp.x;
        }
        else if (iFilpOPtion_Op[i] == 5)
        {
            float2 vTemp = vTexcoord_Op[i];
            vTexcoord_Op[i].x = vTemp.y;
            vTexcoord_Op[i].y = 1 - vTemp.x;
        }
    }
  
    /* Sampled from textures */    
    float4 vSample_Op1 = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Op2 = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Op3 = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Blend = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Overlay = { 0.f, 0.f, 0.f, 0.f };
    float4 vSample_Dissolve = { 0.f, 0.f, 0.f, 0.f };
    if (bHasTexturemap7)
    {
        if (0.f == bUseTexColor_Op[0])
        {
            vSample_Op1.a = TextureMap7.Sample(LinearSampler, vTexcoord_Op[0]).r;
            vSample_Op1.rgb = lerp(vBaseColor2_Op1, vBaseColor1_Op1, vSample_Op1.a);
        }
        else
        {
            vSample_Op1 = TextureMap7.Sample(LinearSampler, vTexcoord_Op[0]);
            vSample_Op1.rgb = pow(vSample_Op1.rgb, GAMMA);
        }
        
        float luminance = dot(vSample_Op1.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op1.rgb = lerp(vSample_Op1.rgb, vSample_Op1.rgb * vColorOptions_Op[0].x, saturate(luminance));
        vSample_Op1.a = saturate(vSample_Op1.a * vColorOptions_Op[0].y);
    }
    if (bHasTexturemap8)
    {
        if (0.f == bUseTexColor_Op[1])
        {
            vSample_Op2.a = TextureMap8.Sample(LinearSampler, vTexcoord_Op[1]).r;
            vSample_Op2.rgb = lerp(vBaseColor2_Op2, vBaseColor1_Op2, vSample_Op2.a);
        }
        else
        {
            vSample_Op2 = TextureMap8.Sample(LinearSampler, vTexcoord_Op[1]);
            vSample_Op2.rgb = pow(vSample_Op2.rgb, GAMMA);
        }
        
        float luminance = dot(vSample_Op2.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op2.rgb = lerp(vSample_Op2.rgb, vSample_Op2.rgb * vColorOptions_Op[1].x, saturate(luminance));
        vSample_Op2.a = saturate(vSample_Op2.a * vColorOptions_Op[1].y);
    }
    if (bHasTexturemap9)
    {
        if (0.f == bUseTexColor_Op[2])
        {
            vSample_Op3.a = TextureMap9.Sample(LinearSampler, vTexcoord_Op[2]).r;
            vSample_Op3.rgb = lerp(vBaseColor2_Op3, vBaseColor1_Op3, vSample_Op3.a);
        }
        else
        {
            vSample_Op3 = TextureMap9.Sample(LinearSampler, vTexcoord_Op[2]);
            vSample_Op3.rgb = pow(vSample_Op3.rgb, GAMMA);
        }

        float luminance = dot(vSample_Op3.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op3.rgb = lerp(vSample_Op3.rgb, vSample_Op3.rgb * vColorOptions_Op[2].x, saturate(luminance));
        vSample_Op3.a = saturate(vSample_Op3.a * vColorOptions_Op[2].y);
    }
    if (bHasTexturemap10)
        vSample_Blend = TextureMap10.Sample(LinearSampler, vTexcoord_Blend);
    if (bHasTexturemap11)
        vSample_Overlay = TextureMap11.Sample(LinearSampler, vTexcoord_Overlay);
    if (bHasDissolveMap)
        vSample_Dissolve = DissolveMap.Sample(LinearSampler, vTexcoord_Dissolve);
    
    /* Mix four option textures */
    vOutColor = vSample_Op1;
    vOutColor = lerp(vOutColor, vSample_Op2, vSample_Op2.a);
    vOutColor = lerp(vOutColor, vSample_Op3, vSample_Op3.a);
    
    /* Gamma collection */
    if (bUseTexColor_Op[0] || bUseTexColor_Op[1] || bUseTexColor_Op[2])
        vOutColor.rgb = pow(vOutColor.rgb, 1.f / GAMMA);

    /* Blend */
    if (bHasTexturemap10)
        vOutColor.a *= vSample_Blend.r;
    
    /* DissolveMap */
    if (bHasDissolveMap)
    {
        float fDissolve = vSample_Dissolve.r;
        if (fDissolve < sin(fDissolveWeight))//sin(fLifeTimeRatio))
            vOutColor.a = 0.f;
    }
    
    /* Overlay */
    if (bHasTexturemap11)
    {
        float fOverlayIntensity = vSample_Overlay.r;
        float4 vFinalOverlayColor = float4(0.f, 0.f, 0.f, 0.f);

        vFinalOverlayColor.r = (vOutColor.r <= 0.5) ? 2 * vOutColor.r * vSample_Overlay.r : 1 - 2 * (1 - vOutColor.r) * (1 - vSample_Overlay.r);
        vFinalOverlayColor.g = (vOutColor.g <= 0.5) ? 2 * vOutColor.g * vSample_Overlay.g : 1 - 2 * (1 - vOutColor.g) * (1 - vSample_Overlay.g);
        vFinalOverlayColor.b = (vOutColor.b <= 0.5) ? 2 * vOutColor.b * vSample_Overlay.b : 1 - 2 * (1 - vOutColor.b) * (1 - vSample_Overlay.b);
        
        vOutColor.rgb = lerp(vOutColor.rgb, vFinalOverlayColor.rgb, fOverlayIntensity);
    }
    
    /* Lighting */
    float diffuseRatio = 0.f;
    {
        float3 viewLightDir = 0.f;
        
        viewLightDir = normalize(mul(float4(lights[0].vDirection.xyz, 0.f), V).xyz);
        diffuseRatio = saturate(dot(-viewLightDir, normalize(input.viewNormal)));
                
    }

    vOutColor.rgb = vOutColor.rgb * diffuseRatio;
    
    /* Rim Light */
        // TODO 
    
    /* Fade Out */
        if (bUseFadeOut)
            vOutColor.a *= (1.f - fLifeTimeRatio);
    
    if (vOutColor.a < 0.1f)
        discard;
    
    return vOutColor;
    
    
}

float4 PS_Mirror(EffectOut input) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 0.f);
    // TODO
    return vOutColor;
}

float4 PS_Border(EffectOut input) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 0.f);
    // TODO
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
  