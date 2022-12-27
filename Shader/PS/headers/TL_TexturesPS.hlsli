#ifndef TL_TexturesPS
#define TL_TexturesPS

//0 ~ 10
Texture2D albedoMap : register(t0);
Texture2D matallicMap : register(t1);
Texture2D roughnessMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D alphaMap : register(t4);
Texture2D emissiveMap : register(t5);
Texture2D AO_Map : register(t6);

//11 ~ 20
Buffer<float4> Lights : register(t11);//first float4 represent metaData


//21 ~ 30
Texture2D albedo_Deferred : register(t20);
Texture2D normal_world_Deferred : register(t21);
Texture2D pos_world_Deferred : register(t22);
Texture2D metalness_Deferred: register(t23);
Texture2D roughness_Deferred: register(t24);
#endif