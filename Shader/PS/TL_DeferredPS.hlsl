#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_Light.hlsli"

#include "TL_PBR.hlsli"

#include "TL_GammaCorrection.hlsli"

//#include "TL_Reinhard.hlsli"

cbuffer material : register(b12)
{
	float lodLevel;
}

//TextureCube prefilteredEnvMap : register(t13); //Texture for IBL(Specular)
 //https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
float4 main(VS_Out_ScreenSpace surface) : SV_Target0
{
    
    float3 albedo = albedo_Deferred.Sample(Sampler_Wrap, surface.uv).rgb;
    float opacity = albedo_Deferred.Sample(Sampler_Wrap, surface.uv).a;
    float metalness = metalness_Deferred.Sample(Sampler_Wrap, surface.uv).r;
    float roughness = roughness_Deferred.Sample(Sampler_Wrap, surface.uv).r;
    //roughness = max(0.01f, roughness);
    
    float3 pos_world = pos_world_Deferred.Sample(Sampler_Wrap, surface.uv).rgb;
    float3 normal = normal_world_Deferred.Sample(Sampler_Wrap, surface.uv).rgb;
    
    albedo = sRGBtoLinear(albedo);
    
    //표면으로 부터 눈으로 향하는 방향 벡터
    float3 toEye = normalize(camPos - pos_world);
    
    
    // 노말과 눈방향의 각
    float nDotToEye = max(0.0, dot(normal, toEye));
    
    // 프레넬0의 값, 메탈의 경우 albedo 값으로 대체
    float3 F0 = lerp(Fdielectric, albedo, metalness);
    
    
    //Light 갯수 Load from register(t11), TL_TexturesPS 확인
    uint NumLights = (uint) Lights.Load(0).x;
    
    
    // 여러 광원으로부터 직접광 (Diffuse + Specular) 더해줄 변수 선언
    float3 directLighting = 0.0f;
    
    for (uint i = 0; i < NumLights; ++i)
    {
        Light light = LoadLightInfo(i);
        
        if (!IsValid(light, pos_world))
            continue;
        
        //빛으로 향하는 벡터
        float3 lightDir = GetLightDirection(light, pos_world);
        
        //감쇠된 빛의 세기
        float3 intensity = GetLightIntensity(light, pos_world);
        
        //표면과 빛의 각
        float nDotL = max(0.0f, dot(normal, lightDir));

        if (nDotL <= 0.0f)
            continue;
        
        //램버트 코사인에 의해 들어오는 조도
        float3 illuminance = intensity * nDotL;

		// 눈과 빛의 중간 벡터
        float3 halfVec = normalize(lightDir + toEye);

		// 하프벡터와 표면노멀벡터의 각
        float nDotH = max(0.0f, dot(normal, halfVec));

        // 하프벡터와 눈방향의 각
        float hDotE = max(0.0f, dot(halfVec, toEye));
        
        // 프레넬 항, 물질의 종류(F0가 바뀜), 눈과 표면(이경우 미세표면)의 각에 영향 받음
        float3 F = fresnelSchlick(F0, hDotE);
        
        
        // 분포함수, n을 노말로 갖고 있는 표면에 ,h를 노말로 갖고 있는 미세표면이 얼마나 있는지?
        // 사실 아직도 확신 못하겠음
        float D = D_Beckmann(roughness * roughness, nDotH);
        //float D = ndfGGX(cosLh, roughness);
        //float D = D_GGX(roughness * roughness, nDotH);
        
		// 기하감쇠율, masking, shadowing 으로 인한 빛의 손실율
        float G = gaSchlickGGX(nDotL, nDotToEye, roughness);

        // 빛의 굴절율, 반사율(프레넬 항)의 보수, 
        float3 refracted = float3(1.0f, 1.0f, 1.0f) - F;
        
        //금속의 경우 굴절된 빛을 모두 흡수해서 산란하지 않는다.
        float3 kd = lerp(refracted, float3(0.0f, 0.0f, 0.0f), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        float3 diffuse = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0f * nDotL * nDotToEye);

		// Total contribution for this light.
        directLighting = directLighting + illuminance * (diffuse + specularBRDF);
    }
    
    directLighting = directLighting;
    
    
    //환경으로부터의 간접광들, environmentMap을 광원으로 삼음
	float3 indirectLighting = 0.0f;
    
    {
        //Diffuse
		float3 ref = normalize(reflect(-toEye, normal));
        
        float3 diffuseIrradiance = sRGBtoLinear(irradianceMap.Sample(Sampler_Clamp, ref));
        
        // 빛의 입사각을 특정할 수 없어서 half대신 normal로
        float3 F = fresnelSchlick(F0, nDotToEye);
        
        // 빛의 굴절율, 반사율(프레넬 항)의 보수, 
        float3 refracted = float3(1.0f, 1.0f, 1.0f) - F;
        
        //금속의 경우 굴절된 빛을 모두 흡수해서 산란하지 않는다.
        float3 kd = lerp(refracted, float3(0.0f, 0.0f, 0.0f), metalness);
        
        float3 diffuse = kd * albedo * diffuseIrradiance;

        
        
        //Specular
        uint width, height, level;
        prefilteredEnvMap.GetDimensions(0, width, height, level);
        float3 specularIrradiance = sRGBtoLinear(prefilteredEnvMap.SampleLevel(Sampler_Clamp, ref, roughness * level));
        
        float2 specularBRDF = IBL_BRDF_LUT.Sample(Sampler_Clamp
        , float2(nDotToEye, roughness));
        
        float3 specular = specularIrradiance * (F0 * specularBRDF.x + specularBRDF.y);
        
        
        //return float4(0.0f, specularBRDF.g, 0.0f, 1.0f);
        //return float4(specularBRDF.r, 0.0f, 0.0f, 1.0f);
        
        indirectLighting = specular + diffuse;
	  
        
		}
    
	//indirectLighting = LinearTosRGB(indirectLighting);
    
    return float4(LinearTosRGB(directLighting + indirectLighting), opacity);
}