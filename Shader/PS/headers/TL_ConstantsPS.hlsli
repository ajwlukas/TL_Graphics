#ifndef TL_ConstantsPS
#define TL_ConstantsPS

cbuffer CameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float3 camPos;
}


cbuffer Material : register(b11)
{
    float4 mMetallic;
    float4 mRoughness;
}

cbuffer lightInfo : register(b1)
{
    float4 directional;
}

#endif