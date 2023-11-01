#include "Render.fx"
#include "Light.fx"

struct TrailOut
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

TrailOut VS_Main(VTXMesh input)
{
    TrailOut output = (TrailOut) 0.f;
    
    //if(g_int_0 == 0)
    //    output.position = mul(float4(input.position, 1.f), W);
    //else if(g_int_0 == 1)
        output.position = float4(input.position, 1.f);
    
    output.position = mul(output.position, VP);
    
    output.uv = input.uv;
       
    
    return output;
}

float4 PS_MAIN(TrailOut input) : SV_Target
{
    float4 finalColor = (float4) 0.f;
    
    float4 diffuseColor = Material.diffuse;
    float4 emissiveColor = Material.emissive;
    float opacity = 1.f;
    if (bHasDiffuseMap)
        diffuseColor.xyz *= DiffuseMap.Sample(LinearSampler, input.uv).xyz;
    
    if (bHasOpacityMap)
    {
        opacity = OpacityMap.Sample(LinearSampler, input.uv).x;
        if(opacity < 0.2f)
            discard;
    }
    
    if (bHasEmissiveMap)
    {
        emissiveColor.xyz *= EmissiveMap.Sample(LinearSampler, input.uv).xyz;
    }
   
    finalColor.xyz = diffuseColor.xyz + emissiveColor.xyz;
    finalColor.w = diffuseColor.w * opacity;
    return finalColor;
}

technique11 T0
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_LESS_NO_WRITE, 0);

        SetPixelShader(CompileShader(ps_5_0, PS_MAIN()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
    
}