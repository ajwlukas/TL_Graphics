#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_Light.hlsli"

#include "TL_PBR.hlsli"

cbuffer material : register(b1)
{
    float metal;
    float rough;
}

 //https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
PS_Out main(VS_Out surface)
{
    PS_Out ret;
    
    
    
	// Sample input textures to get shading model params.
    //float3 albedo = albedoMap.;
    //float3 albedo = albedoMap.Sample(Sampler, surface.uv);
    //float metalness = matallicMap.Sample(Sampler, surface.uv);
    //float roughness = roughnessMap.Sample(Sampler, surface.uv);
    //float metalness = mMetallic.r;
    //float roughness = mRoughness.r;
    
    float3 albedo = float3(1, 1, 1);
    float metalness = metal;
    float roughness = rough;
    
    
    
    //ǥ������ ���� ������ ���ϴ� ���� ����
    float3 toEye = normalize(cam.camPos - surface.pos_world);
    
    //ǥ���� �븻
    float3 normal = surface.normal_world;
    
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
        
        if (!IsValid(light, surface.pos_world))
            continue;
        
        //������ ���ϴ� ����
        float3 lightDir = GetLightDirection(light, surface.pos_world);
        
        //����� ���� ����
        float3 intensity = GetLightIntensity(light, surface.pos_world);
        
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
        //float D = D_GGX(roughness * roughness, cosLh);
        
		// ���ϰ�����, masking, shadowing ���� ���� ���� �ս���
        float G = gaSchlickGGX(nDotL, nDotToEye, roughness);

        // ���� ������, �ݻ���(������ ��)�� ����, 
        float3 refracted = float3(1.0f, 1.0f, 1.0f) - F;
        
        //�ݼ��� ��� ������ ���� ��� ����ؼ� ������� �ʴ´�.
        float3 kd = lerp(refracted, float3(0.0f, 0.0f, 0.0f), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        float3 diffuseBRDF = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0f * nDotL * nDotToEye);

		// Total contribution for this light.
        directLighting = directLighting + illuminance * (diffuseBRDF + specularBRDF);

    }
    
    
	// Final fragment color.
    ret.out0 = float4(directLighting, 1.0f); //pbr
    
    //ret.out0 = float4(1, 1, 1, 1);
    
    /////////////////////////////////////////////////
    
    //float nDotL = dot(surface.normal_world, light);
    
    //ret.out1 = float4(albedo * nDotL, 1.0f); //legacy
    
    return ret;
}