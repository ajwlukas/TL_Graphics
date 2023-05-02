#ifndef TL_ConstantsVS
#define TL_ConstantsVS

cbuffer CameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float4x4 viewInv;
    float4x4 projInv;
    float3 camPos;
    float frustumFar;
}

cbuffer World : register(b1)
{
    float4x4 world;
}

struct AnimationMatirces
{
    matrix g_BoneTransforms[200];
};

cbuffer SkinnedObject : register(b2)
{
    AnimationMatirces skinnedMatrices;
}

cbuffer SkinnedInstanced : register(b3)
{
    AnimationMatirces skinnedMatrices_instanced[5];
}

cbuffer a : register(b13)
{
    float4x4 test[1024];
}


//struct test
//{
//    float tete;
//};

//StructuredBuffer<test> a : register(b4);


#endif