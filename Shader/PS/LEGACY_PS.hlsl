#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_PBR.hlsli"

PS_Out main(VS_Out surface)
{
    PS_Out ret;
    
    float3 albedo = float3(1, 0, 0);
    
	// Final fragment color.
    ret.out0 = float4(directLighting, 1.0); //pbr
    
    //ret.out0 = float4(1, 1, 1, 1);
    
    /////////////////////////////////////////////////
    
    //float nDotL = dot(surface.normal_world, light);
    
    //ret.out1 = float4(albedo * nDotL, 1.0f); //legacy
    
    return ret;
}