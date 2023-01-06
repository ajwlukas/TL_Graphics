#ifndef TL_ConstantsPS
#define TL_ConstantsPS

cbuffer CameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float4x4 viewInv;
    float4x4 projInv;
    float3 camPos;
}


cbuffer Material : register(b11)
{
    float4 mMetallic;
    float4 mRoughness;
}

#endif