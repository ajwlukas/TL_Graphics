#include "TL_ConstantsPS.hlsli"

#include "TL_IO_VS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Reinhard.hlsli"
// ReinHard ToneMapping, https://64.github.io/tonemapping/
// + DirectX Sample HDRToneMapping

cbuffer Dir_Info : register(b10)
{
    float middleGrey;
    float maxWhite;
};

float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
{
    //float4 vColor = 0;
    float4 vColor = sourceSlot0.Sample(Sampler_Clamp, surface.uv);
    
    //명순응 암순응도 따로 빼줄까..?
    //float4 vLum = screenLuminance.Sample(Sampler_Clamp, float2(0, 0));

    //float3 vBloom = s2.Sample(LinearSampler, Input.Tex);
    
    // Tone mapping
    //vColor.rgb *= middleGrey / (vLum.r + 0.001f);
    
    //bloom 따로 빼주는 것이 어떠신지...?
    //vColor.rgb += 0.6f * vBloom;

    float luminance = Luminance(vColor.rgb);
    
    luminance = Reinhard_Luminance(luminance, maxWhite);
    
    vColor.rgb = Change_luminance(vColor.rgb, luminance);
    
    
    vColor.a = 1.0f;
    
    return vColor;

}
