#include "TL_IO_VS.hlsli"

//화면 공간에서 z = 0 일때 그리드를 그려주는 쉐이더

float4 main(VS_Out_ScreenSpace surface) : SV_Target0
{
	
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}