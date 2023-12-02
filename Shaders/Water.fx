//#include "Render.fx"
//#include "Light.fx"



//struct VS_OUT
//{
//    float4 position : SV_POSITION;
//    float2 uv : TEXCOORD;
//};

//MeshOutput VS_Water(VTXMesh input)
//{
//    MeshOutput output;
    
//    output.position = mul(float4(input.position, 1.f), W);
//    output.worldPosition = output.position.xyz;
    
//    output.viewNormal = mul(input.normal, (float3x3) W);
//    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
//    output.viewTangent = mul(input.normal, (float3x3) W);
//    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
//    output.position = mul(output.position, VP);
//    output.uv = input.uv;
//    return output;
//}

//float4 PS(MeshOutput input) : SV_TARGET
//{
//    //if (g_bHasNormalMap)
//    //    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    
//    //LightColor color = ComputeLight(input.normal, input.uv, input.worldPosition);
  
//    return DiffuseMap.Sample(LinearSampler, input.uv);
//}

//float4 PS_Masked(MeshOutput input) : SV_TARGET
//{ 
//    //if (g_bHasNormalMap)
//    //    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    
//    //LightColor color = ComputeLight(input.normal, input.uv, input.worldPosition);
//    float4 maskColor = OpacityMap.Sample(LinearSampler, input.uv);
    
   
    
//    return float4(1.f, 1.f, 1.f, 1.f) * maskColor + Material.diffuse * (1.f - maskColor);
//}

//float4 PS_Opacity(MeshOutput input) : SV_TARGET
//{
//    //if (g_bHasNormalMap)
//    //    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    
//    //LightColor color = ComputeLight(input.normal, input.uv, input.worldPosition);
//    float4 maskColor = OpacityMap.Sample(LinearSampler, input.uv);
    
   
    
//    return float4(1.f,1.f,1.f,maskColor.r);
//}

//technique11 T0
//{
//    pass p0
//    {													
//        SetRasterizerState(RS_Default);
//        SetBlendState(BlendDefault,float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);						
//      	SetDepthStencilState(DSS_Sky,0);	                   
//        SetVertexShader(CompileShader(vs_5_0, VS()));
//        SetPixelShader(CompileShader(ps_5_0, PS()));
//    }
//    pass p1
//    {
//        SetRasterizerState(RS_Default);
//        SetBlendState(BlendDefault, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
//        SetDepthStencilState(DSS_Sky, 0);
//        SetVertexShader(CompileShader(vs_5_0, VS()));
//        SetPixelShader(CompileShader(ps_5_0, PS_Opacity()));
//    }
//};
