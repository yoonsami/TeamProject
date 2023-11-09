#include "Render.fx"
#include "Light.fx"

int g_SeedCount = 7;
int g_CirclesCount = 20;
float g_fCircleSize = 1.f / 20.f;
float3 col1 = float3(0.2f, 0.1f, 0.f);
float3 col2 = float3(0.9f,1.f,1.f);
float fadeRadius = 0.7f;
float overeallOpacity = 5.f;
int bokehIterations = 5;
float bokehRadius = 0.4;

float easeOutQuad(float x)
{
    return x * x;
}

float random(float seed)
{
    return frac(sin(seed) * 100000.0);
}

float getPointDist(float2 point1, float2 point2)
{
    float x = point2.x - point1.x;
    float y = point2.y - point1.y;
    return length(float2(x, y));
}

float3 hsv2rgb(float3 c)
{
    float4 K = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    float3 p = abs(frac(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * lerp(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float3 drawRainbowCircle(float2 circlePos, float2 pixelPos, float radius)
{
    float dist = getPointDist(circlePos, pixelPos) / radius;
    if (dist > 1.)
        return float3(0, 0, 0);
    
    float colorMul = (dist - 0.4) / (dist + 1.);
    float3 col = hsv2rgb(float3(colorMul * 8., 0.5, 1));
    col *= colorMul;
    col = max(col, 0.);
    if (dist > 0.5)
        col *= 1. - dist;
    
    return col;
}

float3 drawCircle(float2 circlePos, float2 pixelPos, float radius, float falloff, float3 startColor, float3 endColor)
{
    float dist = getPointDist(circlePos, pixelPos) / radius;
    if (dist > 1.)
        return float3(0, 0, 0);
    
    float colorMul = (dist - falloff) / (dist + 1.);
    float3 col = lerp(startColor, endColor, colorMul);
    col *= colorMul;
    col = max(col, 0.);
    
    return col;
}

float2 getFlarePoint(float2 centerPos, float2 centerfloat, float position)
{
    float2 delta = centerfloat * position;
    delta += centerPos;
    return delta;
}

float angleBetween(float2 v1, float2 v2)
{
    return acos(dot(v1, v2) / (length(v1) * length(v2)));
}

#define GOLDEN_ANGLE 2.3999632
#define BLUR 1.
#define PI 3.14159265359 

float2x2 rot = float2x2(cos(GOLDEN_ANGLE), sin(GOLDEN_ANGLE), -sin(GOLDEN_ANGLE), cos(GOLDEN_ANGLE));

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
float2 g_LightPos;
float2 g_TestPos1;
float2 g_TestPos2;
float4 PS_Final(VS_OUT input) : SV_Target0
{
  
    float4 output = (float4) 0.f;

    float3 col = (float3) 0.f;
    
    uint width, height, numMips;
    SubMap1.GetDimensions(0, width, height, numMips);
    
    float2 vLightPos = g_LightPos;
    
    float2 vLightUV;
    vLightUV.x = (vLightPos.x / (float)width);
    vLightUV.y = (vLightPos.y / (float) height);
    
    float fLightDepth = SubMap1.Sample(PointSampler, vLightUV).r;
    float fDepth = SubMap1.Sample(PointSampler, input.uv).r;
    
    float2 centerPos = 2.f * g_LightPos - float2(960.f, 540.f);

    
    float2 centerVec = (vLightPos - centerPos);
    float centerVecLength = length(centerVec);
    
    float centerProximity = centerVecLength / 150.;
    if (centerProximity < 1.)
        centerProximity = 1.;
    
    float opacityMul = centerVecLength / 1920.f / fadeRadius;
    if (opacityMul > 1.)
        opacityMul = 1.;
    opacityMul = 1. - easeOutQuad(1. - opacityMul);
    
    float circleOpacityMul = centerVecLength / 1920.f / 0.2;
    if (circleOpacityMul > 1.)
        circleOpacityMul = 1.;
    
    float3 acc = float3(0,0,0);
    float3 div = acc;
    float r = 1.;
    float2 vangle = float2(0.0, bokehRadius * .01 / sqrt(float(bokehIterations)));
    
    for (int j = 0; j < bokehIterations; j++)
    {
        vangle = mul(rot, vangle);
        float2 pixelPos = (input.uv + (BLUR) * vangle) * float2(1920.f, 1080.f);
        
        float baseRadius = (1. - circleOpacityMul) / 4. + 3. / 4.;
        float circleBoost = 1. + (1. - circleOpacityMul) * 0.1;
        if (fDepth >= 1.f)
        col = drawCircle(getFlarePoint(vLightPos, centerVec, 0.), pixelPos, 1920.f / 1.5 * baseRadius, circleBoost, float3(0, 0, 0), float3(1, 1, 1));
    
        for (int i = 0; i < g_CirclesCount; i++)
        {
            float newSeed = float(g_SeedCount) / (float(i) + 10.);
            float pos = random(newSeed);
            float dist = 1. - pos;
            float radius = 1920.f * g_fCircleSize * random(newSeed * 2.) * dist;
            float falloff = 0.35 + random(newSeed * 3.) * 0.5;
            float opacity = (0.8 + random(newSeed * 4.) * 0.2) * opacityMul * overeallOpacity;
            opacity *= dist;
            
            float2 flarePoint = getFlarePoint(vLightPos, centerVec, pos);
            float2 pointToFlare = flarePoint - pixelPos;
            float angle = angleBetween(pointToFlare, centerVec) / PI;
            angle = easeOutQuad(angle);
            angle = clamp(angle, 0., 1.);
            opacity *= angle;
            opacity /= centerProximity;
            
            if (vLightPos.x >= 1920.f)
            {
                opacity *= saturate(1.f - (vLightPos.x - 1920.f) / 120.f);

            }
            else if (vLightPos.x <= 0.f)
            {
                opacity *= saturate(1 + vLightPos.x / 120.f);
            }
            if (vLightPos.y <= 0.f)
            {
                opacity *= saturate(1 + vLightPos.y / 120.f);
            }
            if (fLightDepth >= 0.999f)
            col += drawCircle(flarePoint, pixelPos, radius, falloff, col1, col2) * opacity;
        }
        
        float2 flarePoint = getFlarePoint(vLightPos, centerVec, 0.25);
        float2 pointToFlare = flarePoint - pixelPos;
        float angle = angleBetween(pointToFlare, centerVec) / PI;
        angle = easeOutQuad(easeOutQuad(angle));
        angle = clamp(angle, 0., 1.);
        angle = lerp(1., angle, opacityMul);
        float opacity = angle * 1.5;
        opacity /= centerProximity / 1.5;
        
        if (vLightPos.x >= 1920.f)
        {
            opacity *= saturate(1.f - (vLightPos.x - 1920.f) / 120.f);

        }
        else if (vLightPos.x <= 0.f)
        {
            opacity *= saturate(1 + vLightPos.x / 120.f);
        }
        if(vLightPos.y <= 0.f)
        {
            opacity *= saturate(1 + vLightPos.y / 120.f);
        }
        
        if (fLightDepth >= 1.f)
        col += drawRainbowCircle(flarePoint, pixelPos, 1920.f / 5.) * opacity;
        
        float2 distToCenter = pixelPos - vLightPos;
        float res = 1920.f / 80.;
        opacity = (1. - opacityMul) * 0.9 + 0.1;
        
        float dist = 1. - (abs(distToCenter.x / 16.) + abs(distToCenter.y * 4.)) / res;
        if (fDepth >= 1.f)
        col += max(dist, 0.) * 0.3 * opacity;
       
        dist = 1. - (abs(distToCenter.x * 4.) + abs(distToCenter.y / 16.)) / res;
        if (fDepth >= 1.f)
        col += max(dist, 0.) * 0.3 * opacity;
        
        res = 1920.f / 80.;
        
        dist = 1. - (abs(distToCenter.x / 9.) + abs(distToCenter.y)) / res;
        if (fDepth >= 1.f)
        col += max(dist, 0.) * 0.3 * opacity;
       
        dist = 1. - (abs(distToCenter.x) + abs(distToCenter.y / 9.)) / res;
        if (fDepth >= 1.f)
        col += max(dist, 0.) * 0.3 * opacity;
        
        
        float3 bokeh = pow(col, float3(4,4,4));
        acc += col * bokeh;
        div += bokeh;
    }
    float power = 1.f;
    if (vLightPos.x >= 1920.f )
    {
        power = saturate(1.f - (vLightPos.x - 1920.f) / 1920.f);
        
    }
    else if(vLightPos.x <= 0.f)
    {
        power = saturate(1 + vLightPos.x / 1920.f);
    }
    else if (vLightPos.y <= 0.f)
    {
        power *= saturate(1 + vLightPos.y / 120.f);
    }
        
    if (length(div) != 0.f)
        output += float4(acc / div, 0.f) * power;

  
    return output;
}

float4 PS_Bloom(VS_OUT input) : SV_Target
{
    float4 output = (float4) 0.f;
    float4 vHDRColor = SubMap0.Sample(LinearSampler, input.uv);
    float4 vBloomOrigin = SubMap2.Sample(LinearSampler, input.uv);
    
    float4 vBloom =vBloomOrigin;
    
    output = vHDRColor;
    
    output = pow(abs(output), 2.2f);
    vBloom = pow(abs(vBloom), 2.2f);
    
    output += vBloom;
    
    return pow(abs(output), 1 / 2.2f);
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
    Pass apply
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Final()));
        SetGeometryShader(NULL);
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_NO_DEPTH_TEST_NO_WRITE, 0);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetPixelShader(CompileShader(ps_5_0, PS_Bloom()));
    }
};
