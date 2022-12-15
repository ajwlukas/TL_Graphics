//https://github.com/KhronosGroup/glTF-Sample-Viewer/blob/master/source/Renderer/shaders/pbr.frag 참고해서 만듦

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