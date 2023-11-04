#include "Render.fx"
#include "Light.fx"
#define PI 3.141592f
float g_BarPercent;
float4 g_DrawColor;
struct VS_OUT
{
    float4 viewPos : POSITION;
    float2 uv : TEXCOORD;
};

UIOutput VS_UI(VTXMesh input)
{
    UIOutput output;
    output.position = mul(float4(input.position, 1.f), W);
    output.position = mul(output.position, VP);
    output.uv = input.uv;

    return output;
}

VS_OUT VS_Default(VTXMesh input)
{
    VS_OUT output;
    float3 worldPos = mul(float4(input.position, 1.f), W).xyz;
    output.viewPos = mul(float4(worldPos, 1.f), V);
    
    output.uv = input.uv;

    return output;
}

UIInstancingOutput VS_Instancing(VTXMeshInstancing input)
{
    UIInstancingOutput output;
    output.position = mul(float4(input.position, 1.f), input.world);
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.id = input.instanceID;
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
//g_int_0 spriteCountX
//g_int_1 spriteCountY
//g_vec2_0 curtime,lifetime
//g_vec2_1 sizexy
//g_vec4_0.x > 0.5f = HasDissolve
//g_vec4_0.y > 0.5f = Sprited
//g_vec4_0.z = dissolveSpeed
//g_vec4_0.w = MaintainTime

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
    
    output[0].position = vtx.viewPos + float4(-scale.x, scale.y, 0.f, 0.f);
    output[1].position = vtx.viewPos + float4(scale.x, scale.y, 0.f, 0.f);
    output[2].position = vtx.viewPos + float4(scale.x, -scale.y, 0.f, 0.f);
    output[3].position = vtx.viewPos + float4(-scale.x, -scale.y, 0.f, 0.f);

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


[maxvertexcount(6)]
void GS_Sprite(point VS_OUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4] =
    {
        (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f
    };
    
    float3 trans, mscale;
    float4 q;
    decompose(W, trans, q, mscale);
        
    VS_OUT vtx = input[0];
    float2 scale = g_vec2_1 * mscale.x;
    
    output[0].position = vtx.viewPos + float4(-scale.x, scale.y, 0.f, 0.f);
    output[1].position = vtx.viewPos + float4(scale.x, scale.y, 0.f, 0.f);
    output[2].position = vtx.viewPos + float4(scale.x, -scale.y, 0.f, 0.f);
    output[3].position = vtx.viewPos + float4(-scale.x, -scale.y, 0.f, 0.f);

    output[0].viewPos = output[0].position.xyz;
    output[1].viewPos = output[1].position.xyz;
    output[2].viewPos = output[2].position.xyz;
    output[3].viewPos = output[3].position.xyz;
                                          
    // proj space
    for (int i = 0; i < 4; ++i)
        output[i].position = mul(output[i].position, P);

    float ratio = g_vec2_0.x / g_vec2_0.y;

    uint spriteCountX = g_int_0;
    uint spriteCountY = g_int_1;
    uint curFrame = 0;

    curFrame = uint(ratio * spriteCountX * spriteCountY);

    
    int indexX = curFrame % spriteCountX;
    int indexY = curFrame / spriteCountX;


    output[0].uv = float2(indexX * 1.f / spriteCountX, indexY * 1.f / spriteCountY);
    output[1].uv = float2((indexX + 1) * 1.f / spriteCountX, indexY * 1.f / spriteCountY);
    output[2].uv = float2((indexX + 1) * 1.f / spriteCountX, (indexY + 1) * 1.f / spriteCountY);
    output[3].uv = float2(indexX * 1.f / spriteCountX, (indexY + 1) * 1.f / spriteCountY);
   
    
    
    
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

float4 PS_UI(UIOutput input) : SV_TARGET
{

    float4 diffuseColor = g_vec4_0;
    if(bHasDiffuseMap)
        diffuseColor = DiffuseMap.Sample(LinearSamplerMirror, input.uv ) * g_vec4_0;
    
    if (bHasOpacityMap)
    {
        diffuseColor.a = OpacityMap.Sample(LinearSamplerMirror, input.uv).x * g_vec4_0.w;
        if(diffuseColor.a <= 0.01f)
            discard;
    }
    
    if(g_int_0 == 1 && g_float_0 < 100.f)
    {
        if(1.f - input.uv.y >= g_float_0 / 100.f)
            diffuseColor.xyz *= 0.2f;
        
        float gauge_Color = SubMap0.Sample(LinearSampler, input.uv + float2(0.f, 0.5f+ g_float_0 / 100.f)).x;
        diffuseColor.xyz += gauge_Color;

    }
    
    else if(g_int_0 == 2 && g_float_0 < 100.f)
    {
        //[-0.5f,0.5f]
        float2 uvPos = float2(input.uv.x - 0.5f, 0.5f - input.uv.y);
        // X가 0에 가까울 때
        float theta = atan2(uvPos.x, uvPos.y);
        
        if(theta <=0.f)
            theta += 2.f * PI;

        if (theta >= 2.f * PI * g_float_0 / 100.f)
        {
            diffuseColor.xyz *= 0.2f;
        }
        else
        {
            diffuseColor.xyz = (DiffuseMap.Sample(LinearSamplerMirror, input.uv) * g_vec4_0).xyz;
        }
        

        
        float c = cos(2.f * PI * g_float_0 / 100.f);
        float s = sin(2.f * PI * g_float_0 / 100.f);
        
        float2 rotatedUV;
        rotatedUV.x = uvPos.x * c - uvPos.y * s;
        rotatedUV.y = uvPos.y * c + uvPos.x * s;
        
        rotatedUV.x += 0.5f;
        rotatedUV.y = (rotatedUV.y - 0.5f) * -1.f;

        float gauge_Color = SubMap0.Sample(LinearSampler, rotatedUV).x;
        diffuseColor.xyz += gauge_Color;


    }
    else if(g_int_0 == 2 && g_float_0>=100.f)
    {
        diffuseColor.xyz = (DiffuseMap.Sample(LinearSamplerMirror, input.uv) * g_vec4_0).xyz;
    }
    else if (g_int_0 == 3 && g_float_0 < 100.f)
    {
        float2 newUV;
        float ratio = (g_float_0 / 100.f) * g_vec2_0.x + g_vec2_0.y;
        newUV.x = input.uv.x + (ratio - 0.5f) * -1.f;
        newUV.y = input.uv.y;
        float3 submap = SubMap0.Sample(LinearSamplerClamp, newUV).xyz;
        diffuseColor.xyz *= submap.xyz;
        diffuseColor.a *= submap.x;
        
    }
    else if (g_int_0 == 4 )
    {
        if (input.uv.x >= g_float_0 / 100.f)
            diffuseColor = float4(0.3f, 0.3f, 0.3f, 1.f);
    }
    
    
    return diffuseColor;
}

float4 PS_UIInstancing(UIInstancingOutput input) : SV_TARGET
{

    float4 diffuseColor = InstanceRenderParams[input.id].g_vec4_0;
    if (bHasDiffuseMap)
        diffuseColor = DiffuseMap.Sample(LinearSamplerMirror, input.uv) * InstanceRenderParams[input.id].g_vec4_0;
    
    if (bHasOpacityMap)
    {
        diffuseColor.a = OpacityMap.Sample(LinearSamplerMirror, input.uv).x * InstanceRenderParams[input.id].g_vec4_0.w;
        if (diffuseColor.a <= 0.01f)
            discard;
    }
    
    if (InstanceRenderParams[input.id].g_int_0 == 1 && InstanceRenderParams[input.id].g_float_0 < 100.f)
    {
        if (1.f - input.uv.y >= InstanceRenderParams[input.id].g_float_0 / 100.f)
            diffuseColor.xyz *= 0.2f;
        
        float gauge_Color = SubMap0.Sample(LinearSampler, input.uv + float2(0.f, 0.5f + InstanceRenderParams[input.id].g_float_0 / 100.f)).x;
        diffuseColor.xyz += gauge_Color;

    }
    else if (InstanceRenderParams[input.id].g_int_0 == 2 && InstanceRenderParams[input.id].g_float_0 < 100.f)
    {
        //[-0.5f,0.5f]
        float2 uvPos = float2(input.uv.x - 0.5f, 0.5f - input.uv.y);
        // X가 0에 가까울 때
        float theta = atan2(uvPos.x, uvPos.y);
        
        if (theta <= 0.f)
            theta += 2.f * PI;

        if (theta >= 2.f * PI * InstanceRenderParams[input.id].g_float_0 / 100.f)
        {
            diffuseColor.xyz *= 0.2f;
        }
        else
        {
            diffuseColor.xyz = (DiffuseMap.Sample(LinearSamplerMirror, input.uv) * InstanceRenderParams[input.id].g_vec4_0).xyz;
        }
        

        
        float c = cos(2.f * PI * InstanceRenderParams[input.id].g_float_0 / 100.f);
        float s = sin(2.f * PI * InstanceRenderParams[input.id].g_float_0 / 100.f);
        
        float2 rotatedUV;
        rotatedUV.x = uvPos.x * c - uvPos.y * s;
        rotatedUV.y = uvPos.y * c + uvPos.x * s;
        
        rotatedUV.x += 0.5f;
        rotatedUV.y = (rotatedUV.y - 0.5f) * -1.f;

        float gauge_Color = SubMap0.Sample(LinearSampler, rotatedUV).x;
        diffuseColor.xyz += gauge_Color;


    }
    else if (InstanceRenderParams[input.id].g_int_0 == 2 && InstanceRenderParams[input.id].g_float_0 >= 100.f)
    {
        diffuseColor.xyz = (DiffuseMap.Sample(LinearSamplerMirror, input.uv) * InstanceRenderParams[input.id].g_vec4_0).xyz;
    }
    else if (InstanceRenderParams[input.id].g_int_0 == 3 && InstanceRenderParams[input.id].g_float_0 < 100.f)
    {
        float2 newUV;
        float ratio = (InstanceRenderParams[input.id].g_float_0 / 100.f) * InstanceRenderParams[input.id].g_vec2_0.x + InstanceRenderParams[input.id].g_vec2_0.y;
        newUV.x = input.uv.x + (ratio - 0.5f) * -1.f;
        newUV.y = input.uv.y;
        float3 submap = SubMap0.Sample(LinearSamplerClamp, newUV).xyz;
        diffuseColor.xyz *= submap.xyz;
        diffuseColor.a *= submap.x;
        
    }
    else if (InstanceRenderParams[input.id].g_int_0 == 4)
    {
        if (input.uv.x >= InstanceRenderParams[input.id].g_float_0 / 100.f)
            diffuseColor = float4(0.3f, 0.3f, 0.3f, 1.f);
    }
    
    
    return diffuseColor;
}


float4 PS_UIBAR(UIOutput input) : SV_TARGET
{
    float4 diffuseColor = DiffuseMap.Sample(LinearSamplerMirror, input.uv);
    
    if(input.uv.x >= g_BarPercent / 100.f)
        diffuseColor = diffuseColor * float4(0.1, 0.1, 0.1, 0.1);
    
    return diffuseColor;
}

//g_float_0 dissolveTime

float4 PS_CustomEffect1(GS_OUTPUT input) : SV_Target
{
    float4 color = (float4) 0.f;
    float2 distortionUV = (float2)0.f;
    float3 viewNormal = (float3) 0.f;
    if(bHasDistortionMap != 0)
    {
        distortionUV = DistortionMap.Sample(LinearSampler, input.uv).xy * g_vec2_2;
    }
    LightColor totalLightColor = (LightColor) 0.f;
    float4 diffuseColor= Material.diffuse;
    float4 emissiveColor = Material.emissive;
    float opacity = 1.f;
    if(bHasDiffuseMap)
        diffuseColor.xyz *= DiffuseMap.Sample(LinearSampler, input.uv + distortionUV).xyz;
    
    //if(g_bHasNormalMap)
    //{
    //    viewNormal = NormalMap.Sample(LinearSampler, input.uv + distortionUV).xyz;
        
    //    for (uint i = 0; i < lightCount; ++i)
    //    {
    //        LightColor color = CalculateLightColor_ViewSpace(i, viewNormal, input.viewPos);
    //        totalLightColor.ambient += color.ambient;
    //        totalLightColor.diffuse += color.diffuse;
    //        totalLightColor.specular += color.specular;
    //        totalLightColor.emissive += color.emissive;
            
    //    }

    //}
    //else
    //    totalLightColor = (LightColor) 1.f;
    
    if(bHasOpacityMap)
    {
        opacity = OpacityMap.Sample(LinearSampler, input.uv + distortionUV).x;
    }
    
    if(bHasEmissiveMap)
    {
        emissiveColor.xyz *= EmissiveMap.Sample(LinearSampler, input.uv + distortionUV).xyz;
    }
   
    color.xyz = diffuseColor.xyz 
    + emissiveColor.xyz;
    color.w = opacity * diffuseColor.a;
    
    if(bHasDissolveMap != 0)
    {
        float dissolveColor = DissolveMap.Sample(LinearSampler, input.uv + distortionUV).x;
        if (dissolveColor < g_float_0)
            discard;
    }
    
    
    return color;
}


float4 PS_FRAME(UIOutput input) : SV_TARGET
{
    return g_DrawColor;
}

float4 PS_Test(UIOutput input) :SV_TARGET
{
    float2 uvSliding = g_vec2_0;
    
    float4 color = (float4) 0.f;
    float2 distortionUV = (float2) 0.f;
    float3 viewNormal = (float3) 0.f;
    
    if (bHasSubmap0 != 0)
    {
        distortionUV = SubMap0.Sample(LinearSampler, input.uv + uvSliding).xy * g_vec2_1;
    }
    
    float4 diffuseColor = g_vec4_0;
    float4 emissiveColor = g_vec4_1;
    float opacity = 1.f;
    
    if (bHasDiffuseMap)
        diffuseColor.xyz *= DiffuseMap.Sample(LinearSampler, input.uv + distortionUV).xyz;
        
    if (bHasOpacityMap)
    {
        opacity = OpacityMap.Sample(LinearSampler, input.uv + distortionUV).x;
    }
    
    if (bHasEmissiveMap)
    {
        emissiveColor.xyz *= EmissiveMap.Sample(LinearSampler, input.uv + distortionUV).xyz;
    }
   
    color.xyz = diffuseColor.xyz 
    + emissiveColor.xyz;
    color.w = opacity * diffuseColor.a;
    
    return color;
}

float4 PS_CustomEffect2(GS_OUTPUT input) : SV_TARGET
{
    float2 uvSliding = g_vec2_0;
    
    float4 color = (float4) 0.f;
    float2 distortionUV = (float2) 0.f;
    float3 viewNormal = (float3) 0.f;
    
    if (bHasSubmap0 != 0)
    {
        distortionUV = SubMap0.Sample(LinearSampler, input.uv + uvSliding).xy * g_vec2_2;
    }
    
    float4 diffuseColor = Material.diffuse;
    float4 emissiveColor = Material.emissive;
    float opacity = 1.f;
    
    if (bHasDiffuseMap)
        diffuseColor.xyz *= DiffuseMap.Sample(LinearSampler, input.uv + distortionUV).xyz;
        
    if (bHasOpacityMap)
    {
        opacity = OpacityMap.Sample(LinearSampler, input.uv + distortionUV).x;
    }
    
    if (bHasEmissiveMap)
    {
        emissiveColor.xyz *= EmissiveMap.Sample(LinearSampler, input.uv + distortionUV).xyz;
    }
   
    color.xyz = diffuseColor.xyz 
    + emissiveColor.xyz;
    color.w = opacity * diffuseColor.a;
    
    return color;
}

float4 PS_CustomEffect3(GS_OUTPUT input) : SV_TARGET
{

    float4 color = (float4) 0.f;
    float distortion = 0.f;
    
    if (bHasDistortionMap != 0)
    {
        distortion = DistortionMap.Sample(LinearSampler, input.uv).x * g_vec2_2.x;
        
        
        
    }
    
    float2 centerUV = float2(0.5f, 0.5f);
    float2 dir = normalize(centerUV - input.uv);
    float dist = length(centerUV - input.uv);
    float f = exp(distortion * (dist - 0.5f)) - 1.f;
    if (dist > 0.5)
        f = 0.f;
    
    float2 newUV = input.uv + f * dir;
    
    float4 diffuseColor = Material.diffuse;
    float4 emissiveColor = Material.emissive;
    float opacity = 1.f;
    
    if (bHasDiffuseMap)
        diffuseColor.xyz *= DiffuseMap.Sample(LinearSampler, newUV).xyz;
        
    if (bHasOpacityMap)
    {
        opacity = OpacityMap.Sample(LinearSampler, newUV).x;
    }
    
    if (bHasEmissiveMap)
    {
        emissiveColor.xyz *= EmissiveMap.Sample(LinearSampler, newUV).xyz;
    }
   
     color.xyz = diffuseColor.xyz 
                 + emissiveColor.xyz;
                 color.w = opacity * diffuseColor.a;
     
     return color;
}
float4 PS_CustomEffect4(GS_OUTPUT input) : SV_TARGET
{
    float4 diffuseColor = DiffuseMap.Sample(LinearSampler,input.uv).aaaa * Material.diffuse;

     
    return diffuseColor;
}

float4 PS_Test2(GS_OUTPUT input) : SV_TARGET
{
    float2 uvSliding = g_vec2_0;
    
    float4 color = (float4) 0.f;
    float2 distortionUV = (float2) 0.f;
    float3 viewNormal = (float3) 0.f;
    
    if (bHasSubmap0 != 0)
    {
        distortionUV = SubMap0.Sample(LinearSampler, input.uv + uvSliding).xy * g_vec2_1;
    }
    
    float4 diffuseColor = g_vec4_0;
    float4 emissiveColor = g_vec4_1;
    float opacity = 1.f;
    
    if (bHasDiffuseMap)
        diffuseColor.xyz *= DiffuseMap.Sample(LinearSampler, input.uv + distortionUV).xyz;
        
    if (bHasOpacityMap)
    {
        opacity = OpacityMap.Sample(LinearSampler, input.uv + distortionUV).x;
    }
    
    if (bHasEmissiveMap)
    {
        emissiveColor.xyz *= EmissiveMap.Sample(LinearSampler, input.uv + distortionUV).xyz;
    }
   
    color.xyz = diffuseColor.xyz 
    + emissiveColor.xyz;
    color.w = opacity * diffuseColor.a;
    
    return color;
}


technique11 T0
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_UI()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_UI()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(NULL);

    }
    PASS_VP(p1_instancing, VS_Instancing, PS_UIInstancing)
    PASS_VP_BLEND(P2, VS_UI, PS_UIBAR)

    pass p3
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Default()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_CustomEffect1()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(CompileShader(gs_5_0, GS_Sprite()));

    }    
    pass p4
    {
        SetVertexShader(CompileShader(vs_5_0, VS_UI()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Test()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(NULL);

    }
    pass p5
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Default()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_CustomEffect2()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(CompileShader(gs_5_0, GS_Sprite()));

    }
    pass p6
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Default()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_CustomEffect3()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(CompileShader(gs_5_0, GS_Sprite()));

    }
    pass p7
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Default()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_CustomEffect4()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(CompileShader(gs_5_0, GS_Sprite()));

    }
    pass p8
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Default()));
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_Default, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_Test2()));
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(CompileShader(gs_5_0, GS_Main()));

    }
};

technique11 T1
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_UI()));
        SetRasterizerState(FillModeWireFrame);
        SetDepthStencilState(DSS_LESS_NO_WRITE, 0);
        SetPixelShader(CompileShader(ps_5_0, PS_FRAME()));
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetGeometryShader(NULL);

    }

};