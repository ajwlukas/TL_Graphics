#include "TL_IO_VS.hlsli"

#include "TL_TexturesPS.hlsli"

#include "TL_Samplers.hlsli"

float4 main(VS_Out_ScreenSpace surface) : SV_Target0
{
    return accumulateA.Sample(Sampler_Wrap, surface.uv) + accumulateB.Sample(Sampler_Wrap, surface.uv);
}