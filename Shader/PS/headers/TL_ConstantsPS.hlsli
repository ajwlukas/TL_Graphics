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

struct TextureInfo
{
    uint texX;//as Pixel
    uint texY;
    float texXInv;//as UV
    float texYInv;
};

cbuffer RenderTargetTexInfo : register(b2)//¾²·Á³ª...? ÀÏ´Ü ¹Ì·ï³ð
{
    TextureInfo rttInfo[8]; //index means slot of renderTarget
}

cbuffer TexInfo : register(b3)
{
    TextureInfo texInfo[4096]; //index means slot of texture ref TL_TexturePS
}


cbuffer Material : register(b11)
{
    float4 mMetallic;
    float4 mRoughness;
}

#endif