#include "Render.fx"
#include "Light.fx"

float g_ShadowBias;

float CalcShadowFactor(float4 shadowPosH)
{
  //w로 나누어서 투영을 완료한다.
    shadowPosH.xyz /= shadowPosH.w;
    float2 uv = shadowPosH.xy;
    uv.y = -uv.y;
    uv = uv * 0.5f + 0.5f;
  //NDC 공간 기준의 깊이 값
    float depth = shadowPosH.z;

    uint width, height, numMips;
    SubMap2.GetDimensions(0, width, height, numMips);

  //텍셀 크기
    float dx = 1.0f / (float) width;

    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
    float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
    float2(-dx, dx), float2(0.0f, dx), float2(dx, dx)
    };
    depth -= g_ShadowBias;
  [unroll]
    for (int i = 0; i < 9; ++i)
    {  
        percentLit += SubMap2.SampleCmpLevelZero(samShadow, uv + offsets[i], depth).r;
    }
     percentLit /= 9.0f;
    
    
    return percentLit;
}

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

struct PS_LIGHT_Deferred
{
    float4 ambient : SV_Target0;
    float4 diffuse : SV_Target1;
    float4 specular : SV_Target2;
    float4 emissive : SV_Target3;
};

int lightIndex;
//SubMap0 : PositionRT
//SubMap1 : NormalRT
//SubMap2 : ShadowRT
float2 RenderTargetResolution;
float4x4 ShadowCameraVP;

//Mesh : Rect
VS_OUT VS_DirLight(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;
    
    return output;
}

PS_LIGHT_Deferred PS_DirLight(VS_OUT input)
{
    PS_LIGHT_Deferred output = (PS_LIGHT_Deferred) 0.f;
    
    float3 viewPos = SubMap0.Sample(LinearSampler, input.uv).xyz;
    if(viewPos.z <= 0.f)
        clip(-1);
    
    float3 viewNormal = SubMap1.Sample(LinearSampler, input.uv).xyz;
    
    LightColor color = CalculateLightColor_ViewSpace(lightIndex, viewNormal, viewPos);
  
    if(length(color.diffuse) !=0)
    {
        float4 worldPos = mul(float4(viewPos.xyz, 1.f), VInv);
        float4 shadowClipPos = mul(worldPos, ShadowCameraVP);
        float shadowFactor = CalcShadowFactor(shadowClipPos);


        color.diffuse = color.diffuse * shadowFactor;

      
    }


    output.ambient = color.ambient;
    output.ambient.a = 1.f;
    output.diffuse = color.diffuse;
    output.diffuse.a = 1.f;
    output.specular = color.specular;
    output.specular.a = 1.f;
    output.emissive = color.emissive;
    output.emissive.a = 1.f;
    
    return output;
}

//Mesh : Sphere
VS_OUT VS_PointLight(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.pos = mul(float4(input.pos, 1.f), W);
    output.pos = mul(output.pos, VP);
    output.uv = input.uv;
    
    
    return output;
}

PS_LIGHT_Deferred PS_PointLight(VS_OUT input)
{
    PS_LIGHT_Deferred output = (PS_LIGHT_Deferred) 0.f;
    
    // input.pos = SV_Position = Screen좌표
    float2 uv = float2(input.pos.x / RenderTargetResolution.x, input.pos.y / RenderTargetResolution.y);
    
    float3 viewPos = SubMap0.Sample(LinearSampler, uv).xyz;
    if (viewPos.z <= 0.f)
        clip(-1);
    
    float3 viewLightPos = mul(float4(lights[lightIndex].vPosition.xyz, 1.f), V).xyz;
    float distance = length(viewPos - viewLightPos);
    if(distance > lights[lightIndex].range)
        clip(-1);
    
    float3 viewNormal = SubMap1.Sample(LinearSampler, uv).xyz;
    
    LightColor color = CalculateLightColor_ViewSpace(lightIndex, viewNormal, viewPos);
    output.ambient = color.ambient;
    output.ambient.a = 1.f;
    output.diffuse = color.diffuse;
    output.diffuse.a = 1.f;
    output.specular = color.specular;
    output.specular.a = 1.f;
    output.emissive = color.emissive;
    output.emissive.a = 1.f;
    
    return output;
}

VS_OUT VS_SpotLight(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float4x4 scale = IDENTITY_MATRIX;
    float halfAngle = lights[lightIndex].angle / 2.f;
    
    float scaleRatio = 1.f/cos(halfAngle);
    
    m_scale(scale, float3(scaleRatio, scaleRatio, scaleRatio));
    
    output.pos = mul(float4(input.pos, 1.f), mul(scale, W));
    output.pos = mul(output.pos, VP);
    output.uv = input.uv;
    
    
    return output;
}

PS_LIGHT_Deferred PS_SpotLight(VS_OUT input)
{
    PS_LIGHT_Deferred output = (PS_LIGHT_Deferred) 0.f;
    
    // input.pos = SV_Position = Screen좌표
    float2 uv = float2(input.pos.x / RenderTargetResolution.x, input.pos.y / RenderTargetResolution.y);
    
    float3 viewPos = SubMap0.Sample(LinearSampler, input.uv).xyz;
    if (viewPos.z <= 0.f)
        clip(-1);
    
    float3 viewLightPos = mul(float4(lights[lightIndex].vPosition.xyz, 1.f), V).xyz;
    float distance = length(viewPos - viewLightPos);
    
    float halfAngle = lights[lightIndex].angle / 2.f;
    
    float scaleRatio = 1.f / cos(halfAngle);
    
    if (distance > lights[lightIndex].range * scaleRatio)
        clip(-1);
    
    float3 viewNormal = SubMap1.Sample(LinearSampler, input.uv).xyz;
    
    LightColor color = CalculateLightColor_ViewSpace(lightIndex, viewNormal, viewPos);
    output.ambient = color.ambient;
    output.ambient.a = 1.f;
    output.diffuse = color.diffuse;
    output.diffuse.a = 1.f;
    output.specular = color.specular;
    output.specular.a = 1.f;
    output.emissive = color.emissive;
    output.emissive.a = 1.f;
    
    return output;
}

//Final

//SubMap1 : DiffuseColor
//SubMap2 : SpecularColor
//SubMap3 : EmissiveColor
//SubMap4 : AmbientLightColor
//SubMap5 : DiffuseLightColor
//SubMap6 : SpecularLightColor
//SubMap7 : EmissiveLightColor

VS_OUT VS_Final(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;

    return output;
}

float g_gamma;

float4 PS_Final(VS_OUT input) : SV_Target
{
    float4 output = (float4) 0.f;
    
    float4 diffuseColor = SubMap1.Sample(LinearSampler, input.uv);
   // diffuseColor = pow(diffuseColor, 2.2);
    float4 specularColor = SubMap2.Sample(LinearSampler, input.uv);
    //specularColor = pow(specularColor, 2.2);
    float4 emissiveColor = SubMap3.Sample(LinearSampler, input.uv);
  //  emissiveColor = pow(emissiveColor, 2.2);
   
    float4 ambientLightColor = SubMap4.Sample(LinearSampler, input.uv);
  //  ambientLightColor = pow(ambientLightColor, 2.2);
    float4 diffuseLightColor = SubMap5.Sample(LinearSampler, input.uv);
  //  diffuseLightColor = pow(diffuseLightColor, 2.2);
    float4 specularLightColor = SubMap6.Sample(LinearSampler, input.uv);
 //   specularLightColor = pow(specularLightColor, 2.2);
    float4 emissiveLightColor = SubMap7.Sample(LinearSampler, input.uv);
  //  emissiveLightColor = pow(emissiveLightColor, 2.2);
    diffuseLightColor += ambientLightColor;
  //  float4 emissiveBlurColor = SubMap8.Sample(LinearSampler, input.uv);
   // diffuseLightColor = clamp(diffuseLightColor, 0.f, 1.f);
   
    //diffuseColor = pow(diffuseColor, g_gamma);
    
    diffuseColor *= diffuseLightColor;
    specularColor *= specularLightColor;
    emissiveColor *= emissiveLightColor;
    
    output = diffuseColor //pow(diffuseColor, 1.f / g_gamma)
    + specularColor
    + emissiveColor;
   // +emissiveBlurColor * emissiveLightColor;
    
  //  output = pow(output, 1.f/ 2.2f);
    
    output.a = 1.f;
    return output;
}

technique11 t0
{
    pass dirLight
    {
        SetVertexShader(CompileShader(vs_5_0, VS_DirLight()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(AdditiveBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_DirLight()));
    }
    pass pointLight
    {
        SetVertexShader(CompileShader(vs_5_0, VS_PointLight()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(AdditiveBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_PointLight()));
    }
    pass spotLight
    {
        SetVertexShader(CompileShader(vs_5_0, VS_SpotLight()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_CullNone);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(AdditiveBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_SpotLight()));
    }
    pass Final
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Final()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Final()));
    }
}