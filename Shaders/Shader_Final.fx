#include "Render.fx"
#include "Light.fx"

float g_AberrationPower;

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

float4 PS_LumainanceToneMap(VS_OUT input) : SV_Target0
{
   // float4 output = SubMap0.Sample(LinearSampler, input.uv);
    float4 output = (float4) 0.f;

    output = SubMap0.Sample(LinearSampler, input.uv);
   // output.rgb = pow(output.rgb, g_gamma);
    output.rgb *= g_brightness;
    
    output.rgb = ((output.rgb - 0.5f) * g_contrast) + 0.5f;
    
    output.rgb = reinhard_extended_luminance(output.rgb,g_max_white);
    return output;
}

float4 PS_Uncharted2TMO(VS_OUT input) : SV_Target0
{
   // float4 output = SubMap0.Sample(LinearSampler, input.uv);
    float4 output = (float4) 0.f;

    output = SubMap0.Sample(LinearSampler, input.uv);
   // output.rgb = pow(output.rgb, g_gamma);
    output.rgb *= g_brightness;
    
    output.rgb = ((output.rgb - 0.5f) * g_contrast) + 0.5f;
        
    output.rgb = uncharted2_filmic(output.rgb);
    
    return output;
}

float4 PS_ACESTMO(VS_OUT input) : SV_Target0
{
   // float4 output = SubMap0.Sample(LinearSampler, input.uv);
    float4 output = (float4) 0.f;

    output = SubMap0.Sample(LinearSampler, input.uv);
    //output.rgb = pow(output.rgb, g_gamma);
    output.rgb *= g_brightness;
    
    output.rgb = ((output.rgb - 0.5f) * g_contrast) + 0.5f;
    
    output.rgb = pow(output.rgb, 1.f / GAMMA);
    
    output.rgb = aces_fitted(output.rgb);
    return output;
}

float4 PS_Aberration(VS_OUT input) : SV_Target
{
    uint width, height, numMips;
    SubMap0.GetDimensions(0, width, height, numMips);
    
    float2 texel = float2(1.f / (float) width, 1.f / (float) height);
   // float2 coords = (input.uv - 0.5f) * 2.f;
    
    float2 coords = abs(input.uv - 0.5f) * 2.f;
    coords *= -1.f;
    coords += 0.5f;
    
    float coordDot = dot(coords, coords);
    
    float2 preCompute = g_AberrationPower * coordDot * coords;
    float2 uvR = input.uv - texel.xy * preCompute;
    float2 uvB = input.uv + texel.xy * preCompute;
    
    float4 color;
    color.r = SubMap0.Sample(LinearSamplerClamp, uvR).r;
    color.g = SubMap0.Sample(LinearSamplerClamp, input.uv).g;
    color.b = SubMap0.Sample(LinearSamplerClamp, uvB).b;
    color.a = 1.f;
    
    return color;
    
}



technique11 T0
{
    Pass brightcontrast
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Final()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Final()));
    }
    Pass LumainanceToneMap
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Final()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_LumainanceToneMap()));
    }
    Pass Uncharted2TMO
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Final()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Uncharted2TMO()));
    }
    Pass ACESTMO
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Final()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_ACESTMO()));
    }
};

technique11 Aberration
{
    Pass pass_Aberration
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Final()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Aberration()));
    }
  
};