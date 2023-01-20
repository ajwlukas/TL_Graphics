#pragma once

//paired with TL_TexturePS.hlsli

//0 ~ 10
constexpr unsigned int baseColor_opacityMapSlot = 0;
constexpr unsigned int roughness_specular_metallic_AOMapSlot = 1;
constexpr unsigned int normalMapSlot = 2;
constexpr unsigned int emissiveMapSlot = 3;
//constexpr unsigned int alphaMapSlot = 4;
//constexpr unsigned int emissiveMapSlot = 5;
//constexpr unsigned int AO_MapSlot = 6;


//11 ~ 20
constexpr unsigned int LightsSlot = 11;//first float4 represent metaData


//21 ~ 30 Textures for Deferred Lighting, generated by GBufferRenderPass
constexpr unsigned int albedo_DeferredSlot = 20;
constexpr unsigned int normal_world_DeferredSlot = 21;
constexpr unsigned int pos_world_DeferredSlot = 22;
constexpr unsigned int metalness_DeferredSlot = 23;
constexpr unsigned int roughness_DeferredSlot = 24;


//31 ~ 40
constexpr unsigned int mainRenderSlot = 30;
constexpr unsigned int interPassSlot = 31;
constexpr unsigned int sampledTextureSlot = 32;


//40 ~ 49
constexpr unsigned int LUTSlot = 40;