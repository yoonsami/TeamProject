#include "Render.fx"
#include "Light.fx"

bool    g_isModel;
bool    g_bUseShapeTextureColor;
float   g_fTimeDelta;

struct ParticleInfo_UAV  // 주의: 16byte씩 맞추기
{
    float fCurrAge;
    float3 vCurrWorldPos;
    
    float  fLifeTime;
    float3 vRotationAxis;
    
    int iIsAlive;
    float3 vRotationSpeed;
    
    float2 StartEndScale;
    float2 StartEndSpeed;
    
    float4 vDiffuseColor;
    
    float3 vCurrWorldDir;
    float  fCurrRotateAngle;
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
    
    int iMaxInstanceCnt = g_int_0;
    
    if (threadIndex.x >= iMaxInstanceCnt)
        return;
    
    g_ComputeShared_UAVBuffer[0].addCount = g_iNewlyAddCnt;
    g_ComputeShared_UAVBuffer[0].padding = float3(0.f, 0.f, 0.f);
    GroupMemoryBarrierWithGroupSync();
    
    // For. Create New Particle
    if (g_ParticleInfo_UAVBuffer[threadIndex.x].iIsAlive == 0)
    {
        int iIndex = 0;
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
                g_ParticleInfo_UAVBuffer[threadIndex.x].iIsAlive = 1;
                break;
            }
        }
        
        // For. New Particle  
        float x = ((float) threadIndex.x / (float) iMaxInstanceCnt) + g_fTimeDelta;
        float r1 = Rand(float2(x, g_fTimeDelta)); // r1,r2,r3 = 0.5 ~ 1
        float r2 = Rand(float2(x * g_fTimeDelta, g_fTimeDelta));
        float r3 = Rand(float2(x * g_fTimeDelta * g_fTimeDelta, g_fTimeDelta * g_fTimeDelta));
        float3 noise = { 2 * r1 - 1, 2 * r2 - 1, 2 * r3 - 1 }; //  0 ~ 1
        float arrayNoise[3] = { 2 * r1 - 1, 2 * r2 - 1, 2 * r3 - 1 };
        if (g_ParticleInfo_UAVBuffer[threadIndex.x].iIsAlive == 1)
        {
            float fLifeTimeRatio = g_float_1 / g_float_0;
            
            /* Setting Particle's UAV */
            
            g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrAge = 0.f;

            g_ParticleInfo_UAVBuffer[threadIndex.x].vDiffuseColor = lerp(g_vStartColor, g_vEndColor, fLifeTimeRatio);
            
            g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldPos = g_vec4_0.xyz + (noise.xyz - 0.5f) * g_vCreateRange.xyz;

            g_ParticleInfo_UAVBuffer[threadIndex.x].fLifeTime = ((g_vMinMaxLifeTime.y - g_vMinMaxLifeTime.x) * noise.x) + g_vMinMaxLifeTime.x;

            g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndScale.x = ((g_vMinMaxStartScale.y - g_vMinMaxStartScale.x) * noise.x) + g_vMinMaxStartScale.x;
            float fRandomEndScale = ((g_vMinMaxEndScale.y - g_vMinMaxEndScale.x) * noise.x) + g_vMinMaxEndScale.x;
            if (0 == g_iEndScaleOption)         // static
                g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndScale.y = fRandomEndScale;
            else if (1 == g_iEndScaleOption)    // Offset to Add start scale
                g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndScale.y = g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndScale.x + fRandomEndScale;
            
            g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndSpeed.x = ((g_vMinMaxStartSpeed.y - g_vMinMaxStartSpeed.x) * noise.x) + g_vMinMaxStartSpeed.x;
            float fRandomEndSpeed = ((g_vMinMaxEndSpeed.y - g_vMinMaxEndSpeed.x) * noise.x) + g_vMinMaxEndSpeed.x;
            if (0 == g_iEndSpeedOption)         // static
                g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndSpeed.y = fRandomEndSpeed;
            else if (1 == g_iEndSpeedOption)    // Offset to Add start scale
                g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndSpeed.y = g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndSpeed.x + fRandomEndSpeed;
            
            // Init rotation angle and speed 
            g_ParticleInfo_UAVBuffer[threadIndex.x].vRotationSpeed.x = ((g_vMinMaxRotationSpeed_X.y - g_vMinMaxRotationSpeed_X.x) * arrayNoise[iIndex++]) + g_vMinMaxRotationSpeed_X.x; // y,z no use 
            g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrRotateAngle = g_ParticleInfo_UAVBuffer[threadIndex.x].vRotationSpeed.x;
            if (iIndex >= 3)
                iIndex = 0;
            
            // Init Look 
            g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldDir.x = ((g_vMinMaxRotationAngle_X.y - g_vMinMaxRotationAngle_X.x) * arrayNoise[iIndex++]) + g_vMinMaxRotationAngle_X.x;
            if (iIndex >= 3)
                iIndex = 0;
            g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldDir.y = ((g_vMinMaxRotationAngle_Y.y - g_vMinMaxRotationAngle_Y.x) * arrayNoise[iIndex++]) + g_vMinMaxRotationAngle_Y.x;
            if (iIndex >= 3)
                iIndex = 0;
            g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldDir.z = ((g_vMinMaxRotationAngle_Z.y - g_vMinMaxRotationAngle_Z.x) * arrayNoise[iIndex++]) + g_vMinMaxRotationAngle_Z.x;
            if (iIndex >= 3)
                iIndex = 0;
            
            // Random vector (axis about rotation)
            float3 randVec =
            {
                2 * Rand(float2(x * g_float_1 * threadIndex.x, g_float_1 * g_float_1)) - 1,
                2 * Rand(float2(x * threadIndex.x, g_float_1 * g_float_1 * g_float_1)) - 1,
                2 * Rand(float2(x * threadIndex.x * g_float_1 * g_float_1, g_float_1 * g_float_1 * g_float_1)) - 1
            };
            float c = -(g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldDir.x * randVec.x + g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldDir.y * randVec.y) / g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldDir.z;
            g_ParticleInfo_UAVBuffer[threadIndex.x].vRotationAxis = float3(randVec.x, randVec.y, c);
        }
    }
    // For. Update particle UAV data which previouslly alived
    else
    {
        g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrAge += g_fTimeDelta;
        float fLifeTimeRatio = g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrAge / g_ParticleInfo_UAVBuffer[threadIndex.x].fLifeTime;
        
        // For. Check end lifetime 
        if (fLifeTimeRatio >= 1.f)
        {
            g_ParticleInfo_UAVBuffer[threadIndex.x].iIsAlive = 0;
            return;
        }      

        float fAngle = g_ParticleInfo_UAVBuffer[threadIndex.x].fCurrRotateAngle * (3.141592f / 180.f);
        
        float3 vLook = normalize(g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldDir);
        float3 vAxis = normalize(g_ParticleInfo_UAVBuffer[threadIndex.x].vRotationAxis);
        
        float cosAngle = cos(fAngle);
        float sinAngle = sin(fAngle);
        
        float3x3 rotationMatrix = float3x3(
            cosAngle + (1 - cosAngle) * vAxis.x * vAxis.x,
            (1 - cosAngle) * vAxis.x * vAxis.y - vAxis.z * sinAngle,
            (1 - cosAngle) * vAxis.x * vAxis.z + vAxis.y * sinAngle,
            (1 - cosAngle) * vAxis.y * vAxis.x + vAxis.z * sinAngle,
            cosAngle + (1 - cosAngle) * vAxis.y * vAxis.y,
            (1 - cosAngle) * vAxis.y * vAxis.z - vAxis.x * sinAngle,
            (1 - cosAngle) * vAxis.z * vAxis.x - vAxis.y * sinAngle,
            (1 - cosAngle) * vAxis.z * vAxis.y + vAxis.x * sinAngle,
            cosAngle + (1 - cosAngle) * vAxis.z * vAxis.z
        );
        
        g_ParticleInfo_UAVBuffer[threadIndex.x].vCurrWorldDir = mul(rotationMatrix, vLook);
        
        // For. Movement 
        float fCurrSpeed = lerp(g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndSpeed.x, g_ParticleInfo_UAVBuffer[threadIndex.x].StartEndSpeed.y, fLifeTimeRatio);
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
void GS_Default(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4];

    VS_OUTPUT vtx = input[0];
    uint id = (uint) vtx.id;
    if (0 >= g_Data[id].iIsAlive)
        return;
    

    float2 vCurrScale = lerp(g_Data[id].StartEndScale.x, g_Data[id].StartEndScale.y, g_Data[id].fCurrAge / g_Data[id].fLifeTime);
   
    float3 vLook = normalize(g_Data[id].vCurrWorldDir);
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * vCurrScale.x * 0.5f;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * vCurrScale.y * 0.5f; 
    
    // For. world space
    output[0].position = float4(input[0].worldPos.xyz + vRight + vUp, 1.f);
    output[1].position = float4(input[0].worldPos.xyz - vRight + vUp, 1.f);
    output[2].position = float4(input[0].worldPos.xyz - vRight - vUp, 1.f);
    output[3].position = float4(input[0].worldPos.xyz + vRight - vUp, 1.f);
    
    // For. proj space
    for (int i = 0; i < 4; ++i)
    {
        output[i].viewPos = mul(output[i].position, V).xyz;
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

[maxvertexcount(6)]
void GS_Billbord(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4];

    VS_OUTPUT vtx = input[0];
    uint id = (uint) vtx.id;
    if (0 >= g_Data[id].iIsAlive)
        return;
    
    float2 vCurrScale = lerp(g_Data[id].StartEndScale.x, g_Data[id].StartEndScale.y, g_Data[id].fCurrAge / g_Data[id].fLifeTime);
    
    float4 vLook = float4(CameraPosition() - input[0].worldPos.xyz, 0.f);
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * vCurrScale.x * 0.5f;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * vCurrScale.y * 0.5f;
    
    // For. world space
    output[0].position = float4(input[0].worldPos.xyz + vRight + vUp, 1.f);
    output[1].position = float4(input[0].worldPos.xyz - vRight + vUp, 1.f);                        
    output[2].position = float4(input[0].worldPos.xyz - vRight - vUp, 1.f);
    output[3].position = float4(input[0].worldPos.xyz + vRight - vUp, 1.f);
    
    // For. proj space
    for (int i = 0; i < 4; ++i)
    {
        output[i].viewPos = mul(output[i].position, V).xyz;
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
    if (0 >= g_Data[input.id].iIsAlive)
        discard;
  
    float fLifeRatio = g_Data[input.id].fCurrAge / g_Data[input.id].fLifeTime;
    float4 fDiffuseMapColor = float4(0.f, 0.f, 0.f, 0.f);
    if (0 != bHasDiffuseMap)
        fDiffuseMapColor = DiffuseMap.Sample(LinearSampler, input.uv);
    
    // For. Changing Diffuse Color
    if (g_bUseShapeTextureColor)
        output.rgb = fDiffuseMapColor.rgb;
    else
        output = lerp(g_Data[input.id].vDiffuseColor, g_vec4_0, fLifeRatio);
    output.a = fDiffuseMapColor.r;
    
    // For. Brightness
    if (output.a > 1.f - g_float_0)
        output.rgb = lerp(output.rgb, float3(1.f, 1.f, 1.f), output.a);

    // For. Alpha Gradation by Duration ( does not works in Instance particle cause of blend off )
    //if(1 == g_int_0)
    //    output.a *= 1.f - (g_float_3 / g_float_2);
    
    // For. Dissiolve
    if (0 != bHasDissolveMap)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < fLifeRatio)
            discard;
    }
    
    // For. Alpah Test 
    if (output.a <= 0.5f)
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

technique11 T_InstanceParticleRender // 1
{
    pass pass_NoBillbord // 0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Default()));
        SetPixelShader(CompileShader(ps_5_0, PS_Default()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
    pass pass_Billbord  // 1
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Billbord()));
        SetPixelShader(CompileShader(ps_5_0, PS_Default()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
}; 
