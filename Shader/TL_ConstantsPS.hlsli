#ifndef TL_ConstantsPS
#define TL_ConstantsPS

cbuffer CameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float3 camPos;
}


#endif