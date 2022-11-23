#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_PBR.hlsli"

 //https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
PS_Out main(VS_Out surface)
{
    PS_Out ret;
    
	// Sample input textures to get shading model params.
    float3 albedo = { 1, 0, 0 };
    //float3 albedo = albedoMap.Sample(Sampler, surface.uv);
    //float metalness = matallicMap.Sample(Sampler, surface.uv);
    //float roughness = roughnessMap.Sample(Sampler, surface.uv);
    float metalness = 0.5f;
    float roughness = 0.5f;
    
	// Outgoing light direction (vector from world-space fragment position to the "eye").
    float3 Lo = normalize(camPos - surface.pos_world);
    
    // Get current fragment's normal and transform to world space.
    float3 N = surface.normal_world;
    
    // Angle between surface normal and outgoing light direction.
    float cosLo = max(0.0, dot(N, Lo));
    
    // Specular reflection vector.
    float3 Lr = 2.0 * cosLo * N - Lo;
    
    // Fresnel reflectance at normal incidence (for metals use albedo color).
    float3 F0 = lerp(Fdielectric, albedo, metalness);
    
    // Direct lighting calculation for analytical lights.
    float3 directLighting = 0.0;
    
    float3 light = float3(0, 0, -1);
    
    for (uint i = 0; i < NumLights; ++i)
    {
        float3 Li = light;
        float3 Lradiance = float3(1, 1, 1);

		// Half-vector between Li and Lo.
        float3 Lh = normalize(Li + Lo);

		// Calculate angles between surface normal and various light vectors.
        float cosLi = max(0.0, dot(N, Li));
        float cosLh = max(0.0, dot(N, Lh));

		// Calculate Fresnel term for direct lighting. 
        float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
		// Calculate normal distribution for specular BRDF.
        float D = ndfGGX(cosLh, roughness);
		// Calculate geometric attenuation for specular BRDF.
        float G = gaSchlickGGX(cosLi, cosLo, roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
        float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        float3 diffuseBRDF = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

		// Total contribution for this light.
        directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
    }
    
    
	// Final fragment color.
    ret.out0 = float4(directLighting, 1.0); //pbr
    
    /////////////////////////////////////////////////
    
    float nDotL = dot(surface.normal_world, light);
    
    ret.out1 = float4(albedo * nDotL, 1.0f); //legacy
    
    return ret;
}