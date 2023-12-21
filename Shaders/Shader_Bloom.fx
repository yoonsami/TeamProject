#include "Render.fx"
#include "Light.fx"
int g_BlurCount;
float DownScalePower;
float UpScalePower;
float g_BloomMin;
struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


//Mesh : Rect
VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;
    
    return output;
}

float4 PS_ExtractBloom(VS_OUT input) : SV_Target0
{
    float4 output = (float4) 0.f;
    float3 color = SubMap0.Sample(LinearSampler, input.uv).rgb;
    float brightness = luminance(color);
    if (brightness > g_BloomMin)
        output = float4(color.rgb, 1.f);
    
    return output;
}

float4 PS_Bloom(VS_OUT input) : SV_Target
{
    float4 output = (float4) 0.f;
    float4 vHDRColor = SubMap0.Sample(LinearSampler,input.uv);
    float4 vBloomColor = SubMap1.Sample(LinearSampler, input.uv);
    float4 vBloomOrigin = SubMap2.Sample(LinearSampler, input.uv);
    
    float4 vBloom = vBloomColor +vBloomOrigin;
    
    output = vHDRColor;
    

    vBloom =vBloomColor;
    
    output.xyz += vBloom.xyz;
    
    return output;
}

technique11 t0
{
    pass extract
    {
        SetVertexShader(CompileShader(vs_5_0, VS_MAIN()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_ExtractBloom()));
    }
    pass apply
    {
        SetVertexShader(CompileShader(vs_5_0, VS_MAIN()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Bloom()));
    }
}