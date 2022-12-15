#ifndef TL_TexturesPS
#define TL_TexturesPS

//0 ~ 10
Texture2D albedoMap : register(t0);
Texture2D matallicMap : register(t1);
Texture2D roughnessMap : register(t2);

//11 ~ 20
Buffer<float4> pointLights : register(t11);
Buffer<float4> spotLights : register(t12);

#endif