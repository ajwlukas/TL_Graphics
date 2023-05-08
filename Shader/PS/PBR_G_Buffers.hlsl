#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_Samplers.hlsli"

Texture2D albedoMap : register(t0);
Texture2D roughnessMap : register(t1);
Texture2D metallicMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D AOMap : register(t4);

PS_Out main(VS_Out surface)
{
    PS_Out ret;
    
    float3 albedo = albedoMap.Sample(Sampler_Wrap, surface.uv).rgb;
    float roughness = roughnessMap.Sample(Sampler_Wrap, surface.uv).r;
    float metallic = metallicMap.Sample(Sampler_Wrap, surface.uv).r;
    float3 normal = normalMap.Sample(Sampler_Wrap, surface.uv).xyz;
    float ao = AOMap.Sample(Sampler_Wrap, surface.uv).r;
    
    //normal Mapping
    
    normal = (normal * 2.0f) - 1.0f;
    
    float3x3 TBN = float3x3(surface.tangent_world, surface.bitangent_world, surface.normal_world);
    
    normal = normalize(mul(normal, TBN));
    ///
    
    //float3 emissive = emissiveMap.Sample(Sampler_Clamp, surface.uv);
    
    //todo : 추후 opacity 추가할 것
    //ret.out0 = basecolor_opacity; //albedo
    ret.out0 = float4(albedo.rgb, 1.0f); //albedo

    ret.out1 = float4(normal, 1.0f); //normal_world
    //ret.out1 = float4(surface.normal_world, 1.0f); //normal_world
    
    ret.out2 = float4(surface.pos_world, 1.0f); //pos_world
    ret.out3 = float4(metallic, 1.0f, 1.0f, 0.0f); //metalness
    ret.out4 = float4(roughness, 0.0f, 0.0f, 0.0f); //roughness
    ret.out5 = float4(surface.pos_NDC.z, 0.0f, 0.0f, 0.0f);
    ret.out6 = float4(ao, 0.0f, 0.0f, 0.0f);
    ret.out7 = float4(0.0f,0.0f,0.0f, 0.0f);
    
    return ret;
}