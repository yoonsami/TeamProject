#include "Render.fx"
#include "Light.fx"

#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FXAA_QUALITY__PRESET 39
#define FXAA_GREEN_AS_LUMA 1
#include "Fxaa3_11.h"
float4 RCPFrame;
bool Use;

struct VS_Output
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
};

VS_Output VS(uint id : SV_VertexID)
{
    //  W : 동촤 -> 0 : 방향 , 1 : 위치
    VS_Output output;

    output.Uv = float2((id << 1) & 2, id & 2);
    
    output.Position.x = output.Uv.x * 2 - 1;
    output.Position.y = output.Uv.y * -2 + 1;
    output.Position.z = 0.0f;
    output.Position.w = 1.0f;

    return output; // 반환값이 픽셀의 위치
}

SamplerState InputSampler
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
    MipLodBias = 0.0f;
    MaxAnisotropy = 1;
    ComparisonFunc = ALWAYS;
};

float4 PS(VS_Output input) : SV_TARGET
{
    float4 color = 1;
    if (Use == true)
    {
        FxaaTex InputFXAATex = { InputSampler, SubMap0 };
        
        color = FxaaPixelShader(
        input.Uv.xy, // FxaaFloat2 pos,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsolePosPos,
        InputFXAATex, // FxaaTex tex,
        InputFXAATex, // FxaaTex Luma,
        InputFXAATex, // FxaaTex fxaaConsole360TexExpBiasNegOne,
        InputFXAATex, // FxaaTex fxaaConsole360TexExpBiasNegTwo,
        RCPFrame.xy, // FxaaFloat2 fxaaQualityRcpFrame,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsoleRcpFrameOpt,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsoleRcpFrameOpt2,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsole360RcpFrameOpt2,
        0.75f, // FxaaFloat fxaaQualitySubpix,
        0.166f, // FxaaFloat fxaaQualityEdgeThreshold,
        0.0833f, // FxaaFloat fxaaQualityEdgeThresholdMin,
        0.0f, // FxaaFloat fxaaConsoleEdgeSharpness,
        0.0f, // FxaaFloat fxaaConsoleEdgeThreshold,
        0.0f, // FxaaFloat fxaaConsoleEdgeThresholdMin,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f) // FxaaFloat fxaaConsole360ConstDir,
    );
    }
    else
    {
        int3 sampleIndices = int3(input.Position.xy, 0);
        color = SubMap0.Load(sampleIndices);
    }

    return color;
}

technique11 T0
{
    Pass brightcontrast
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

};
