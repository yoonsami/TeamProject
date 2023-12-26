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

VS_OUT VS_Final(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;

    return output;
}

//SubMap0 : FinalTarget
//SubMap1 : DistortionTarget
//SubMap2 : DistortionTarget1
float4 PS_Final(VS_OUT input) : SV_Target0
{
   // float4 output = SubMap0.Sample(LinearSampler, input.uv);
    float4 output = (float4) 0.f;
    float4 submap1 = SubMap1.Sample(LinearSamplerMirror, input.uv);

    float2 newUV = input.uv + submap1.x;
    
    output = SubMap0.Sample(LinearSamplerMirror, newUV);
    
   // output = pow(output, 2.2);
    
    
    
    return output;
}

float4 PS_TEST(VS_OUT input) : SV_Target0
{
    
   // float4 output = SubMap0.Sample(LinearSampler, input.uv);
    float4 output = (float4) 0.f;
    float4 submap1 = SubMap1.Sample(LinearSampler, input.uv);
    float distortion = submap1.x;
    
    float2 centerUV = SubMap2.Sample(LinearSampler, input.uv).xy;
    float size = submap1.z;
    float2 dir = normalize(centerUV - input.uv);
    float dist = length(centerUV - input.uv);
    float factor = submap1.y - 0.5f;
    float f = exp(factor * (dist - size)) - size * 2.f;
    if (dist > size)
        f = 0;
    
    
    
    float2 newUV = input.uv + f * dir;
    
    output = SubMap0.Sample(LinearSampler, newUV);
    
    return output;
}

technique11 t0
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Final()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Final()));
    }
}