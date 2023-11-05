#include "Render.fx"
#include "Light.fx"

bool    g_isModel;
bool    g_bUseShapeTextureColor;
float   g_fTimeDelta;


struct ParticleInfo_UAV  // 주의: 16byte씩 맞추기
{
   /* 매 프레임마다 변할 수 있는 값들 */
    float   fCurrTime;          // Current 생성되고나서부터 얼마나 시간이 흘렀는지
    float3  vCurrWorldPos;      // Current World pos 
    float   fCurrSpeed;         // Current speed
    float3  vCurrRotationSpeed; // Current 회전속도 
    float   fCurrSpeedExponent; // Current speed Exponent
    float3  vCurrWorldDir;      // Current 현재 방향 
    float2  vCurrSize;          // Current size 
    int     iAlive;             // Is Alive

    /* 입자가 처음 생성될 때 고정되는 값들 */
    float   fLifeTime; // 입자의 LifeTime 
    float4  vDiffuseColor;      // Current color 
    float3  vRotationAngle; // 입자의 회전각도  

    /* 자유롭게 사용한 데이터 공간 */
    float   vOffsets; // 16 byte맞추기 위해 넣음. 쓸일있으면 알아서 맞춰서 쓰면됨. 
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
void CS_Movement_Non(int3 threadIndex : SV_DispatchThreadID)
{
    /* Movement : 없음. */
    
    int iMaxInstanceCnt = g_int_3;
    
    if (threadIndex.x >= iMaxInstanceCnt)
        return;
    
    g_ComputeShared_UAVBuffer[0].addCount = g_NewlyAddCnt;
    g_ComputeShared_UAVBuffer[0].padding = float3(0.f, 0.f, 0.f);
    GroupMemoryBarrierWithGroupSync();
    
    // For. Create New Particle
    if (g_ParticleInfo_UAVBuffer[threadIndex.x].iAlive == 0)
    {
        while (true)
        {
            int remaining = g_ComputeShared_UAVBuffer[0].addCount;
            if (remaining <= 0)
                break;
            
            int expected = remaining;
            int desired = remaining - 1;
            int originalValue;
            InterlockedCompareExchange(g_ComputeShared_UAVBuffer[0].addCount, expected, desired, originalValue);

            if (originalValue == expected)
            {
                g_ParticleInfo_UAVBuffer[threadIndex.x].iAlive = 1;
                break;
            }
        }
        
        // For. 새로 생성이 되었다면 기본 값 채워주기 
        if (g_ParticleInfo_UAVBuffer[threadIndex.x].iAlive == 1)
        {
            float x = ((float) threadIndex.x / (float) iMaxInstanceCnt) + g_fTimeDelta;
            float r1 = Rand(float2(x, g_fTimeDelta));
            float r2 = Rand(float2(x * g_fTimeDelta, g_fTimeDelta));
            float r3 = Rand(float2(x * g_fTimeDelta * g_fTimeDelta, g_fTimeDelta * g_fTimeDelta));
            float3 noise = { 2 * r1 - 1, 2 * r2 - 1, 2 * r3 - 1 }; // [0.5~1] -> [0~1]
            
            float fParticleObjectsLifeTimeRatio = g_float_1 / g_float_0;
            
            g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrTime = 0.f;
            
            // Diffuse Color
            g_ParticleInfo_UAVBuffer[threadIndex.x].vDiffuseColor = lerp(g_startColor.rgba, g_endColor.rgba, fParticleObjectsLifeTimeRatio);
            
            // Dissolve Speed
            if (0.f == g_int_2.x)       // constant
                g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrSpeed = ((g_vec2_1.y - g_vec2_1.x) * noise.x) + g_vec2_1.x;
            if (1.f == g_int_2.x)       // curve
                g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrSpeed = pow(abs(g_vec2_1.x), g_vec2_1.y);

            // Create Position
            g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldPos = g_vec4_0.xyz + (noise.xyz - 0.5f) * g_createRange.xyz;
            
            // LifeTime (if you need option, use g_lifeTimeOption)
            g_ParticleInfo_UAVBuffer[threadIndex.x].fLifeTime = ((g_MinMaxLifeTime.y - g_MinMaxLifeTime.x) * noise.x) + g_MinMaxLifeTime.x;
            
             // Scale 
            g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrSize.xy = ((g_MinMaxScale.y - g_MinMaxScale.x) * noise.x) + g_MinMaxScale.x;
            
            // Speed
            if (0 == g_int_2)       // constant
                g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrSpeed = ((g_vec2_2.y - g_vec2_2.x) * noise.x) + g_vec2_2.x;
            else if (1 == g_int_2)  // curve
                g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrSpeed = pow(abs(g_vec2_2.x), g_vec2_2.y);
            
            // Rotation Angle 
            if (0.f == g_vec4_2.x)       // constant
                g_ParticleInfo_UAVBuffer[threadIndex.x].vRotationAngle.xyz = g_vec4_2.xyz;
            if (1.f == g_vec4_2.x)       // random
            {
                g_ParticleInfo_UAVBuffer[threadIndex.x].vRotationAngle.x = ((g_vec4_2.z - g_vec4_2.y) * noise.x) + g_vec4_2.y;
                g_ParticleInfo_UAVBuffer[threadIndex.x].vRotationAngle.y = ((g_vec4_2.z - g_vec4_2.y) * noise.y) + g_vec4_2.y;
                g_ParticleInfo_UAVBuffer[threadIndex.x].vRotationAngle.z = ((g_vec4_2.z - g_vec4_2.y) * noise.z) + g_vec4_2.y;
            }
            
            // Rotation Speed
            g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrRotationSpeed.xyz = g_vec4_3.xyz;
        }
    }
    
    // For. Update particle UAV data which previouslly alived
    else
    {
        // For. Check end lifetime 
        g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrTime += g_fTimeDelta;
        if (g_ParticleInfo_UAVBuffer[threadIndex.x].fLifeTime < g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrTime)
        {
            g_ParticleInfo_UAVBuffer[threadIndex.x].iAlive = 0;
            return;
        }       
        
        // 
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
    
    output.viewPos = mul(float4(g_Data[input.instanceID].vCurrWorldPos, 1.f), V);
    output.worldPos = float4(g_Data[input.instanceID].vCurrWorldPos, 1.f);
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
void GS_Billbord(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4];

    VS_OUTPUT vtx = input[0];
    uint id = (uint) vtx.id;
    if (0 >= g_Data[id].iAlive)
        return;
    
    float4 vLook = float4(CameraPosition() - input[0].worldPos.xyz, 0.f);
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * g_Data[id].vCurrSize.x * 0.5f;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * g_Data[id].vCurrSize.y * 0.5f;
    
    // For. world space
    output[0].position = float4(input[0].worldPos.xyz + vRight + vUp, 1.f);
    output[1].position = float4(input[0].worldPos.xyz - vRight + vUp, 1.f);
    output[2].position = float4(input[0].worldPos.xyz - vRight - vUp, 1.f);
    output[3].position = float4(input[0].worldPos.xyz + vRight - vUp, 1.f);
    
    // For. proj space
    for (int i = 0; i < 4; ++i)
    {
        output[i].viewPos = mul(output[i].position, V);
        output[i].position = mul(output[i].position, VP);
    }
    
    // For. Texcoord // TODO : sprite animation가능하도록 수정해야함.
    output[0].uv = float2(0.f, 0.f);
    output[1].uv = float2(1.f, 0.f);
    output[2].uv = float2(1.f, 1.f);
    output[3].uv = float2(0.f, 1.f);
    
    // For. Normal, Tangent
    for (int j = 0; j < 4; ++j)
    {
        output[j].viewNormal = float3(0.f, 0.f, -1.f);
        output[j].viewTangent = float3(1.f, 0.f, 0.f);
        output[j].id = id;
    }
    
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
float4 PS_Default(GS_OUTPUT input) : SV_Target
{
    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    
    // For. Check is exist
    uint id = (uint) input.id;
    if (0 >= g_Data[input.id].iAlive)
        discard;
    
    // For. Diffuse Gradation 
    float4 fDiffuseMapColor = DiffuseMap.Sample(LinearSampler, input.uv);
    float4 diffuseColor = float4(0.f, 0.f, 0.f, 1.f);
    
    if (g_bUseShapeTextureColor)
    {
        diffuseColor = fDiffuseMapColor;
    }
    else
    {
        float ratio = g_Data[input.id].fCurrTime / g_Data[input.id].fLifeTime;
        diffuseColor = lerp(g_Data[input.id].vDiffuseColor, g_vec4_0, ratio);
                
        diffuseColor.a = fDiffuseMapColor.r;
    }
    
    // For. Alpha Gradation 
    if (diffuseColor.a > 1.f - g_float_0)
        diffuseColor = lerp(diffuseColor, float4(1.f, 1.f, 1.f, 1.f), diffuseColor.a);

    output = diffuseColor;
    
    if (output.a < 0.f)
        discard;
    
    return output;
}

/* [Techniques]------------------------------ */

technique11 T_Compute // 0
{
    pass pass_Default // 0
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(CompileShader(cs_5_0, CS_Movement_Non()));
    }
};

technique11 T_MeshRender // 1
{
    pass pass_Default  // 0 (No billbord)
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Billbord()));
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
        SetGeometryShader(CompileShader(gs_5_0, GS_Billbord()));
        SetPixelShader(CompileShader(ps_5_0, PS_Default()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
    pass pass_Billbord_LockX // 3
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Billbord()));
        SetPixelShader(CompileShader(ps_5_0, PS_Default()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
}; 