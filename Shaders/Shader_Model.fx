#include "Render.fx"
#include "Light.fx"

MeshOutput VS_NonAnim(VTXModel input)
{
    MeshOutput output;
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    output.newUV.x = (output.worldPosition.x + 256.f)/512.f;
    output.newUV.y = (256.f - output.worldPosition.z) / 512.f;

    return output;
}

MeshOutput VS_Anim(VTXModel input)
{
    MeshOutput output;
    
    matrix m = GetAnimationMatrix(input);

    output.position = mul(float4(input.position, 1.f), m);

    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) m);
    output.viewNormal = mul(output.viewNormal, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    output.newUV.x = (output.worldPosition.x + 256.f) / 512.f;
    output.newUV.y = (256.f - output.worldPosition.z) / 512.f;
    return output;
}

MeshOutput VS_NonAnimInstancing(VTXModelInstancing input)
{
    MeshOutput output;
    
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, input.world);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    output.newUV.x = (output.worldPosition.x + 256.f) / 512.f;
    output.newUV.y = (256.f - output.worldPosition.z) / 512.f;
    return output;
}

MeshOutput VS_AnimInstancing(VTXModelInstancing input)
{
    MeshOutput output;

    matrix m = GetAnimationMatrix_Instance(input);
    
    output.position = mul(float4(input.position, 1.f), m);
    output.position = mul(output.position, input.world);
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.viewNormal = mul(input.normal, (float3x3) m);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    output.newUV.x = (output.worldPosition.x + 256.f) / 512.f;
    output.newUV.y = (256.f - output.worldPosition.z) / 512.f;
    return output;
}


// OutLine
float4 g_LineColor;
float g_LineThickness;
MeshOutput VS_NonAnimOutline(VTXModel input)
{
    MeshOutput output;
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) W));
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
   // output.position += float4(normalize(output.viewNormal) * g_LineThickness, 0.f);
    
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, V);
    output.position += float4(normalize(output.viewNormal) * g_LineThickness * output.viewPosition.z, 0.f);
    output.position = mul(output.position, P);
    
    output.uv = input.uv;
    
    output.newUV.x = (output.worldPosition.x + 256.f) / 512.f;
    output.newUV.y = (256.f - output.worldPosition.z) / 512.f;
    
   // output.viewPosition = output.viewPosition + output.viewNormal * g_LineThickness;

    return output;
}

MeshOutput VS_AnimOutline(VTXModel input)
{
    MeshOutput output;
    
    matrix m = GetAnimationMatrix(input);

    output.position = mul(float4(input.position, 1.f), m);
    output.position = mul(output.position, W);
    output.viewNormal = normalize(mul(input.normal, (float3x3) m));
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) W));
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) W);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));

   // output.position += float4(normalize(output.viewNormal) * g_LineThickness, 0.f);

    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, V);
    output.position += float4(normalize(output.viewNormal) * g_LineThickness * output.viewPosition.z, 0.f);
    output.position = mul(output.position, P);

    output.uv = input.uv;
    output.newUV.x = (output.worldPosition.x + 256.f) / 512.f;
    output.newUV.y = (256.f - output.worldPosition.z) / 512.f;
    
   // output.viewPosition = output.viewPosition + output.viewNormal * g_LineThickness;
    
    return output;
}

MeshOutput VS_NonAnimInstancingOutline(VTXModelInstancing input)
{
    MeshOutput output;
    
    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.position = mul(output.position, input.world);
    output.viewNormal = mul(input.normal, (float3x3) BoneTransform[BoneIndex]);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewTangent = mul(input.tangent, (float3x3) BoneTransform[BoneIndex]);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    //output.position += float4(output.viewNormal * g_LineThickness, 0.f);
    
    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;
    output.position = mul(output.position, V);
    output.position += float4(normalize(output.viewNormal) * g_LineThickness * output.viewPosition.z, 0.f);
    output.position = mul(output.position, P);

    output.uv = input.uv;
    output.newUV.x = (output.worldPosition.x + 256.f) / 512.f;
    output.newUV.y = (256.f - output.worldPosition.z) / 512.f;
    
  //  output.viewPosition = output.viewPosition + output.viewNormal * g_LineThickness;
    return output;
}

MeshOutput VS_AnimInstancingOutline(VTXModelInstancing input)
{
    MeshOutput output;

    matrix m = GetAnimationMatrix_Instance(input);
    
    output.position = mul(float4(input.position, 1.f), m);
    output.position = mul(output.position, input.world);
    output.viewNormal = mul(input.normal, (float3x3) m);
    output.viewNormal = mul(output.viewNormal, (float3x3) input.world);
    output.viewTangent = mul(input.tangent, (float3x3) m);
    output.viewTangent = mul(output.viewTangent, (float3x3) input.world);
    output.viewNormal = normalize(mul(output.viewNormal, (float3x3) V));
    output.viewTangent = normalize(mul(output.viewTangent, (float3x3) V));
    
    //output.position += float4(output.viewNormal * g_LineThickness, 0.f);

    output.worldPosition = output.position.xyz;
    output.viewPosition = mul(float4(output.worldPosition, 1.f), V).xyz;

        output.position = mul(output.position, V);
    output.position += float4(normalize(output.viewNormal) * g_LineThickness * output.viewPosition.z, 0.f);
    output.position = mul(output.position, P);
    output.uv = input.uv;
    output.newUV.x = (output.worldPosition.x + 256.f) / 512.f;
    output.newUV.y = (256.f - output.worldPosition.z) / 512.f;
   // output.viewPosition = output.viewPosition + output.viewNormal * g_LineThickness;
    return output;
}


PS_OUT_Deferred PS_Deferred(MeshOutput input)
{
    PS_OUT_Deferred output = (PS_OUT_Deferred) 0.f;
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    float4 emissiveColor;
    
    float2 distortedUV = input.uv;
    
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < g_float_0)
            discard;
    }
    
    
    if (bHasNormalMap)
        ComputeNormalMapping_ViewSpace(input.viewNormal, input.viewTangent, input.uv);
    
    
    
    
    if (bHasDiffuseMap)
        diffuseColor = DiffuseMap.Sample(LinearSampler, input.uv);
    else
        diffuseColor = Material.diffuse;
    

    
    //if (g_bHasFileterMap)
    //{
    //    if (g_bHasMaskMap)
    //    {
    //        float4 maskColor = MaskMap.Sample(LinearSampler, input.newUV);
            
    //        float4 filterColor = FilterMap.Sample(LinearSampler, input.uv);
            
    //        diffuseColor = diffuseColor * maskColor + filterColor * (1.f - maskColor);
    //    }
    //}
    
    if (bHasSpecularMap)
        specularColor = SpecularMap.Sample(LinearSampler, input.uv);
    else
        specularColor = Material.specular;
    
    if (bHasEmissiveMap)
        emissiveColor = EmissiveMap.Sample(LinearSampler, input.uv);
    else
        emissiveColor = Material.emissive;

    ambientColor = diffuseColor;
    diffuseColor = diffuseColor;
    
    output.position = float4(input.viewPosition.xyz, 0.f);
    output.normal = float4(input.viewNormal.xyz, 0.f);
    output.ambientColor = ambientColor;
    output.diffuseColor = diffuseColor;
    output.specularColor = specularColor;
    output.emissiveColor = emissiveColor;
    output.blurColor = (float4) 0.f;
    return output;
}

struct OutlineOutput
{
    float4 ambientColor : SV_Target2;
    float4 diffuseColor : SV_Target3;
    float4 specularColor : SV_Target4;
    float4 emissiveColor : SV_Target5;
};

OutlineOutput PS_Deferred_Outline(MeshOutput input)
{
    OutlineOutput output = (OutlineOutput) 0.f;
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < g_float_0)
            discard;
    }
    output.ambientColor = g_LineColor;
    output.diffuseColor = g_LineColor;
    output.specularColor = g_LineColor;
    output.emissiveColor = g_LineColor;
    
    return output;
}

float4 PS_FRAME(VertexOutput input) : SV_TARGET
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    return color;
}

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 clipPos : POSITION1;
    float4 worldPos : POSITION2;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Shadow_NonAnim(VTXModel input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.pos = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);
    output.pos = mul(output.pos, W);
    output.worldPos = output.pos;
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
    output.worldPos = output.pos;
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
    output.worldPos = output.pos;
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
    output.worldPos = output.pos;
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

technique11 T0
{
    pass NonAnim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
    pass NonAnim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimInstancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
    pass Anim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Anim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
    pass Anim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_AnimInstancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
    pass NonAnim_NonInstancing_CullNone
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
    pass NonAnim_Instancing_CullNone
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimInstancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred()));
    }
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

    PASS_RS_VP(P4_WIREFRAME, FillModeWireFrame, VS_NonAnim, PS_FRAME)
    PASS_RS_VP(P6_WIREFRAME, FillModeWireFrame, VS_NonAnimInstancing, PS_FRAME)
};

technique11 t1
{
    pass NonAnim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimOutline()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CW);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Outline()));
    }
    pass NonAnim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimInstancingOutline()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CW);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Outline()));
    }
    pass Anim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_AnimOutline()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CW);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Outline()));
    }
    pass Anim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_AnimInstancingOutline()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CW);
        SetDepthStencilState(DSS_LESS, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Deferred_Outline()));
    }
};

technique11 T2
{
    pass NonAnim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(NULL);
    }
    pass NonAnim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnimInstancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(NULL);
    }
    pass Anim_NonInstancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Anim()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(NULL);
    }
    pass Anim_Instancing
    {
        SetVertexShader(CompileShader(vs_5_0, VS_AnimInstancing()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(NULL);
    }
};