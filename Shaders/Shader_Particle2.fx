#include "Render.fx"
#include "Light.fx"

bool    g_isModel;
float   g_fTimeDelta;


struct ParticleInfo_UAV  // ����: 16byte�� ���߱�
{
   /* �� �����Ӹ��� ���� �� �ִ� ���� */
    float   fCurrTime;          // Current �����ǰ��������� �󸶳� �ð��� �귶����
    float3  vCurrWorldPos;      // Current World pos 
    float   fCurrSpeed;         // Current speed
    float3  vCurrRotationSpeed; // Current ȸ���ӵ� 
    float   fCurrSpeedExponent; // Current speed Exponent
    float3  vCurrWorldDir;      // Current ���� ���� 
    float4  vCurrColor;         // Current color 
    float2  vCurrSize;          // Current size 
    int     iAlive;             // Is Alive

    /* ���ڰ� ó�� ������ �� �����Ǵ� ���� */
    float   fLifeTime; // ������ LifeTime 
    float3  vRotationAngle; // ������ ȸ������  

    /* �����Ӱ� ����� ������ ���� */
    float   vOffsets; // 16 byte���߱� ���� ����. ���������� �˾Ƽ� ���缭 �����. 
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
    /* Movement : ����. */
    
    if (threadIndex.x >= maxCount)
        return;
    
    g_ComputeShared_UAVBuffer[0].addCount = g_NewlyAddCnt;
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
        
        // For. ���� ������ �Ǿ��ٸ� �⺻ �� ä���ֱ� 
        if (g_ParticleInfo_UAVBuffer[threadIndex.x].iAlive == 1)
        {
            float x = ((float) threadIndex.x / (float) maxCount) + accTime;
            float r1 = Rand(float2(x, accTime));
            float r2 = Rand(float2(x * accTime, accTime));
            float r3 = Rand(float2(x * accTime * accTime, accTime * accTime));
            float3 noise = { 2 * r1 - 1, 2 * r2 - 1, 2 * r3 - 1 }; // [0.5~1] -> [0~1]
            
            float fParticleObjectsLifeTimeRatio = g_float_1 / g_float_0;
            
            g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrTime = 0.f;
            
            // Diffuse Color
            g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrColor = g_startColor.rgb + g_endColor.rgb * fParticleObjectsLifeTimeRatio;
            
            // Dissolve Speed
                // TODO ���߿� �ٲ����. 
            g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrSpeed = 0.f; 

            // Create Position
            g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldPos = g_vec4_0.xyz + (noise.xyz - 0.5f) * g_createRange.xyz;
            
            // LifeTime (if you need option, use g_lifeTimeOption)
            g_ParticleInfo_UAVBuffer[threadIndex.x].fLifeTime = ((g_MinMaxLifeTime.y - g_MinMaxLifeTime.x) * noise.x) + g_MinMaxLifeTime.x;
            
             // Scale 
            if (0 == g_int_1)       // constant
                g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrSize.xyz = ((g_vec2_1.y - g_vec2_1.x) * noise.x) + g_vec2_1.x;
            else if (1 == g_int_1)  // curve
                g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrSize.xyz = pow(g_vec2_1.x, g_vec2_1.y);
            
            // Speed
            if (0 == g_int_2)       // constant
                g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrSpeed = ((g_vec2_2.y - g_vec2_2.x) * noise.x) + g_vec2_2.x;
            else if (1 == g_int_2)  // curve
                g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrSpeed = pow(g_vec2_2.x, g_vec2_2.y);
            
            // Rotation Angle 
            if (0 == g_vec4_2)       // constant
                g_ParticleInfo_UAVBuffer[threadIndex.x].vRotationAngle.xyz = g_vec4_2.xyz;
            if (1 == g_vec4_2)       // random
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
void GS_Default(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    // TODO: ������  full billbord �ڵ� �־����. ���߿� �����̼� �Դ°ɷ� �����ϱ� 
    
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
        // TODO : sprite animation�����ϵ��� �����ؾ���.
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
        // TODO : sprite animation�����ϵ��� �����ؾ���.
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
        // TODO : sprite animation�����ϵ��� �����ؾ���.
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
        // TODO : sprite animation�����ϵ��� �����ؾ���.
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