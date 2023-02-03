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
    
    //ǥ������ ���� ������ ���ϴ� ���� ����
    float3 toEye = normalize(camPos - pos_world);
    
    
    // �븻�� �������� ��
    float nDotToEye = max(0.0, dot(normal, toEye));
    
    // ������0�� ��, ��Ż�� ��� albedo ������ ��ü
    float3 F0 = lerp(Fdielectric, albedo, metalness);
    
    
    //Light ���� Load from register(t11), TL_TexturesPS Ȯ��
    uint NumLights = (uint) Lights.Load(0).x;
    
    
    // ���� �������κ��� ������ (Diffuse + Specular) ������ ���� ����
    float3 directLighting = 0.0f;
    
    for (uint i = 0; i < NumLights; ++i)
    {
        Light light = LoadLightInfo(i);
        
        if (!IsValid(light, pos_world))
            continue;
        
        //������ ���ϴ� ����
        float3 lightDir = GetLightDirection(light, pos_world);
        
        //����� ���� ����
        float3 intensity = GetLightIntensity(light, pos_world);
        
        //ǥ��� ���� ��
        float nDotL = max(0.0f, dot(normal, lightDir));

        if (nDotL <= 0.0f)
            continue;
        
        //����Ʈ �ڻ��ο� ���� ������ ����
        float3 illuminance = intensity * nDotL;

		// ���� ���� �߰� ����
        float3 halfVec = normalize(lightDir + toEye);

		// �������Ϳ� ǥ���ֺ����� ��
        float nDotH = max(0.0f, dot(normal, halfVec));

        // �������Ϳ� �������� ��
        float hDotE = max(0.0f, dot(halfVec, toEye));
        
        // ������ ��, ������ ����(F0�� �ٲ�), ���� ǥ��(�̰�� �̼�ǥ��)�� ���� ���� ����
        float3 F = fresnelSchlick(F0, hDotE);
        
        
        // �����Լ�, n�� �븻�� ���� �ִ� ǥ�鿡 ,h�� �븻�� ���� �ִ� �̼�ǥ���� �󸶳� �ִ���?
        // ��� ������ Ȯ�� ���ϰ���
        float D = D_Beckmann(roughness * roughness, nDotH);
        //float D = ndfGGX(cosLh, roughness);
        //float D = D_GGX(roughness * roughness, nDotH);
        
		// ���ϰ�����, masking, shadowing ���� ���� ���� �ս���
        float G = gaSchlickGGX(nDotL, nDotToEye, roughness);

        // ���� ������, �ݻ���(������ ��)�� ����, 
        float3 refracted = float3(1.0f, 1.0f, 1.0f) - F;
        
        //�ݼ��� ��� ������ ���� ��� ����ؼ� ������� �ʴ´�.
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
    
    
    //ȯ�����κ����� ��������, environmentMap�� �������� ����
	float3 indirectLighting = 0.0f;
    
    {
        //Diffuse
		float3 ref = normalize(reflect(-toEye, normal));
        
        float3 diffuseIrradiance = sRGBtoLinear(irradianceMap.Sample(Sampler_Clamp, ref));
        
        // ���� �Ի簢�� Ư���� �� ��� half��� normal��
        float3 F = fresnelSchlick(F0, nDotToEye);
        
        // ���� ������, �ݻ���(������ ��)�� ����, 
        float3 refracted = float3(1.0f, 1.0f, 1.0f) - F;
        
        //�ݼ��� ��� ������ ���� ��� ����ؼ� ������� �ʴ´�.
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