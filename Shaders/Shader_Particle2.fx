#include "Render.fx"
#include "Light.fx"

float2 StartEndScale;
bool isModel;

struct ParticleInfo_UAV  // 주의: 16byte씩 맞추기
{
    /* 매 프레임마다 변할 수 있는 값들 */
    float3  vWorldPos;      // 입자의 현재 World pos 
    float   fCurrTime;      // 입자가 생성되고나서부터 얼마나 시간이 흘렀는지
    float3  vWorldDir;      // 입자의 현재 방향 
    int     iAlive;         // 입자가 현재 살아있는가. 
    float4  vColor;         // 입자의 color 
    float   fSpeed;         // 입자의 speed
    float   fSpeeeExponent; // 입자의 speed Exponent
    
    /* 입자가 처음 생성될 때 고정되는 값들 */
    float2  vSize;          // 입자의 size 
    float   fLifeTime;      // 입자의 LifeTime 
    float3  vOffsets;       // 16 byte맞추기 위해 넣음. 쓸일있으면 알아서 맞춰서 쓰면됨. 
};

struct ComputeShared_UAV
{
    int     addCount;
    float3  padding;
};

RWStructuredBuffer<ParticleInfo_UAV> g_ParticleInfo_UAVBuffer;
RWStructuredBuffer<ComputeShared_UAV> g_ComputeShared_UAVBuffer;

/* [Compute Shader]------------------------------ */

[numthreads(1024, 1, 1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    if(threadIndex.x>= maxCount)
        return;
        
    float deltaTime = DeltaAccTime.x;
    float accTime = DeltaAccTime.y;
    float minLifeTime = MinMaxLifeTime.x;
    float maxLifeTime = MinMaxLifeTime.y;
    float minSpeed = MinMaxSpeed.x;
    float maxSpeed = MinMaxSpeed.y;
    
    float3 additionalVel = g_vec4_0.xyz;
    
    g_Shared[0].addCount = addCount;
    GroupMemoryBarrierWithGroupSync();
    
    if(g_Particle[threadIndex.x].alive ==0)
    {
        while (true)
        {
            int remaining = g_Shared[0].addCount;
            if(remaining <= 0)
                break;
            
            int expected = remaining;
            int desired = remaining - 1;
            int originalValue;
            InterlockedCompareExchange(g_Shared[0].addCount, expected, desired, originalValue);

            if (originalValue == expected)
            {
                g_Particle[threadIndex.x].alive = 1;
                break;
            }
        }
        if (g_Particle[threadIndex.x].alive == 1)
        {
            float x = ((float) threadIndex.x / (float) maxCount) + accTime;

            float r1 = Rand(float2(x, accTime));
            float r2 = Rand(float2(x * accTime, accTime));
            float r3 = Rand(float2(x * accTime * accTime, accTime * accTime));

            // [0.5~1] -> [0~1]
            float3 noise =
            {
                2 * r1 - 1,
                2 * r2 - 1,
                2 * r3 - 1
            };

            // [0~1] -> [-1~1]
            float3 dir = (noise - 0.5f) * 2.f;

            float3 centerPos = (float3) 0.f;
            
            if(g_int_3 == 1)
                centerPos = float3(W._41_42_43);
            
            g_Particle[threadIndex.x].worldDir = normalize(dir) * g_float_1;
            g_Particle[threadIndex.x].worldPos = centerPos + (noise.xyz - 0.5f) * g_float_0;
            g_Particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
            g_Particle[threadIndex.x].curTime = 0.f;
            g_Particle[threadIndex.x].randTheta = float4(dir.x,dir.y,dir.z,dir.x * dir.y);
            if (g_int_2 == 1)
            {
                g_Particle[threadIndex.x].size.x = noise.x * max(StartEndScale.x, StartEndScale.y);
                if (g_Particle[threadIndex.x].size.x < min(StartEndScale.x, StartEndScale.y))
                {
                    g_Particle[threadIndex.x].size.x = min(StartEndScale.x, StartEndScale.y);

                }
                g_Particle[threadIndex.x].size.y = noise.y * max(StartEndScale.x, StartEndScale.y);
                if (g_Particle[threadIndex.x].size.y < min(StartEndScale.x, StartEndScale.y))
                {
                    g_Particle[threadIndex.x].size.y = min(StartEndScale.x, StartEndScale.y);

                }

            }
            else
                g_Particle[threadIndex.x].size.x = -1.f;
            
            if(isModel)
            {
                float3 randVec =
                {
                    2 * Rand(float2(x * accTime * threadIndex.x, accTime * accTime)) - 1,
                2 * Rand(float2(x * threadIndex.x, accTime * accTime * accTime)) - 1,
                2 * Rand(float2(x * threadIndex.x * accTime * accTime, accTime * accTime * accTime)) - 1
                };
            
                float c = -(dir.x * randVec.x + dir.y * randVec.y) / dir.z;
                float3 rotateAxis = { randVec.x, randVec.y, c };
                
                //right
                g_Particle[threadIndex.x].randTheta = float4(rotateAxis, 0.f);
                g_Particle[threadIndex.x].size.x = noise.x * max(StartEndScale.x, StartEndScale.y);
                if (g_Particle[threadIndex.x].size.x < min(StartEndScale.x, StartEndScale.y))
                {
                    g_Particle[threadIndex.x].size.x = min(StartEndScale.x, StartEndScale.y);

                }
            }
            
            g_Particle[threadIndex.x].nowMovingVel = 0;

        }
    }
    else
    {
        g_Particle[threadIndex.x].curTime += deltaTime;
        if (g_Particle[threadIndex.x].lifeTime < g_Particle[threadIndex.x].curTime)
        {
            g_Particle[threadIndex.x].alive = 0;
            return;
        }
        
        float ratio = g_Particle[threadIndex.x].curTime / g_Particle[threadIndex.x].lifeTime;
        float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
        
        if ((g_int_0 != 0 || g_int_1 != 0) && !isModel)
        {
            g_Particle[threadIndex.x].worldDir =
            float3(cos(g_Particle[threadIndex.x].randTheta.x * g_Particle[threadIndex.x].curTime * g_float_2) * g_int_0 * sign(g_Particle[threadIndex.x].randTheta.y),
            0,
            sin(g_Particle[threadIndex.x].randTheta.x * g_Particle[threadIndex.x].curTime * g_float_2) * g_int_1 * sign(g_Particle[threadIndex.x].randTheta.z)) * g_float_1;
            
        }
       

            g_Particle[threadIndex.x].worldDir += additionalVel * deltaTime;
            g_Particle[threadIndex.x].worldPos += g_Particle[threadIndex.x].worldDir * speed * deltaTime;

        
        g_Particle[threadIndex.x].nowMovingVel = float4(g_Particle[threadIndex.x].worldDir * speed + additionalVel, 0.f);

    }
}

/* [Vertex Shader]------------------------------ */

struct VS_OUTPUT
{
    float4 viewPos :POSITION;
    float4 worldPos : POSITION2;
    float2 uv : TEXCOORD;
    float id : ID;
};
struct VTXParticle
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint instanceID : SV_INSTANCEID;
};

StructuredBuffer<ParticleInfo_UAV> g_Data;

VS_OUTPUT VS_Main(VTXParticle input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
    
    if(g_int_3 == 1)
    {
        output.viewPos = mul(float4(g_Data[input.instanceID].worldPos, 1.f), V);
        output.worldPos = float4(g_Data[input.instanceID].worldPos, 1.f);

    }
    else
    {
        float3 worldPos = mul(float4(input.position, 1.f), W).xyz;
        worldPos += g_Data[input.instanceID].worldPos;

        output.worldPos = float4(worldPos, 1.f);
        output.viewPos = mul(float4(worldPos, 1.f), V);
    }
    

    output.uv = input.uv;
    output.id = input.instanceID;
    
    return output;
}

/* [Geomatry Shader]------------------------------ */
struct GS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
    float3 viewPos : POSITION1;
    uint id : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_Main(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4] =
    {
        (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f
    };
    
    float StartScale = StartEndScale.x;
    float EndScale = StartEndScale.y;
    
    VS_OUTPUT vtx = input[0];
    uint id = (uint) vtx.id;
    if (0 >= g_Data[id].alive)
        return;
    
    float ratio = g_Data[id].curTime / g_Data[id].lifeTime;
    float2 scale = float2(((EndScale - StartScale) * ratio + StartScale) / 2.f, ((EndScale - StartScale) * ratio + StartScale) / 2.f);
    
    if(g_Data[id].size.x >=0.f)
        scale = g_Data[id].size * (1.f - ratio);
    
    float3 velocity = (g_Data[id].nowMovingVel.xyz) * scale.y;
    float3 viewVel = mul(float4(velocity, 0.f), V).xyz * g_float_1;
    
    // view space
    output[0].position = vtx.viewPos + float4(-g_float_2, 0.f, 0.f, 0.f);
    output[1].position = vtx.viewPos + float4(g_float_2 ,0.f, 0.f, 0.f);
    output[2].position = vtx.viewPos + float4(g_float_2, 0.f, 0.f, 0.f) - float4(viewVel, 0.f);
    output[3].position = vtx.viewPos + float4(-g_float_2, 0.f, 0.f, 0.f) - float4(viewVel, 0.f);

    output[0].viewPos = output[0].position.xyz;
    output[1].viewPos = output[1].position.xyz;
    output[2].viewPos = output[2].position.xyz;
    output[3].viewPos = output[3].position.xyz;
    // proj space
    for (int i = 0; i < 4;++i)
        output[i].position = mul(output[i].position, P);
    

    output[0].uv = float2(0.f,0.f);
    output[1].uv = float2(1.f,0.f);
    output[2].uv = float2(1.f,1.f);
    output[3].uv = float2(0.f,1.f); 
    
     for (int j = 0; j < 4; ++j)
    {
        output[j].viewNormal = float3(0.f, 0.f, -1.f);
        output[j].viewTangent = float3(1.f, 0.f, 0.f);
        output[j].id = id;
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

/* [Pixel Shader]------------------------------ */
float4 PS_Main(GS_OUTPUT input) : SV_Target
{
    // 
    float ratio = g_Data[input.id].curTime / g_Data[input.id].lifeTime;
    
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    float distortion = 0.f;
    if(bHasDistortionMap == 1)
        distortion = DistortionMap.Sample(LinearSampler, input.uv).r * g_float_0;

    
    //diffuseMapping
    float4 diffuseColor = Material.diffuse;
    
    if(bHasDiffuseMap)
        diffuseColor *= DiffuseMap.Sample(LinearSampler, input.uv + distortion);
    
    float4 emissiveColor = Material.emissive;
    if(bHasEmissiveMap)
        emissiveColor.rgb *= EmissiveMap.Sample(LinearSamplerMirror, input.uv + distortion).rgb;
        
    color.xyz = (Material.diffuse.xyz * diffuseColor.xyz)
     + Material.ambient.xyz
     + emissiveColor.xyz;
    
    if(bHasOpacityMap)
        color.a = OpacityMap.Sample(LinearSampler, input.uv + distortion).r;
    
    
    return color;
}

/* [Techniques]------------------------------ */

technique11 T_Compute // 0
{
    pass p0
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(CompileShader(cs_5_0, CS_Main()));
    }
};

technique11 T_MeshRender // 1
{
    pass pass_Geomatry
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Main()));
        SetPixelShader(CompileShader(ps_5_0, PS_Main()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
}; 

technique11 T_ModelRender // 2
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_NonAnim()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Model()));
        SetRasterizerState(RS_Default);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
};