#include "Render.fx"
#include "Light.fx"

Texture2D RTV;

UIOutput VS_FINAL(VTXMesh input)
{
    UIOutput output;
    output.position = mul(float4(input.position, 1.f), W);
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    
    return output;
}

float4 PS_UI(UIOutput input) : SV_TARGET
{
    float4 diffuseColor = DiffuseMap.Sample(LinearSampler, input.uv);
    
    if(input.uv.x > 0.5f)
        diffuseColor.ra = float2(1.f, 0.5f);
    
    
    return diffuseColor;
}


technique11 T0
{
    Pass P7
    {
        SetVertexShader(CompileShader(vs_5_0, VS_FINAL()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_UI()));
    }

};
