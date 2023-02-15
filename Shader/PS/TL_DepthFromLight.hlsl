#include "TL_IO_VS.hlsli"

#include "TL_OUT_PS.hlsli"

#include "TL_ConstantsPS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_Light.hlsli"

#include "TL_PBR.hlsli"

float4 main(VS_Out surface) : SV_Target0
{
    return float4(surface.pos_NDC.zzz, 1.0f);
}