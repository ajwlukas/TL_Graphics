#include "TL_IO_VS.hlsli"

#include "TL_ConstantsPS.hlsli"


//ȭ�� �������� z = 0 �϶� �׸��带 �׷��ִ� ���̴�


//1���Լ��� �� ��
bool CloseToEqual(float a, float b, float gap)//gap == ���� ����
{
    return abs(a - b) < gap;
}

float4 main(VS_Out_ScreenSpace surface) : SV_Target0
{
	
    float NDC_x = surface.uv.x * 2.0f - 1.0f;
    float NDC_y = -(surface.uv.y * 2.0f - 1.0f);
    
    float4 NDC_xyz = float4(NDC_x, NDC_y, 0.0f, 1.0f);
	
    float3 rayOrigin = mul(viewInv, mul(projInv, NDC_xyz)).xyz;
	
    float3 rayDirection = rayOrigin - camPos;
	
    if (rayDirection.y * camPos.y > 0) 
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
    
	
    float t = -rayOrigin.y / rayDirection.y;//�Ÿ�
    
    
    float alpha = (500 - t) / 500.0f;
	
    float3 intersection = rayOrigin + t * rayDirection;
    
    
    if (CloseToEqual(intersection.x, 0.0f, 0.5f))
        return float4(0, 0, 1, 1);
    
    if (CloseToEqual(intersection.z, 0.0f, 0.5f))
        return float4(1, 0, 0, 1);
    
	
	
    if (CloseToEqual(intersection.x % 10, 0.0f, 0.3f))
        return float4(1, 1, 1, alpha);
    
	
    if (CloseToEqual(intersection.z % 10, 0.0f, 0.3f))
        return float4(1, 1, 1, alpha);
    
	
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}