#include "Render.fx"
#include "Light.fx"
#include "PBR.fx"


MeshOutput VS_Terrain(VTXMesh input)
{
    MeshOutput output;
    
    output.position = mul(float4(input.position, 1.f), W);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.viewNormal = mul(input.normal, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    return output;
}

PS_OUT_Deferred PS_Deferred(MeshOutput input)
{
    PS_OUT_Deferred output = (PS_OUT_Deferred) 0.f;
    float4 specularColor;
    float4 emissiveColor;
    
    //if (bHasNormalMap)
    //{
    //    ComputeNormalMapping_ViewSpace(input.viewNormal, input.viewTangent, input.uv);
        
    //}
    float4 maskColor = TextureMap7.Sample(LinearSampler, float2(input.uv.x / g_vec2_0.x, input.uv.y / g_vec2_0.y));
    float4 diffuseColor = 1.f;
    
    // 길이 있으면 길색깔잡초색깔 비율맞춤
    if (maskColor.g > 0)
    {
        float4 RoadColor = TextureMap8.Sample(LinearSampler, input.uv);
        float4 tempDiffuseColor = DiffuseMap.Sample(LinearSampler, input.uv);
        
        RoadColor = mul(RoadColor, maskColor.g);
        tempDiffuseColor = mul(tempDiffuseColor, 1 - maskColor.g);
        
        diffuseColor = RoadColor + tempDiffuseColor;
    }
    else
        diffuseColor = DiffuseMap.Sample(LinearSampler, input.uv);
    
    ComputeNormalMapping_ViewSpace(input.viewNormal, input.viewTangent, input.uv);
    
    // 브러시관련
    float3 vBrushPos = g_vec4_0.xyz;
    float fBrushRadius = g_float_0;
    float4 colBrushColor = float4(0.f, 0.f, 0.f, 0.f);
    
    vBrushPos.y = input.worldPosition.y; // 높이는 같다고 가정.
    
    // 브러시포지션과 거리가 브러시Radius이하라면 색깔을 흰색으로 함.
    if (length((input.worldPosition) - vBrushPos) <= fBrushRadius)
    {
        colBrushColor = float4(0.2f, 0.2f, 0.2, 0.f);
    }
    diffuseColor = diffuseColor + colBrushColor;
    
    if (bHasSpecularMap)
    {
        specularColor = SpecularMap.Sample(LinearSampler, input.uv);
        specularColor.rgb = pow(abs(specularColor.rgb), GAMMA);
    }
    else
        specularColor = Material.specular;
   
    if (bHasEmissiveMap)
    {
        emissiveColor = EmissiveMap.Sample(LinearSampler, input.uv);
        emissiveColor.rgb = pow(abs(emissiveColor.rgb), GAMMA);
    }
    else
        emissiveColor = Material.emissive;
    
    output.position = float4(input.viewPosition.xyz, 0.f);
    output.normal = float4(input.viewNormal.xyz, 0.f);
    output.depth = input.position.z;
    output.depth.w = input.viewPosition.z;
    output.diffuseColor = diffuseColor;
    output.specularColor = specularColor;
    output.emissiveColor = emissiveColor;
    return output;
}

PBR_MAPOBJECT_OUTPUT PS_Terrain_PBR(MeshOutput input)
{
    PBR_MAPOBJECT_OUTPUT output = (PBR_MAPOBJECT_OUTPUT) 0.f;
    float4 color;
    
    float4 maskColor = TextureMap7.Sample(LinearSampler, float2(input.uv.x / g_vec2_0.x, input.uv.y / g_vec2_0.y));
    float4 diffuseColor = 1.f;
    
    // 길이 있으면 길색깔잡초색깔 비율맞춤
    if (maskColor.g > 0)
    {
        float4 RoadColor = TextureMap8.Sample(LinearSampler, input.uv);
        float4 tempDiffuseColor = DiffuseMap.Sample(LinearSampler, input.uv);
        
        RoadColor = mul(RoadColor, maskColor.g);
        tempDiffuseColor = mul(tempDiffuseColor, 1 - maskColor.g);
        
        diffuseColor = RoadColor + tempDiffuseColor;
    }
    else
        diffuseColor = DiffuseMap.Sample(LinearSampler, input.uv);
        
    // 브러시관련
    float3 vBrushPos = g_vec4_0.xyz;
    float fBrushRadius = g_float_0;
    float4 colBrushColor = float4(0.f, 0.f, 0.f, 0.f);
    
    vBrushPos.y = input.worldPosition.y; // 높이는 같다고 가정.
    
    // 브러시포지션과 거리가 브러시Radius이하라면 색깔을 흰색으로 함.
    if (length((input.worldPosition) - vBrushPos) <= fBrushRadius)
    {
        colBrushColor = float4(0.2f, 0.2f, 0.2, 0.f);
    }
    color = diffuseColor + colBrushColor;
    
    if (bHasNormalMap)
        ComputeNormalMapping_ViewSpace(input.viewNormal, input.viewTangent, input.uv);

    
    
    output.position = float4(input.viewPosition.xyz, 1.f);
    output.normal = float4(input.viewNormal.xyz, 0.f);
    output.positionSSD = float4(input.viewPosition.xyz, 1.f);
    output.arm = float4(1.f, 0.8f, 0.0f, 1.f);
    output.diffuseColor = color;
    output.emissive = 0.f;
    output.rimColor = Material.emissive;
    output.blur = 0;
    return output;

}


technique11 T0
{
    pass Terrain_Deferred
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Terrain()));
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(NULL);
    }
    pass Terrain_Deferred_PBR
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Terrain()));
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Terrain_PBR()));
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(NULL);
    }

};
