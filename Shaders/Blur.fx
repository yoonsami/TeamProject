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

static const float Weight[9] =
{
    0.075, 0.125, 0.075,
  0.125, 0.200, 0.125,
  0.075, 0.125, 0.075,
};

static const float Total = 1.;

//Mesh : Rect
VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;
    
    return output;
}

float4 PS_DOWN(VS_OUT input) : SV_Target0
{
    float4 Out = 0;

    uint m_TexW, m_TexH, numMips;
    SubMap0.GetDimensions(0, m_TexW, m_TexH, numMips);
    

    float tu = 1.f / m_TexW;
    float tv = 1.f / m_TexH;

    float3 a = SubMap0.Sample(LinearSampler, input.uv + float2(-2.f * tu, 2.f * tv)).rgb;
    float3 b = SubMap0.Sample(LinearSampler, input.uv + float2(0.f, 2 * tv)).rgb;
    float3 c = SubMap0.Sample(LinearSampler, input.uv + float2(2.f * tu, 2.f * tv)).rgb;
    
    float3 d = SubMap0.Sample(LinearSampler, input.uv + float2(-2.f * tu, 0.f)).rgb;
    float3 e = SubMap0.Sample(LinearSampler, input.uv + float2(0.f, 0.f)).rgb;
    float3 f = SubMap0.Sample(LinearSampler, input.uv + float2(2.f * tu, 0.f)).rgb;
    
    float3 g = SubMap0.Sample(LinearSampler, input.uv + float2(-2.f * tu, -2.f * tv)).rgb;
    float3 h = SubMap0.Sample(LinearSampler, input.uv + float2(0.f, -2.f * tv)).rgb;
    float3 i = SubMap0.Sample(LinearSampler, input.uv + float2(2.f * tu, -2.f * tv)).rgb;

    float3 j = SubMap0.Sample(LinearSampler, input.uv + float2(-tu,+tv)).rgb;
    float3 k = SubMap0.Sample(LinearSampler, input.uv + float2(+tu,+tv)).rgb;
    float3 l = SubMap0.Sample(LinearSampler, input.uv + float2(-tu,-tv)).rgb;
    float3 m = SubMap0.Sample(LinearSampler, input.uv + float2(+tu,-tv)).rgb;
    
    float3 vDownSample = e * 0.125;
    vDownSample += (a + c + g + i) * 0.03125;
    vDownSample += (b + d + f + h) * 0.0625;
    vDownSample += (j + k + l + m) * 0.125;
    
    Out = float4(vDownSample, 1.f);

    return Out;
    
    
}

float4 PS_UP(VS_OUT input) : SV_Target0
{
    float4 Out = 0;

    uint m_TexW, m_TexH, numMips;
    SubMap0.GetDimensions(0, m_TexW, m_TexH, numMips);
    

    float tu = 1.f / m_TexW;
    float tv = 1.f / m_TexH;

    float3 a = SubMap0.Sample(LinearSampler, clamp(float2(input.uv.x - tu, input.uv.y + tv), 0, 1)).rgb;
    float3 b = SubMap0.Sample(LinearSampler, clamp(float2(input.uv.x,      input.uv.y + tv), 0, 1)).rgb;
    float3 c = SubMap0.Sample(LinearSampler, clamp(float2(input.uv.x + tu, input.uv.y + tv), 0, 1)).rgb;
    
    float3 d = SubMap0.Sample(LinearSampler, clamp(float2(input.uv.x - tu, input.uv.y), 0, 1)).rgb;
    float3 e = SubMap0.Sample(LinearSampler, clamp(float2(input.uv.x,      input.uv.y), 0, 1)).rgb;
    float3 f = SubMap0.Sample(LinearSampler, clamp(float2(input.uv.x + tu, input.uv.y), 0, 1)).rgb;
    
    float3 g = SubMap0.Sample(LinearSampler, clamp(float2(input.uv.x - tu, input.uv.y - tv), 0, 1)).rgb;
    float3 h = SubMap0.Sample(LinearSampler, clamp(float2(input.uv.x,      input.uv.y - tv), 0, 1)).rgb;
    float3 i = SubMap0.Sample(LinearSampler, clamp(float2(input.uv.x + tu, input.uv.y - tv), 0, 1)).rgb;
    
    float3 vUpSample = e * 4.f;
    vUpSample += (b + d + f + h) * 2.f;
    vUpSample += (a + c + g + i);
    vUpSample *= 1.f / 16.f;
    Out = float4(vUpSample, 1.f);

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
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_DOWN()));
    }
    pass bigger
    {
        SetVertexShader(CompileShader(vs_5_0, VS_MAIN()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_UP()));
    }
}