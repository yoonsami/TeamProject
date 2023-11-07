#include "Render.fx"
#include "Light.fx"

float2 StartEndScale;

bool isModel;

struct VTXParticle
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint instanceID : SV_INSTANCEID;
};

struct Particle
{
    float3 worldPos;
    float curTime;
    float3 worldDir;
    float lifeTime;
    int alive;
    float2 size;
    float padding;
    float4 randTheta;
    float4 nowMovingVel;

};

struct ComputeShared
{
    int addCount;
    float3 padding;
};

RWStructuredBuffer<Particle> g_Particle;
RWStructuredBuffer<ComputeShared> g_Shared;
//Main
// g_int_0 rightRandMove
// g_int_1 forwardRandMove
// g_int_2 InitRandomSize
// g_int_3 == 1 -> not mul W
// g_float_0 initRandomPos
// g_float_1 movingDirLength
// g_float_2 randMovingPower

//Sprite
// g_float_0 initRandomPos
// g_float_1 movingDirLength

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
void CS_Sphere(int3 threadIndex : SV_DispatchThreadID)
{
    if (threadIndex.x >= maxCount)
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
    
    if (g_Particle[threadIndex.x].alive == 0)
    {
        while (true)
        {
            int remaining = g_Shared[0].addCount;
            if (remaining <= 0)
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
            
            if (g_int_3 == 1)
                centerPos = float3(W._41_42_43);
            
            float3 pos;
            float3 matscale;
            float4 rotq;
            decompose(W, pos, rotq, matscale);
            
            
            float radius = g_float_0 * matscale.x * 0.5f;
            
            float3 randVec =
            {
                2 * Rand(float2(x * accTime * threadIndex.x, accTime * accTime)) - 1,
                2 * Rand(float2(x * threadIndex.x, accTime * accTime * accTime)) - 1,
                2 * Rand(float2(x * threadIndex.x * accTime * accTime, accTime * accTime * accTime)) - 1
            };
            
            float c = -(dir.x * randVec.x + dir.y * randVec.y) / dir.z;
            float3 rotateAxis = { randVec.x, randVec.y, c };
            
            g_Particle[threadIndex.x].worldPos = centerPos + normalize(dir) * radius;
            g_Particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
            g_Particle[threadIndex.x].curTime = 0.f;
            g_Particle[threadIndex.x].randTheta = float4(normalize(rotateAxis), dir.x * 3.141592);
            g_Particle[threadIndex.x].size.x = -1.f;
            g_Particle[threadIndex.x].worldDir = normalize(cross(dir, normalize(rotateAxis)));
            
            g_Particle[threadIndex.x].nowMovingVel = 0;
            if(isModel)
            {
                
                g_Particle[threadIndex.x].size.x = noise.x * max(StartEndScale.x, StartEndScale.y);
                if (g_Particle[threadIndex.x].size.x < min(StartEndScale.x, StartEndScale.y))
                {
                    g_Particle[threadIndex.x].size.x = min(StartEndScale.x, StartEndScale.y);

                }

            }
        }
    }
    else
    {
        float3 centerPos = (float3) 0.f;
            
        if (g_int_3 == 1)
            centerPos = float3(W._41_42_43);
        
        float3 pos;
        float3 matscale;
        float4 rotq;
        decompose(W, pos, rotq, matscale);
        
        g_Particle[threadIndex.x].curTime += deltaTime;
        if (g_Particle[threadIndex.x].lifeTime < g_Particle[threadIndex.x].curTime)
        {
            g_Particle[threadIndex.x].alive = 0;
            return;
        }
        
        float ratio = g_Particle[threadIndex.x].curTime / g_Particle[threadIndex.x].lifeTime;
        float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
        //°¢¼Óµµ
        float rotationSpeed = g_Particle[threadIndex.x].randTheta.w * g_float_1;
        float rotationAngle = rotationSpeed * g_Particle[threadIndex.x].curTime;
        
        float cosTheta = cos(rotationAngle);
        float sinTheta = sin(rotationAngle);
        float x = g_Particle[threadIndex.x].randTheta.x;
        float y = g_Particle[threadIndex.x].randTheta.y;
        float z = g_Particle[threadIndex.x].randTheta.z;
        float radius = g_float_0 * matscale.x * 0.5f;
        float3x3 rotationMat =
        {
            cosTheta + (1 - cosTheta) * x * x, (1 - cosTheta) * x * y - sinTheta * z, (1 - cosTheta) * x * z + sinTheta * y,
            (1 - cosTheta) * y * x + sinTheta * z, cosTheta + (1 - cosTheta) * y * y, (1 - cosTheta) * y * z - sinTheta * x,
            (1 - cosTheta) * z * x - sinTheta * y, (1 - cosTheta) * z * y + sinTheta * x, cosTheta + (1 - cosTheta) * z * z
        };
        float3 relativeDir = radius * normalize(g_Particle[threadIndex.x].worldPos - centerPos);
        float3 rotatedDir = mul(relativeDir, rotationMat);
        
        g_Particle[threadIndex.x].worldDir = normalize(cross(rotatedDir, g_Particle[threadIndex.x].randTheta.xyz));
        
        g_Particle[threadIndex.x].worldPos = centerPos + rotatedDir;
        g_Particle[threadIndex.x].nowMovingVel = float4(g_Particle[threadIndex.x].worldDir, 0.f) * length(relativeDir) * rotationSpeed;

    }
}

[numthreads(1024, 1, 1)]
void CS_Sprite(int3 threadIndex : SV_DispatchThreadID)
{
    if (threadIndex.x >= maxCount)
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
    
    if (g_Particle[threadIndex.x].alive == 0)
    {
        while (true)
        {
            int remaining = g_Shared[0].addCount;
            if (remaining <= 0)
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

            g_Particle[threadIndex.x].worldDir = normalize(dir) * g_float_1;
            g_Particle[threadIndex.x].worldPos = (noise.xyz - 0.5f) * g_float_0;
            g_Particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
            g_Particle[threadIndex.x].curTime = 0.f;
            g_Particle[threadIndex.x].randTheta = dir.x;
            g_Particle[threadIndex.x].size = float2(noise.x, noise.x);
            
        }
    }
    else
    {
        g_Particle[threadIndex.x].curTime += deltaTime;
        if (g_Particle[threadIndex.x].lifeTime < g_Particle[threadIndex.x].curTime)
        {
            g_Particle[threadIndex.x].alive = -1;
            return;
        }
        
        
        float ratio = g_Particle[threadIndex.x].curTime / g_Particle[threadIndex.x].lifeTime;
        float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
        
        
        
        g_Particle[threadIndex.x].worldPos += g_Particle[threadIndex.x].worldDir * speed * deltaTime + additionalVel * deltaTime;
        g_Particle[threadIndex.x].nowMovingVel = float4(g_Particle[threadIndex.x].worldDir * speed + additionalVel, 0.f);
     
    }
}

[numthreads(1024, 1, 1)]
void CS_Gather(int3 threadIndex : SV_DispatchThreadID)
{
    if (threadIndex.x >= maxCount)
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
    
    if (g_Particle[threadIndex.x].alive == 0)
    {
        while (true)
        {
            int remaining = g_Shared[0].addCount;
            if (remaining <= 0)
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
            
            if (g_int_3 == 1)
                centerPos = float3(W._41_42_43);
            
            g_Particle[threadIndex.x].worldDir = (dir) * g_float_1;
            g_Particle[threadIndex.x].worldPos = centerPos - g_Particle[threadIndex.x].worldDir * g_float_0;
            g_Particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
            g_Particle[threadIndex.x].curTime = 0.f;
            g_Particle[threadIndex.x].randTheta = centerPos.xyzz;
            
            g_Particle[threadIndex.x].size.x = -1.f;
            
            g_Particle[threadIndex.x].nowMovingVel = 0;
            
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
        float3 centerPos = (float3) 0.f;
            
        if (g_int_3 == 1)
            centerPos = float3(W._41_42_43);
        if (length(g_Particle[threadIndex.x].worldPos - centerPos) < 0.1f)
        {
            g_Particle[threadIndex.x].alive = 0;
            return;
        }
        
        float ratio = g_Particle[threadIndex.x].curTime / g_Particle[threadIndex.x].lifeTime;
        float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
               
        g_Particle[threadIndex.x].worldPos += g_Particle[threadIndex.x].worldDir * speed * deltaTime;
        g_Particle[threadIndex.x].nowMovingVel = float4(g_Particle[threadIndex.x].worldDir * speed, 0.f);

    }
}

[numthreads(1024, 1, 1)]
void CS_Spread(int3 threadIndex : SV_DispatchThreadID)
{
    if (threadIndex.x >= maxCount)
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
    
    if (g_Particle[threadIndex.x].alive == 0)
    {
        while (true)
        {
            int remaining = g_Shared[0].addCount;
            if (remaining <= 0)
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
            
            if (g_int_3 == 1)
                centerPos = float3(W._41_42_43);
            
            g_Particle[threadIndex.x].worldDir = (dir) * g_float_1;
            g_Particle[threadIndex.x].worldPos = centerPos;
            g_Particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
            g_Particle[threadIndex.x].curTime = 0.f;
            g_Particle[threadIndex.x].randTheta = centerPos.xyzz;
            
            g_Particle[threadIndex.x].size.x = -1.f;
            
            g_Particle[threadIndex.x].nowMovingVel = 0;
            
            if (isModel)
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

        }
    }
    else
    {
        float3 centerPos = (float3) 0.f;
            
        if (g_int_3 == 1)
            centerPos = float3(W._41_42_43);
        g_Particle[threadIndex.x].curTime += deltaTime;
        if (g_Particle[threadIndex.x].lifeTime < g_Particle[threadIndex.x].curTime)
        {
            g_Particle[threadIndex.x].alive = 0;
            return;
        }
        if (length(g_Particle[threadIndex.x].worldPos - centerPos) > g_float_0)
        {
            g_Particle[threadIndex.x].alive = 0;
            return;
        }
        
        float ratio = g_Particle[threadIndex.x].curTime / g_Particle[threadIndex.x].lifeTime;
        float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
               
        g_Particle[threadIndex.x].worldPos += g_Particle[threadIndex.x].worldDir * speed * deltaTime;
        g_Particle[threadIndex.x].nowMovingVel = float4(g_Particle[threadIndex.x].worldDir * speed, 0.f);

    }
}

[numthreads(1024, 1, 1)]
void CS_Clear(int3 threadIndex : SV_DispatchThreadID)
{
   
    if (g_Particle[threadIndex.x].alive <= 0)
    {
        g_Particle[threadIndex.x].alive = 0;
        g_Particle[threadIndex.x].worldPos = float3(0,0,0);

    }
}

struct VS_OUTPUT
{
    float4 viewPos :POSITION;
    float4 worldPos : POSITION2;
    float2 uv : TEXCOORD;
    float id : ID;
};

StructuredBuffer<Particle> g_Data;

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

struct VTXParticleModel
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 blendIndices : BLENDINDICES;
    float4 blendWeight : BLENDWEIGHT;
    uint instanceID : SV_INSTANCEID;
};

struct VTXParticleModelOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float id : ID;
};

VTXParticleModelOutput VS_NonAnim(VTXParticleModel input)
{
    VTXParticleModelOutput output;
    
    float3 particlePos = (g_Data[input.instanceID].worldPos);
    float3 particleLook = normalize(g_Data[input.instanceID].worldDir);
    float3 particleRight = normalize(g_Data[input.instanceID].randTheta).xyz;
    float3 particleUp = normalize(cross(particleLook, particleRight));
    particleRight = normalize(cross(particleUp, particleLook));
    if(g_int_3 == 0)
        particlePos += W._41_42_43;
    float4x4 particleMat =
    float4x4
    (float4(particleRight * g_Data[input.instanceID].size.x, 0.f),
             float4(cross(particleLook, particleRight) * g_Data[input.instanceID].size.x, 0.f),
             float4(particleLook * g_Data[input.instanceID].size.x, 0.f),
             float4(particlePos, 1.f));

    output.position = mul(float4(input.position, 1.f), BoneTransform[BoneIndex]);

    output.position = mul(output.position, particleMat);

    output.position = mul(output.position, VP);
    
    output.uv = input.uv;
    output.id = input.instanceID;
    
    
    return output;
}

VS_OUTPUT VS_Sprite(VTXParticle input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
    
    float3 worldPos = mul(float4(input.position, 1.f), W).xyz;
    worldPos += g_Data[input.instanceID].worldPos;

    output.worldPos = float4(worldPos, 1.f);
    output.viewPos = mul(float4(worldPos, 1.f), V);   
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
    //output[0].position = vtx.viewPos + float4(-scale.x, scale.y, 0.f, 0.f);
    //output[1].position = vtx.viewPos + float4(scale.x, scale.y, 0.f, 0.f);
    //output[2].position = vtx.viewPos + float4(scale.x, -scale.y, 0.f, 0.f);
    //output[3].position = vtx.viewPos + float4(-scale.x, -scale.y, 0.f, 0.f);

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

//g_int_0 = spriteX
//g_int_1 = spriteY
//g_int_2 = randomIndexMode
// g_float_0 = distortionPower
// SubMap0 = distortion UV

[maxvertexcount(6)]
void GS_Sprite(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outputStream)
{
    GS_OUTPUT output[4] =
    {
        (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f, (GS_OUTPUT) 0.f
    };
    
    float MaxScale = max(StartEndScale.x, StartEndScale.y);
    float MinScale = min(StartEndScale.x, StartEndScale.y);
    
    VS_OUTPUT vtx = input[0];
    uint id = (uint) vtx.id;
    if (0 >= g_Data[id].alive)
        return;
    
    float ratio = g_Data[id].curTime / g_Data[id].lifeTime;
    float2 scale = g_Data[id].size * (MaxScale - MinScale) + MinScale;
        
    // view space
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
    uint spriteCountX = g_int_0;
    uint spriteCountY = g_int_1;
    uint curFrame = 0;
    if(g_int_2 == 0)
        curFrame = int(ratio * spriteCountX * spriteCountY);
    else
        curFrame = id % spriteCountX * spriteCountY;
    
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

float4 PS_Main(GS_OUTPUT input) : SV_Target
{
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

float4 PS_Test(GS_OUTPUT input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    
    return color;
}

float4 PS_Model(VTXParticleModelOutput input) :SV_Target
{
    if(g_Data[input.id].alive == 0)
        discard;
    float4 diffuseColor;

    float4 emissiveColor;
    if (bHasDissolveMap != 0)
    {
        float dissolve = DissolveMap.Sample(LinearSampler, input.uv).r;
        if (dissolve < g_float_0)
            discard;
    }
    if (bHasDiffuseMap)
        diffuseColor = DiffuseMap.Sample(LinearSampler, input.uv) * Material.diffuse;
    else
        diffuseColor = Material.diffuse;

    if (bHasOpacityMap)
    {
        float4 maskColor = OpacityMap.Sample(LinearSampler, input.uv);
        if (maskColor.r < 0.01f)
            discard;
        diffuseColor.a = maskColor.r * diffuseColor.a;
    }
    
    if (bHasEmissiveMap)
        emissiveColor = EmissiveMap.Sample(LinearSampler, input.uv ) * Material.emissive;
    else
        emissiveColor = Material.emissive;

    diffuseColor.xyz = diffuseColor.xyz + emissiveColor.xyz;
    return diffuseColor;
}

technique11 T0
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Main()));
        SetPixelShader(CompileShader(ps_5_0, PS_Main()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }
    pass p1
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Sprite()));
        SetGeometryShader(CompileShader(gs_5_0, GS_Sprite()));
        SetPixelShader(CompileShader(ps_5_0, PS_Main()));
        SetRasterizerState(RS_CullNone);
        SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(NULL);
    }

};

technique11 T1
{
    pass p0
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(CompileShader(cs_5_0, CS_Main()));
    }
    pass p1
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(CompileShader(cs_5_0, CS_Sprite()));
    }
    pass p2
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(CompileShader(cs_5_0, CS_Clear()));
    }
    pass p3
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(CompileShader(cs_5_0, CS_Sphere()));
    }
    pass p4
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(CompileShader(cs_5_0, CS_Gather()));
    }
    pass p5
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetBlendState(BlendOff, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetComputeShader(CompileShader(cs_5_0, CS_Spread()));
    }
};

technique11 T2
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