#ifndef _RENDER_FX_
#define _RENDER_FX_

#include "Global.fx"

#define MAX_MODEL_TRANSFORM 320
#define MAX_MODEL_INSTANCE 100
#define MAX_SRV_MAP_COUNT 16



// Mesh_Renderer

struct VTXMeshInstancing
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint instanceID : SV_INSTANCEID;
    matrix world : INST;
};

// ModelRenderer, Animator

struct VTXModelInstancing
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 blendIndices : BLENDINDICES;
    float4 blendWeight : BLENDWEIGHT;
    int filtered : TEXCOORD2;
    uint instanceID : SV_INSTANCEID;
    matrix world : INST;
};

struct KeyFrameDesc
{
    int animIndex;
    uint currFrame;
    uint nextFrame;
    float ratio;
    float sumTime;
    float3 padding;
};

struct TweenFrameDesc
{
    float tweenDuration;
    float tweenRatio;
    float tweenSumTime;
    float padding;
    KeyFrameDesc curr;
    KeyFrameDesc next;
};
struct RenderParamDesc
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    row_major float4x4 g_mat_0;
    row_major float4x4 g_mat_1;
    row_major float4x4 g_mat_2;
    row_major float4x4 g_mat_3;
};

cbuffer RenderParamBuffer
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    row_major float4x4 g_mat_0;
    row_major float4x4 g_mat_1;
    row_major float4x4 g_mat_2;
    row_major float4x4 g_mat_3;
};
cbuffer KeyFrameBuffer
{
    KeyFrameDesc keyframes;
};

cbuffer TweenBuffer
{
    TweenFrameDesc TweenFrames;
};

cbuffer InstanceTweenBuffer
{
    TweenFrameDesc InstanceTweenFrames[MAX_MODEL_INSTANCE];
};

cbuffer InstanceRenderParamBuffer
{
    RenderParamDesc InstanceRenderParams[MAX_MODEL_INSTANCE];
};

cbuffer BoneBuffer
{
    matrix BoneTransform[MAX_MODEL_TRANSFORM];
};

cbuffer AnimAddonBuffer
{
    matrix OffsetMatrix[MAX_MODEL_TRANSFORM];
    matrix pivot;
};

cbuffer EffectBuffer
{
    float2 UVSliding;
    int maxGradationIndex;
    int gradationMapIndex;
    
    int maxSubGradationIndex;
    int SubGradationMapIndex;
    float2 uvSlidingAcc;
};

cbuffer ParticleBuffer
{
    int maxCount;
    int addCount;
    int frameNumber;
    int padding0;
    
    float2 DeltaAccTime;
    float2 MinMaxLifeTime;
    
    float2 MinMaxSpeed;
    float2 padding1;
};

cbuffer TextureMapBuffer
{
    int bHasDiffuseMap;
    int bHasNormalMap;
    int bHasSpecularMap;
    int bHasOpacityMap;
    int bHasEmissiveMap;
    int bHasDissolveMap;
    int bHasDistortionMap;
    int bHasTexturemap7;
    int bHasTexturemap8;
    int bHasTexturemap9;
    int bHasTexturemap10;
    int bHasTexturemap11;
    int bHasTexturemap12;
    int bHasTexturemap13;
    int bHasTexturemap14;
    int bHasTexturemap15;
};

cbuffer SubMapBuffer
{
    int bHasSubmap0;
    int bHasSubmap1;
    int bHasSubmap2;
    int bHasSubmap3;
    int bHasSubmap4;
    int bHasSubmap5;
    int bHasSubmap6;
    int bHasSubmap7;
    int bHasSubmap8;
    int bHasSubmap9;
    int bHasSubmap10;
    int bHasSubmap11;
    int bHasSubmap12;
    int bHasSubmap13;
    int bHasSubmap14;
    int bHasSubmap15;
};



Texture2DArray TransformMap;

int parentIndex[MAX_MODEL_TRANSFORM];


uint BoneIndex;

float2 g_UVSliding;

matrix GetAnimationMatrix(VTXModel input)
{
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
    float weights[4] = { input.blendWeight.x, input.blendWeight.y, input.blendWeight.z, input.blendWeight.w };
    if (input.blendIndices.x + input.blendIndices.y + input.blendIndices.z + input.blendIndices.w == 0.f)
    {
        indices[0] = BoneIndex;
        weights[0] = 1.f;
        
        for (int i = 1; i < 4; ++i)
        {
            indices[i] = 0;
            weights[i] = 0;

        }

    }
    {
        int animIndex[2] = { TweenFrames.curr.animIndex, TweenFrames.next.animIndex };
        int currFrame[2] = { TweenFrames.curr.currFrame, TweenFrames.next.currFrame };
        int nextFrame[2] = { TweenFrames.curr.nextFrame, TweenFrames.next.nextFrame };
        float ratio[2] = { TweenFrames.curr.ratio, TweenFrames.next.ratio };
    
        float4 c0, c1, c2, c3;
        float4 n0, n1, n2, n3;
        matrix curr = 0;
        matrix next = 0;
        matrix transform = 0;
    
        for (int i = 0; i < 4; ++i)
        {
            c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
            c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
            c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
            c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
            curr = matrix(c0, c1, c2, c3);
    
            n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
            n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
            n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
            n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
            next = matrix(n0, n1, n2, n3);
    
            matrix result = slerpMat(curr, next, ratio[0]);
            //matrix result = curr;
            if (animIndex[1] >= 0)
            {
                c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
                c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
                c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
                c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
                curr = matrix(c0, c1, c2, c3);
    
                n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
                n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
                n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
                n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
                next = matrix(n0, n1, n2, n3);
    
                matrix nextResult = slerpMat(curr, next, ratio[1]);
      
                result = slerpMat(result, nextResult, TweenFrames.tweenRatio);
              
            }
            result = mul(OffsetMatrix[indices[i]], result);
            result = mul(result, pivot);
                      
            transform += mul(weights[i], result);
    
        }
        return transform;
    }
}

matrix GetAnimationMatrix_Instance(VTXModelInstancing input)
{
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
    float weights[4] = { input.blendWeight.x, input.blendWeight.y, input.blendWeight.z, input.blendWeight.w };
    if (input.blendIndices.x + input.blendIndices.y + input.blendIndices.z + input.blendIndices.w == 0.f)
    {
        indices[0] = BoneIndex;
        weights[0] = 1.f;
        
        for (int i = 1; i < 4; ++i)
        {
            indices[i] = 0;
            weights[i] = 0;

        }

    }
    int animIndex[2] = { InstanceTweenFrames[input.instanceID].curr.animIndex, InstanceTweenFrames[input.instanceID].next.animIndex };
    int currFrame[2] = { InstanceTweenFrames[input.instanceID].curr.currFrame, InstanceTweenFrames[input.instanceID].next.currFrame };
    int nextFrame[2] = { InstanceTweenFrames[input.instanceID].curr.nextFrame, InstanceTweenFrames[input.instanceID].next.nextFrame };
    float ratio[2] = { InstanceTweenFrames[input.instanceID].curr.ratio, InstanceTweenFrames[input.instanceID].next.ratio };
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;
    
    for (int i = 0; i < 4; ++i)
    {
        c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
        c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
        c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
        c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
        curr = matrix(c0, c1, c2, c3);
      
        n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
        n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
        n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
        n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
        next = matrix(n0, n1, n2, n3);
      
        matrix result = slerpMat(curr, next, ratio[0]);
      
        if (animIndex[1] >= 0)
        {
            c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
            c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
            c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
            c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
            curr = matrix(c0, c1, c2, c3);
      
            n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
            n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
            n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
            n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
            next = matrix(n0, n1, n2, n3);
      
            matrix nextResult = slerpMat(curr, next, ratio[1]);
      

            result = slerpMat(result, nextResult, InstanceTweenFrames[input.instanceID].tweenRatio);

        }
        result = mul(OffsetMatrix[indices[i]], result);
        result = mul(result, pivot);
        transform += mul(weights[i], result);
    }
    
    return transform;
}

#endif