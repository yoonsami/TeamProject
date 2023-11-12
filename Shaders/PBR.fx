#ifndef _PBR_FX_
#define _PBR_FX_

#include "Global.fx"

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)  // k is a remapping of roughness based on direct lighting or IBL lighting
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;

    //float k = (roughness * roughness) / 2.0f;

    float nom = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return nom / denom;
}

float GeometrySmith(float3 n, float3 v, float3 l, float k)
{
    //  Geometry Obstruction
    float NdotV = saturate(dot(n, v));
    //  Geometry Shadowing
    float NdotL = saturate(dot(n, l));

    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

// Diffuse
float3 CalculateLambertianBRDF(in float3 albedo)
{ //램버시안 표면의 난반사 BRDF계산
    return max(albedo / PI, 0.0);

}

//Specular
float NormalDistributionGGXTR(float3 n, float3 h, float a)
{
    float a2 = a * a;
    float NdotH = saturate(dot(n, h));
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}


float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return (F0 + (1.0f - F0) * pow(1.0 - cosTheta, 5.0f));
}
#endif