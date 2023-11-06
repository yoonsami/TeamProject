#include "Render.fx"
#include "Light.fx"

float g_brightness;
float g_contrast;

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

VS_OUT VS_Final(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;

    return output;
}

float4 PS_Final(VS_OUT input) : SV_Target0
{
   // float4 output = SubMap0.Sample(LinearSampler, input.uv);
    float4 output = (float4) 0.f;

    output = SubMap0.Sample(LinearSampler, input.uv);
    
    output.rgb *= g_brightness;
    
    output.rgb = ((output.rgb - 0.5f) * g_contrast) + 0.5f;
    
    
    return output;
}



technique11 T0
{
    Pass P7
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Final()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Final()));
    }

};
