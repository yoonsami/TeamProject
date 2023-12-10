#include "Render.fx"
#include "Light.fx"
MeshOutput VS_Grass(VTXMesh input)
{
    MeshOutput output;
    output.position = float4(input.position, 1.f);
    output.uv = input.uv;
    output.worldPosition = mul(float4(input.position, 1.f), W).xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.viewNormal = mul(float4(input.normal, 0.f), V).xyz;
    output.viewTangent = mul(float4(input.tangent, 0.f), V).xyz;
    
    return output;
}
struct GS_GRASS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 viewPosition : POSITION2;
    float2 uv : TEXCOORD;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
};

[maxvertexcount(18)]
void GS_Grass(point MeshOutput input[1], inout TriangleStream<GS_GRASS_OUTPUT> outputStream)
{
    const uint vertexCount = 4;
    const uint billboardCount = 3;
    GS_GRASS_OUTPUT output[vertexCount * billboardCount] =
    {
        (GS_GRASS_OUTPUT) 0.f, (GS_GRASS_OUTPUT) 0.f, (GS_GRASS_OUTPUT) 0.f, (GS_GRASS_OUTPUT) 0.f,
        (GS_GRASS_OUTPUT) 0.f, (GS_GRASS_OUTPUT) 0.f, (GS_GRASS_OUTPUT) 0.f, (GS_GRASS_OUTPUT) 0.f,
        (GS_GRASS_OUTPUT) 0.f, (GS_GRASS_OUTPUT) 0.f, (GS_GRASS_OUTPUT) 0.f, (GS_GRASS_OUTPUT) 0.f
    };
    
    for (uint j = 0; j < billboardCount; j++)
    {
        float4x4 matRotateByBillboard = RotateMatrix(radians(60.f * j), float3(0.f, 1.f, 0.f));
        float4x4 RotateWByBillboard = mul(matRotateByBillboard, W);
        //RotateWByBillboard[3].xyz += RotateWByBillboard[2].xyz * 10.f;
        MeshOutput vtx = input[0];
        //float2 scale = mscale.xy * 0.5f;
        
        
        
        output[j * 4 + 0].position = float4(vtx.position.xyz + matRotateByBillboard[2].xyz - matRotateByBillboard[0].xyz * 0.5f + matRotateByBillboard[1].xyz * 0.5f, 1.f);
        output[j * 4 + 1].position = float4(vtx.position.xyz + matRotateByBillboard[2].xyz + matRotateByBillboard[0].xyz * 0.5f + matRotateByBillboard[1].xyz * 0.5f, 1.f);
        output[j * 4 + 2].position = float4(vtx.position.xyz + matRotateByBillboard[2].xyz + matRotateByBillboard[0].xyz * 0.5f - matRotateByBillboard[1].xyz * 0.5f, 1.f);
        output[j * 4 + 3].position = float4(vtx.position.xyz + matRotateByBillboard[2].xyz - matRotateByBillboard[0].xyz * 0.5f - matRotateByBillboard[1].xyz * 0.5f, 1.f);
        
        
        
        output[j * 4 + 0].uv = float2(0.f, 0.f);
        output[j * 4 + 1].uv = float2(1.f, 0.f);
        output[j * 4 + 2].uv = float2(1.f, 1.f);
        output[j * 4 + 3].uv = float2(0.f, 1.f);
    
        output[j * 4 + 0].viewPosition = mul(output[j * 4 + 0].position, mul(W,V));
        output[j * 4 + 1].viewPosition = mul(output[j * 4 + 1].position, mul(W,V));
        output[j * 4 + 2].viewPosition = mul(output[j * 4 + 2].position, mul(W,V));
        output[j * 4 + 3].viewPosition = mul(output[j * 4 + 3].position, mul(W, V));

    // proj q
        output[j * 4 + 0].position = mul(float4(output[j * 4 + 0].viewPosition, 1.f), P);
        output[j * 4 + 1].position = mul(float4(output[j * 4 + 1].viewPosition, 1.f), P);
        output[j * 4 + 2].position = mul(float4(output[j * 4 + 2].viewPosition, 1.f), P);
        output[j * 4 + 3].position = mul(float4(output[j * 4 + 3].viewPosition, 1.f), P);
        
        output[j * 4 + 0].viewNormal = mul(RotateWByBillboard[2], V).xyz;
        output[j * 4 + 1].viewNormal = mul(RotateWByBillboard[2], V).xyz;
        output[j * 4 + 2].viewNormal = mul(RotateWByBillboard[2], V).xyz;
        output[j * 4 + 3].viewNormal = mul(RotateWByBillboard[2], V).xyz;
        
        output[j * 4 + 0].viewTangent = mul(RotateWByBillboard[0], V).xyz;
        output[j * 4 + 1].viewTangent = mul(RotateWByBillboard[0], V).xyz;
        output[j * 4 + 2].viewTangent = mul(RotateWByBillboard[0], V).xyz;
        output[j * 4 + 3].viewTangent = mul(RotateWByBillboard[0], V).xyz;

        outputStream.Append(output[j * 4 + 0]);
        outputStream.Append(output[j * 4 + 1]);
        outputStream.Append(output[j * 4 + 2]);
        outputStream.RestartStrip();

        outputStream.Append(output[j * 4 + 0]);
        outputStream.Append(output[j * 4 + 2]);
        outputStream.Append(output[j * 4 + 3]);
        outputStream.RestartStrip();
    }
}

technique11 T0
{
    pass GeometryWeed
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Grass()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Terrain()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(CompileShader(gs_5_0, GS_Grass()));
    }
//19
    pass UIModel
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Default()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_ViewPort()));
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(CompileShader(gs_5_0, GS_Ui()));
    }
};
