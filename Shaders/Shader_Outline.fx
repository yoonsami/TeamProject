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

float mask[9] =
{
    -1, -1, -1,
    -1, 8, -1,
    -1, -1 , -1
};

float coord[3] = { -1, 0, 1 };
float divider = 1;



//Mesh : Rect
VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;
    
    return output;
}

float4 PS_Laplacian(VS_OUT input) : SV_Target0
{
    float4 Out = 0;

    float4 Color = 0.f;
    float3 grayScale = float3(0.3f, 0.59f, 0.11f);
    
    uint width, height, numMips;
    SubMap0.GetDimensions(0, width, height, numMips);
    
    float MAP_CX =(float) width;
    float MAP_CY = (float) height;
    
    for (uint i = 0; i < 9; ++i)
        Color += mask[i] * (SubMap0.Sample(PointSampler, input.uv + float2(coord[i % 3] / MAP_CX, coord[i / 3] / MAP_CY)));
    
    float gray = 1 - dot(Color.rgb, grayScale);
    
    Out = float4(gray, gray, gray, 1) / divider;
    
    return Out;
}

float4 PS_Apply(VS_OUT input) : SV_Target
{
    float4 Out = 0;
    float3 OutLineColor = SubMap0.Sample(LinearSampler, input.uv).rgb;
    float3 FinalColor = SubMap1.Sample(LinearSampler, input.uv).rgb;
    
    Out = float4(OutLineColor * FinalColor, 1.f);
    return Out;
}

technique11 t0
{
    pass Laplacian
    {
        SetVertexShader(CompileShader(vs_5_0, VS_MAIN()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Laplacian()));
    }
    pass Apply
    {
        SetVertexShader(CompileShader(vs_5_0, VS_MAIN()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Apply()));
    }
}
