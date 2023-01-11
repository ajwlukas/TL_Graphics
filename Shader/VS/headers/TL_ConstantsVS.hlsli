#ifndef TL_ConstantsVS
#define TL_ConstantsVS

cbuffer CameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float4x4 viewInv;
    float4x4 projInv;
    float3 camPos;
}

cbuffer World : register(b1)
{
    float4x4 world;
}

cbuffer SkinnedObject : register(b2)
{
    matrix g_BoneTransforms[128];
}

#endif