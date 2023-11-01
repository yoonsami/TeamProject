#include "Render.fx"
#include "Light.fx"

struct VS_OUT
{
    float4 pos : SV_Position;
    float4 clipPos : POSITION;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Shadow_NonAnim(VTXModel input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.pos = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    
    output.pos = mul(output.pos, W);
    output.pos = mul(output.pos, VP);
    output.clipPos = output.pos;
    output.uv = input.uv;
    return output;
}

VS_OUT VS_Shadow_NonAnim_Instancing(VTXModelInstancing input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.pos = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.pos = mul(output.pos, input.world);
    output.pos = mul(output.pos, VP);
    output.clipPos = output.pos;
    output.uv = input.uv;
    return output;
}

VS_OUT VS_Shadow_Anim(VTXModel input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    matrix m = GetAnimationMatrix(input);
    
    output.pos = mul(float4(input.position, 1.f), m);
    
    output.pos = mul(output.pos, W);
    output.pos = mul(output.pos, VP);
    output.clipPos = output.pos;
    output.uv = input.uv;
    return output;
}

VS_OUT VS_Shadow_Anim_Instancing(VTXModelInstancing input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    matrix m = GetAnimationMatrix_Instance(input);
    
    output.pos = mul(float4(input.position, 1.f), m);
    
    output.pos = mul(output.pos, input.world);
    output.pos = mul(output.pos, VP);
    output.clipPos = output.pos;
    output.uv = input.uv;
    return output;
}

float4 PS_Shadow(VS_OUT input) : SV_Target
{
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < g_float_0)
            discard;
    }
    return float4(input.clipPos.z / input.clipPos.w, 0.f, 0.f, 0.f);
}

technique11 t0
{
    pass NonAnimShadow
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Shadow_NonAnim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Shadow()));
    }
    pass NonAnimShadowInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Shadow_NonAnim_Instancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Shadow()));
    }
    pass AnimShadow
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Shadow_Anim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Shadow()));
    }
    pass AnimShadowInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Shadow_Anim_Instancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Shadow()));
    }
    
}