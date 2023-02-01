#include "TL_IO_VS.hlsli"

#include "TL_ConstantsVS.hlsli"
#include "TL_Transform.hlsli"

VS_Out_Pos main(VS_In_StaticMesh v)
{
	VS_Out_Pos ret;
	
    //skybox는 카메라의 트랜스폼을 따른다.
	ret.pos_world = mul(viewInv, float4(v.pos_local, 1.0f)).xyz;
	
	//view공간에서 skybox는 localPos와 같다.
	ret.pos_NDC = mul(proj, v.pos_local);
    
	return ret;
}