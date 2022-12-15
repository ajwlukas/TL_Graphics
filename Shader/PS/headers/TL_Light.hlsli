#ifndef TL_Light
#define TL_Light

#include "TL_TexturesPS.hlsli"

struct LightMetaData
{
    uint lightNum;
};

struct Light
{
    int type;
    float3 position;
    
    float intensity;
    float3 direction;
    
    float range;
    float3 attenuation;

    float spot;
    float3 color;
};

const int LightType_Directional = 0;
const int LightType_Point = 1;
const int LightType_Spot = 2;

const int metaDataOffset = 1;

Light LoadLightInfo(uint index)
{
    Light light;
    
    float4 type_position = Lights.Load(1 + 4 * index + 0);//1 for lightNum
    float4 intensity_direction = Lights.Load(1 + 4 * index + 1);
    float4 range_attenuation = Lights.Load(1 + 4 * index + 2);
    float4 spot_color = Lights.Load(1 + 4 * index + 3);
    
    light.type = (uint)type_position.x;
    light.position = type_position.yzw;
    
    light.intensity = intensity_direction.x;
    light.direction = intensity_direction.yzw;
    
    light.range = range_attenuation.x;
    light.attenuation = range_attenuation.yzw;
    
    light.spot = spot_color.x;
    light.color = spot_color.yzw;
    
    return light;
}

float getRangeAttenuation(float range, float distance)
{
    if (range <= 0.0)
    {
        // negative range means unlimited
        return 1.0 / pow(distance, 2.0);
    }
    return max(min(1.0 - pow(distance / range, 4.0), 1.0), 0.0) / pow(distance, 2.0);
}

float3 GetLighIntensity(Light light, float3 surfaceToLight)
{
    float rangeAttenuation = 1.0;
    float spotAttenuation = 1.0;

    if (light.type != LightType_Directional)
    {
        rangeAttenuation = getRangeAttenuation(light.range, length(surfaceToLight));
    }
    //if (light.type == LightType_Spot)
    //{
    //    spotAttenuation = getSpotAttenuation(surfaceToLight, light.direction, light.outerConeCos, light.innerConeCos);
    //}

    return rangeAttenuation * spotAttenuation * light.intensity * light.color;
}
//https://github.com/KhronosGroup/glTF-Sample-Viewer/blob/master/source/Renderer/shaders/pbr.frag 참고해서 만듦

#endif