#include "Render.fx"
#include "Light.fx"
float g_BarPercent;
struct VS_OUT
{
    float4 viewPos : POSITION;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Default(VTXMesh input)
{
    VS_OUT output;
    float3 worldPos = mul(float4(input.position, 1.f), W).xyz;
    output.viewPos = mul(float4(worldPos, 1.f), V);
    
    output.uv = input.uv;

    return output;
}

struct GS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
    float3 viewPos : POSITION1;
};

[maxvertexcount(6)]
void GS_Main(point VS_OUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4] =
    {
        (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f
    };
    
    float3 trans, mscale;
    float4 q;
    decompose(W, trans, q, mscale);
    
    VS_OUT vtx = input[0];
    float2 scale = mscale.xy * 0.5f;
    
    output[0].position = vtx.viewPos + float4(-scale.x * cos(g_float_2) - scale.y * sin(g_float_2), scale.y * cos(g_float_2) - scale.x * sin(g_float_2), 0.f, 0.f);
    output[1].position = vtx.viewPos + float4(scale.x * cos(g_float_2) - scale.y * sin(g_float_2), scale.y * cos(g_float_2) + scale.x * sin(g_float_2), 0.f, 0.f);
    output[2].position = vtx.viewPos + float4(scale.x * cos(g_float_2) + scale.y * sin(g_float_2), -scale.y * cos(g_float_2) + scale.x * sin(g_float_2), 0.f, 0.f);
    output[3].position = vtx.viewPos + float4(-scale.x * cos(g_float_2) + scale.y * sin(g_float_2), -scale.y * cos(g_float_2) - scale.x * sin(g_float_2), 0.f, 0.f);

    output[0].viewPos = output[0].position.xyz;
    output[1].viewPos = output[1].position.xyz;
    output[2].viewPos = output[2].position.xyz;
    output[3].viewPos = output[3].position.xyz;
    
    // proj space
    for (int i = 0; i < 4; ++i)
        output[i].position = mul(output[i].position, P);
    
    output[0].uv = float2(0.f,0.f);
    output[1].uv = float2(1.f,0.f);
    output[2].uv = float2(1.f,1.f);                          
    output[3].uv = float2(0.f,1.f);
   
    for (int j = 0; j < 4; ++j)
    {
        output[j].viewNormal = float3(0.f, 0.f, -1.f);
        output[j].viewTangent = float3(1.f, 0.f, 0.f);
    }
    
    outputStream.Append(output[0]);
    outputStream.Append(output[1]);
    outputStream.Append(output[2]);
    outputStream.RestartStrip();
    
    outputStream.Append(output[0]);
    outputStream.Append(output[2]);
    outputStream.Append(output[3]);
    outputStream.RestartStrip();
}

//g_float_0 dissolveAcc;
//g_float_1 distortionPower;
//g_float_2 turnAngle;
float4 PS_CustomEffect1(GS_OUTPUT input) : SV_Target0
{
    float4 color = (float4) 0.f;
        
    if(bHasDistortionMap)
    {
        color = DistortionMap.Sample(LinearSampler, input.uv);
    }
    
    if(color.r < 0.1f)
        discard;
    
    color.a *= (1.f - g_float_0);
    if(color.a < 0.1f)
        discard;
    

    
    color.r *= g_float_1;

    return color;
}

struct DistortionOut
{
    float4 color : SV_TARGET0;
    float4 option : SV_TARGET1;
};

technique11 T0
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Default()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_CustomEffect1()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(CompileShader(gs_5_0, GS_Main()));
    }
    pass p1
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Default()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_CustomEffect1()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(CompileShader(gs_5_0, GS_Main()));
    }
};
