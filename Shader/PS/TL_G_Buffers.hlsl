#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_Light.hlsli"

#include "TL_PBR.hlsli"

cbuffer material : register(b1)
{
    float metal;
    float rough;
}


PS_Out main(VS_Out surface)
{
    PS_Out ret;
    
    float3 albedo = float3(1, 1, 1);
    float metalness = metal;
    float roughness = rough;
    
    
    ret.out0 = float4(albedo, 0.0f); //albedo
    ret.out1 = float4(surface.normal_world, 1.0f); //normal_world
    ret.out2 = float4(surface.pos_world, 1.0f); //pos_world
    ret.out3 = float4(metal, 1.0f, 1.0f, 0.0f); //metalness
    ret.out4 = float4(rough, 0.0f, 0.0f, 0.0f); //roughness
    
    
    return ret;
}