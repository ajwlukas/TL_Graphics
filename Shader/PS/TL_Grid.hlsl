#include "TL_IO_VS.hlsli"

#include "TL_ConstantsPS.hlsli"


//화면 공간에서 z = 0 일때 그리드를 그려주는 쉐이더


//1차함수만 쓸 것
bool CloseToEqual(float a, float b, float gap)//gap == 선의 굵기
{
    return abs(a - b) < gap;
}

float4 main(VS_Out_ScreenSpace surface) : SV_Target0
{
	
    float4 NDC_xyz = float4(surface.uv * 2.0f - 1.0f, 0.0f, 1.0f);
	
    float3 rayOrigin = mul(viewInv, mul(projInv, NDC_xyz)).xyz;
	
    float3 rayDirection = rayOrigin - camPos;
	
	
    float t = -rayOrigin.z / rayDirection.z;
	
    float3 intersection = rayOrigin + t * rayDirection;
	
	
    if (CloseToEqual(intersection.x % 10, 0.0f, 1.0f))
        return float4(0, 0, 1, 1);
    
	
    if (CloseToEqual(intersection.y % 10, 0.0f, 1.0f))
        return float4(0, 0, 1, 1);
    
	
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}