#include "Render.fx"
#include "Light.fx"

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

VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;
    
    return output;
}

float4 PS_DOWN(VS_OUT input) : SV_Target0
{
    float4 Out = SubMap0.Sample(LinearSampler,input.uv);

    

    return Out;

}


technique11 t0
{
    pass smaller
    {
        SetVertexShader(CompileShader(vs_5_0, VS_MAIN()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(AdditiveBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_DOWN()));
    }
}