#ifndef _LIGHT_FX_
#define _LIGHT_FX_

#include "Global.fx"


////////////
// Struct //
////////////

struct LightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float3 direction;
    float padding;
};

struct MaterialDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
};

/////////////////
// ConstBuffer //
/////////////////

cbuffer LightBuffer
{
    LightDesc GlobalLight;
};

cbuffer MaterialBuffer
{
    MaterialDesc Material;
};

/////////
// SRV //
/////////

Texture2D DiffuseMap;
Texture2D NormalMap;
Texture2D SpecularMap;

//////////////
// Function //
//////////////

float4 ComputeLight(float3 normal, float2 uv, float3 worldPosition)
{
    float4 ambientColor = 0;
    float4 diffuseColor = 0;
    float4 specularColor = 0;
    float4 emissiveColor = 0;
    float3 cameraPosition = CameraPosition();
    float3 E = normalize(cameraPosition - worldPosition);
	// Ambient
    ambientColor = DiffuseMap.Sample(LinearSampler, uv) * GlobalLight.ambient * Material.ambient;

	// Diffuse
	{
        diffuseColor = DiffuseMap.Sample(LinearSampler, uv) * saturate(dot(-GlobalLight.direction, normalize(normal))) * GlobalLight.diffuse * Material.diffuse;
    }

	// Specular
    float4 specularMap = float4(1.f,1.f,1.f,1.f);
   // if(g_bHasSpecularMap)
       // specularMap = SpecularMap.Sample(LinearSampler, uv);
	{
        float3 R = normalize(reflect(GlobalLight.direction, normal));
       //float3 R = normalize(GlobalLight.direction - (2 * normal * dot(GlobalLight.direction, normal)));

        float specular = pow(saturate(dot(R, E)), 10);

        specularColor = GlobalLight.specular * Material.specular * specular;
    }

	// Emissive


    emissiveColor = GlobalLight.emissive * Material.emissive * pow(smoothstep(0.0f, 1.0f, 1.0f - saturate(dot(E, normal))), 2);

    // 
    return ambientColor + diffuseColor + emissiveColor + specularColor;
}

void ComputeNormalMapping(inout float3 normal, float3 tangent, float2 uv)
{
	// [0,255] 범위에서 [0,1]로 변환
    float4 map = NormalMap.Sample(LinearSampler, uv);
 
    //map.b = sqrt(1 - saturate(dot(map.rg, map.rg)));
    
    float3 N = normalize(normal); // z
    float3 T = normalize(tangent); // x
    float3 B = normalize(cross(N, T)); // y
    float3x3 TBN = float3x3(T, B, N); // TS -> WS

	// [0,1] 범위에서 [-1,1] 범위로 변환
    float3 tangentSpaceNormal = (map.rgb * 2.0f - 1.0f);
    float3 worldNormal = normalize(mul(tangentSpaceNormal, TBN));

    normal = worldNormal;
}

#endif