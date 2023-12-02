#include "Render.fx"
#include "Light.fx"

float4 LightingWater(float3 viewNormal, float3 viewLightDir, float3 viewDir)
{
    float rim = saturate(dot(viewNormal, viewDir));
    rim = pow(1 - rim, 3);
    
    float4 final = rim;
    
    return float4(final.rgb, 0.f);
}

MeshOutput VS_Water(VTXMesh input)
{
    MeshOutput output;
    
    output.position = mul(float4(input.position, 1.f), W);
    //output.position.y += cos(abs(input.uv.x * 2 - 1) * 3);
    
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V);
    
    output.viewNormal = mul(input.normal, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.normal, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
    float4 diffuseColor;
    float4 specularColor;
    float4 emissiveColor;
    
    float fLightIntensity = 1.05f;
        
    float4 vBaseColor1_Op1 = g_vec4_1;
    float4 vBaseColor2_Op1 = g_vec4_2;
        
    /* Calc Texcoord */
    float fDistortionWeight = 0.f;
    if (bHasDistortionMap)
    {
        float4 vDistortion = DistortionMap.Sample(LinearSampler, input.uv + g_vec2_0 /*uvsliding*/);
        fDistortionWeight = vDistortion.r * 0.5f;
    }
    
    float4 vSample_Op1 = { 0.f, 0.f, 0.f, 0.f };

    if (bHasDiffuseMap)
    {
        vSample_Op1.a = DiffuseMap.Sample(LinearSampler, input.uv + g_vec2_1 /*uvsliding*/ + fDistortionWeight).r;
        vSample_Op1.rgb = lerp(vBaseColor2_Op1, vBaseColor1_Op1, vSample_Op1.a);

        float luminance = dot(vSample_Op1.rgb, float3(0.299, 0.587, 0.114));
        vSample_Op1.rgb = lerp(vSample_Op1.rgb, vSample_Op1.rgb * 1.5f, saturate(luminance));
        vSample_Op1.a = saturate(vSample_Op1.a * 4.f);
    }
    
    diffuseColor = vSample_Op1;

    float3 normal1 = input.viewNormal;
    ComputeNormalMapping_ViewSpace(normal1, input.viewTangent, input.uv + g_vec2_1 /*uvsliding*/ + fDistortionWeight);
    float3 normal2 = input.viewNormal;
    ComputeNormalMapping_ViewSpace(normal2, input.viewTangent, input.uv - g_vec2_1 /*uvsliding*/ + fDistortionWeight);

    float3 normal = (normal1 + normal2) * 0.5f;
    normal *= float3(0.5, 0.5, 1.);
    
    float diffuseRatio = 0.f;
    {
        float3 viewLightDir = 0.f;
          
        viewLightDir = normalize(mul(float4(lights[0].vDirection.xyz, 0.f), V).xyz);
        diffuseRatio = saturate(dot(-viewLightDir, normalize(normal)));
                  
    }
    
    
    
    float3 vLightingColor = diffuseColor.rgb * diffuseRatio;
    diffuseColor.rgb = lerp(diffuseColor.rgb, vLightingColor, fLightIntensity);
    diffuseColor += LightingWater(normal, lights[0].vDirection.xyz, input.viewPosition);
    diffuseColor.a = 1.f;
    
    return diffuseColor;
}

float4 PS_Masked(MeshOutput input) : SV_TARGET
{
    //if (g_bHasNormalMap)
    //    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    
    //LightColor color = ComputeLight(input.normal, input.uv, input.worldPosition);
    float4 maskColor = OpacityMap.Sample(LinearSampler, input.uv);
    
   
    
    return float4(1.f, 1.f, 1.f, 1.f) * maskColor + Material.diffuse * (1.f - maskColor);
}

float4 PS_Opacity(MeshOutput input) : SV_TARGET
{
    //if (g_bHasNormalMap)
    //    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    
    //LightColor color = ComputeLight(input.normal, input.uv, input.worldPosition);
    float4 maskColor = OpacityMap.Sample(LinearSampler, input.uv);
    
   
    
    return float4(1.f, 1.f, 1.f, maskColor.r);
}

technique11 T0
{
    pass p0
    {
        SetRasterizerState(RS_Default);
        SetBlendState(BlendDefault, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(DSS_Default, 0);
        SetVertexShader(CompileShader(vs_5_0, VS_Water()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

};
