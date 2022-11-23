#ifndef TL_ConstantsVS
#define TL_ConstantsVS

cbuffer CameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float3 camPos;
}

cbuffer World : register(b1)
{
    float4x4 world;
};


#endif