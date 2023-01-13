#include "TL_ConstantsPS.hlsli"

#include "TL_IO_VS.hlsli"

#include "TL_Samplers.hlsli"

#include "TL_TexturesPS.hlsli"

cbuffer LUT_Info : register(b10)
{
    uint width;
    uint height;
};

float4 main(VS_Out_ScreenSpace surface) : SV_TARGET0
{
    float4 ret;
    
    float4 rgba = interPass.Sample(Sampler_Wrap, surface.uv);
    
    
    float2 rgUV = float2(rgba.r / (float) width, rgba.g / (float) height);
    
    ret.a = rgba.a;
    
    float depth = width * height;
    float maxIndex = depth - 1;
    
    float blueIndexOrigin = rgba.b * maxIndex;
    float blueIndexFloor = floor(blueIndexOrigin);
    
    float UIndex = blueIndexFloor % width;
    float VIndex = (uint) blueIndexFloor / (uint) width;
    
    float2 bUV = float2(UIndex / (float) width, VIndex / (float) height);
    
    float2 SampleUV = rgUV + bUV;
    
    float3 floorColor = LUT.Sample(Sampler_Wrap, SampleUV).rgb;
    
    UIndex = (UIndex + 1) % width;
    VIndex = (uint) (blueIndexFloor + 1) / (uint) width;
    
    bUV = float2(UIndex / (float) width, VIndex / (float) height);
    
    float2 SampleUV2 = rgUV + bUV;
    
    float3 ceilColor = LUT.Sample(Sampler_Wrap, SampleUV2).rgb;
    
    float ratio = blueIndexOrigin - blueIndexFloor;
    
    ret.rgb = lerp(floorColor, ceilColor, ratio);
    
    return ret;
    
    //float4 ret;
    
    //float4 rgba = interPass.Sample(Sampler_Wrap, surface.uv);
    
    //float rOffset = rgba.r / width;
    //float gOffset = rgba.g / height;
    
    //ret.rg = LUT.Sample(Sampler_Wrap, float2(rOffset, gOffset)).rg;
    
    
    //float depth = width * height;
    //float maxIndex = depth - 1;
    
    //float blueIndexOrigin = rgba.b * maxIndex;
    
    //float blueIndexFloor = floor(blueIndexOrigin);
    //float blueIndexCeil = blueIndexFloor + 1; //ceil 함수 안쓰는 이유 origin이 정확히 정수면 좀 애매해져서
    
    //float floorRow = floor(blueIndexFloor / width);
    //float floorColumn = blueIndexFloor % width;
    
    //float floorUStart = floorColumn / width;
    //float floorVStart = floorRow / height;
    
    //float2 middleUVOffset = float2(0.5f / width, 0.5f / height);
    
    //float blueFloor = LUT.Sample(Sampler_Wrap, float2(floorUStart + middleUVOffset.x, floorVStart + middleUVOffset.y)).b;
    
    //float ceilRow = floorRow + floor((floorColumn + 1) / width);
    //float ceilColumn = (floorColumn + 1) % width;
    
    //float ceilUStart = ceilColumn / width;
    //float ceilVStart = ceilRow / height;
    
    //float blueCeil = LUT.Sample(Sampler_Wrap, float2(ceilUStart + middleUVOffset.x, ceilVStart + middleUVOffset.y)).b;
    
    //float ratio = blueIndexOrigin - blueIndexFloor;
    
    //ret.b = lerp(blueFloor, blueCeil, ratio);
    
    //return float4(ret.rgb, rgba.a);
    
    //float depth = width * height;
    //float maxIndex = depth - 1;
    
    //float blueIndexOrigin = rgba.b * maxIndex;
    
    //float blueIndexFloor = floor(blueIndexOrigin);
    
    //float floorRow = blueIndexFloor / width;
    //float floorColumn = blueIndexFloor % width;
    
    //float floorUStart = floorColumn / width;
    //float floorVStart = floorRow / height;
    
    //float floorU = floorUStart + rOffset;
    //float floorV = floorVStart + gOffset;
    
    //float2 floorUV = float2(floorU, floorV);
    
    
    //float3 floorColor = LUT.Sample(Sampler_Wrap, floorUV).rgb;
    
    
    //float blueIndexCeil = blueIndexOrigin + 1; //ceil 함수 안쓰는 이유 origin이 정확히 정수면 좀 애매해져서
    
    
    //float row_ceil = blueIndexCeil / width;
    //float column_ceil = blueIndexCeil % width;
    
    //float UStart_ceil = column_ceil / width;
    //float VStart_ceil = row_ceil / height;
    
    //float U_ceil = UStart_ceil + rOffset;
    //float V_ceil = VStart_ceil + gOffset;
    
    //float2 UV_ceil = float2(U_ceil, V_ceil);
    
    //float3 color_ceil = LUT.Sample(Sampler_Wrap, UV_ceil).rgb;
    
    
    //float ratio = (blueIndexOrigin - blueIndexFloor);
    
    //float3 ret = lerp(floorColor, color_ceil, ratio);
    
	
    //return float4(ret, rgba.a);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //float4 rgba = interPass.Sample(Sampler_Wrap, surface.uv);
    
    //float rOffset = rgba.r / width;
    //float gOffset = rgba.g / height;
    
    //float depth = width * height;
    //float maxIndex = depth - 1;
    
    //float blueIndexOrigin = rgba.b * maxIndex;
    
    //float blueIndexFloor = floor(blueIndexOrigin);
    
    //float floorRow = blueIndexFloor / width;
    //float floorColumn = blueIndexFloor % width;
    
    //float floorUStart = floorColumn / width;
    //float floorVStart = floorRow / height;
    
    //float floorU = floorUStart + rOffset;
    //float floorV = floorVStart + gOffset;
    
    //float2 floorUV = float2(floorU, floorV);
    
    
    //float3 floorColor = LUT.Sample(Sampler_Wrap, floorUV).rgb;
    
    
    //float blueIndexCeil = blueIndexOrigin + 1; //ceil 함수 안쓰는 이유 origin이 정확히 정수면 좀 애매해져서
    
    
    //float row_ceil = blueIndexCeil / width;
    //float column_ceil = blueIndexCeil % width;
    
    //float UStart_ceil = column_ceil / width;
    //float VStart_ceil = row_ceil / height;
    
    //float U_ceil = UStart_ceil + rOffset;
    //float V_ceil = VStart_ceil + gOffset;
    
    //float2 UV_ceil = float2(U_ceil, V_ceil);
    
    //float3 color_ceil = LUT.Sample(Sampler_Wrap, UV_ceil).rgb;
    
    
    //float ratio = (blueIndexOrigin - blueIndexFloor);
    
    //float3 ret = lerp(floorColor, color_ceil, ratio);
    
	
    //return float4(ret, rgba.a);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //float4 rgba = interPass.Sample(Sampler_Wrap, surface.uv);
    
    //float2 sampleUV;
    
    //sampleUV.y = rgba.g;
    
    //float depth = sqrt((float) texInfo[40].texX);
    
    //float r = rgba.r / depth;

    //float original = rgba.b * (depth - 1) / depth;
    //float leftStart = floor(rgba.b * (depth - 1)) / depth;
    //float rightStart = (floor(rgba.b * (depth - 1)) + 1) / depth;
    
    //sampleUV.x = r + leftStart;
    
    //float3 color1 = LUT.Sample(Sampler_Wrap, sampleUV).rgb;
    
    //sampleUV.x = r + rightStart;
    
    //float3 color2 = LUT.Sample(Sampler_Wrap, sampleUV).rgb;
    
    //float ratio = (original - leftStart) / depth;
    
    //float3 ret = lerp(color1, color2, ratio);
    
	
    //return float4(ret, rgba.a);

}