#ifndef TL_TexturesPS
#define TL_TexturesPS

//paired with ShaderResourceSlotPS.h

//0 ~ 10
Texture2D baseColor_opcityMap : register(t0);
Texture2D roughness_specular_metallic_AOMap : register(t1);
Texture2D normalMap : register(t2);
Texture2D emissiveMap : register(t3);
//Texture2D alphaMap : register(t4);
//Texture2D emissiveMap : register(t5);
//Texture2D AO_Map : register(t6);


//10 ~ 19
TextureCube cubeMap : register(t10);
Buffer<float4> Lights : register(t11);//first float4 represent metaData


//20 ~ 29 Textures for Deferred Lighting, generated by GBufferRenderPass
Texture2D albedo_Deferred : register(t20);
Texture2D normal_world_Deferred : register(t21);
Texture2D pos_world_Deferred : register(t22);
Texture2D metalness_Deferred: register(t23);
Texture2D roughness_Deferred: register(t24);
Texture2D depth_Deferred: register(t25);


//30 ~ 39
Texture2D mainRender: register(t30);

Texture2D interPass : register(t31);
Texture2D sampledTexture : register(t32);


//40 ~ 49
Texture3D LUT : register(t40);

Texture2D accumulateA : register(t41);
Texture2D accumulateB : register(t42);
#endif