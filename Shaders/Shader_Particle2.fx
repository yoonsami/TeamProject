#include "Render.fx"
#include "Light.fx"

float2 StartEndScale;
bool isModel;

struct ParticleInfo_UAV  // 주의: 16byte씩 맞추기
{
   /* 매 프레임마다 변할 수 있는 값들 */
    float   fCurrCurrTime;        // Current 생성되고나서부터 얼마나 시간이 흘렀는지
    float3  vCurrWorldPos;      // Current World pos 
    float   fCurrSpeed;         // Current speed
    float3  vCurrRotationSpeed; // Current 회전속도 
    float   fCurrSpeedExponent; // Current speed Exponent
    float3  vCurrWorldDir;      // Current 현재 방향 
    float4  vCurrColor;         // Current color 
    float2  vCurrSize;          // Current size 
    int     iAlive;             // Is Alive

    /* 입자가 처음 생성될 때 고정되는 값들 */
    float   fLifeTime; // 입자의 LifeTime 
    float3  vRotationAngle; // 입자의 회전각도  

    /* 자유롭게 사용한 데이터 공간 */
    float vOffsets; // 16 byte맞추기 위해 넣음. 쓸일있으면 알아서 맞춰서 쓰면됨. 
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

[numthreads(1024, 1, 1)]
void CS_Movement_Non(int3 threadIndex : SV_DispatchThreadID)
{
    /* Movement : 없음. */
    
    if (threadIndex.x >= maxCount)
        return;
    
    
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
void GS_Default(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    // TODO: 지금은  full billbord 코드 넣어놨음. 나중에 로테이션 먹는걸로 수정하기 
    
    GS_OUTPUT output[4];

    VS_OUTPUT vtx = input[0];
    uint id = (uint) vtx.id;
    if (0 >= g_Data[id].iAlive)
        return;
    
    float4 vLook = input[0].viewPos - input[0].worldPos;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * g_Data[id].vCurrSize.x;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * g_Data[id].vCurrSize.y;
    
    // For. world space
    output[0].position = float4(input[0].worldPos.xyz + vRight + vUp, 1.f);
    output[1].position = float4(input[0].worldPos.xyz - vRight + vUp, 1.f);
    output[2].position = float4(input[0].worldPos.xyz - vRight - vUp, 1.f);
    output[3].position = float4(input[0].worldPos.xyz + vRight - vUp, 1.f);
    
    // For. view space
    output[0].viewPos = output[0].position.xyz;
    output[1].viewPos = output[1].position.xyz;
    output[2].viewPos = output[2].position.xyz;
    output[3].viewPos = output[3].position.xyz;
    
    // For. proj space
    for (int i = 0; i < 4; ++i)
        output[i].position = mul(output[i].position, P);
    
    // For. Texcoord 
        // TODO : sprite animation가능하도록 수정해야함.
    output[0].uv = float2(0.f, 0.f);
    output[1].uv = float2(1.f, 0.f);
    output[2].uv = float2(1.f, 1.f);
    output[3].uv = float2(0.f, 1.f);
    
    // For. Append 
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
void GS_Billbord(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4];

    VS_OUTPUT vtx = input[0];
    uint id = (uint) vtx.id;
    if (0 >= g_Data[id].iAlive)
        return;
    
    float4 vLook = input[0].viewPos - input[0].worldPos;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * g_Data[id].vCurrSize.x;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * g_Data[id].vCurrSize.y;
    
    // For. world space
    output[0].position = float4(input[0].worldPos.xyz + vRight + vUp, 1.f);
    output[1].position = float4(input[0].worldPos.xyz - vRight + vUp, 1.f);
    output[2].position = float4(input[0].worldPos.xyz - vRight - vUp, 1.f);
    output[3].position = float4(input[0].worldPos.xyz + vRight - vUp, 1.f);
    
    // For. view space
    output[0].viewPos = output[0].position.xyz;
    output[1].viewPos = output[1].position.xyz;
    output[2].viewPos = output[2].position.xyz;
    output[3].viewPos = output[3].position.xyz;
    
    // For. proj space
    for (int i = 0; i < 4; ++i)
        output[i].position = mul(output[i].position, P);
    
    // For. Texcoord 
        // TODO : sprite animation가능하도록 수정해야함.
    output[0].uv = float2(0.f, 0.f);
    output[1].uv = float2(1.f, 0.f);
    output[2].uv = float2(1.f, 1.f);
    output[3].uv = float2(0.f, 1.f);
    
    // For. Append 
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
void GS_Billbord_LockY(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4];

    VS_OUTPUT vtx = input[0];
    uint id = (uint) vtx.id;
    if (0 >= g_Data[id].iAlive)
        return;
    
    float4 vLook = input[0].viewPos - input[0].worldPos;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * g_Data[id].vCurrSize.x;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * g_Data[id].vCurrSize.y;
    
    // For. world space
    output[0].position = float4(input[0].worldPos.xyz + vRight + vUp, 1.f);
    output[1].position = float4(input[0].worldPos.xyz - vRight + vUp, 1.f);
    output[2].position = float4(input[0].worldPos.xyz - vRight - vUp, 1.f);
    output[3].position = float4(input[0].worldPos.xyz + vRight - vUp, 1.f);
    
    // For. view space
    output[0].viewPos = output[0].position.xyz;
    output[1].viewPos = output[1].position.xyz;
    output[2].viewPos = output[2].position.xyz;
    output[3].viewPos = output[3].position.xyz;
    
    // For. proj space
    for (int i = 0; i < 4; ++i)
        output[i].position = mul(output[i].position, P);
    
    // For. Texcoord 
        // TODO : sprite animation가능하도록 수정해야함.
    output[0].uv = float2(0.f, 0.f);
    output[1].uv = float2(1.f, 0.f);
    output[2].uv = float2(1.f, 1.f);
    output[3].uv = float2(0.f, 1.f);
    
    // For. Append 
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
void GS_Billbord_LockX(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4];

    VS_OUTPUT vtx = input[0];
    uint id = (uint) vtx.id;
    if (0 >= g_Data[id].iAlive)
        return;
    
    float4 vLook = input[0].viewPos - input[0].worldPos;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * g_Data[id].vCurrSize.x;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * g_Data[id].vCurrSize.y;
    
    // For. world space
    output[0].position = float4(input[0].worldPos.xyz + vRight + vUp, 1.f);
    output[1].position = float4(input[0].worldPos.xyz - vRight + vUp, 1.f);
    output[2].position = float4(input[0].worldPos.xyz - vRight - vUp, 1.f);
    output[3].position = float4(input[0].worldPos.xyz + vRight - vUp, 1.f);
    
    // For. view space
    output[0].viewPos = output[0].position.xyz;
    output[1].viewPos = output[1].position.xyz;
    output[2].viewPos = output[2].position.xyz;
    output[3].viewPos = output[3].position.xyz;
    
    // For. proj space
    for (int i = 0; i < 4; ++i)
        output[i].position = mul(output[i].position, P);
    
    // For. Texcoord 
        // TODO : sprite animation가능하도록 수정해야함.
    output[0].uv = float2(0.f, 0.f);
    output[1].uv = float2(1.f, 0.f);
    output[2].uv = float2(1.f, 1.f);
    output[3].uv = float2(0.f, 1.f);
    
    // For. Append 
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

float4 PS_Default(GS_OUTPUT input)
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    return color;
}

/* [Techniques]------------------------------ */

technique11 T_Compute // 0
{
    pass pass_Default // 0
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(CompileShader(cs_5_0, CS_Movement_Non()));
    }
};

technique11 T_MeshRender // 1
{
    pass pass_Default  // 0 (No billbord)
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Default()));
        SetPixelShader(CompileShader(ps_5_0, PS_Default()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
    pass pass_Billbord // 1
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Billbord()));
        SetPixelShader(CompileShader(ps_5_0, PS_Default()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
    pass pass_Billbord_LockY // 2
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Billbord_LockY()));
        SetPixelShader(CompileShader(ps_5_0, PS_Default()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
    pass pass_Billbord_LockX // 3
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Billbord_LockX()));
        SetPixelShader(CompileShader(ps_5_0, PS_Default()));
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