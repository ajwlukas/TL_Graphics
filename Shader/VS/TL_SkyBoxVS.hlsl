#include "TL_IO_VS.hlsli"

#include "TL_ConstantsVS.hlsli"
#include "TL_Transform.hlsli"

VS_Out_Pos main(VS_In_StaticMesh v)
{
	VS_Out_Pos ret;
	
    //skybox�� ī�޶��� Ʈ�������� ������.
	ret.pos_world = mul(viewInv, float4(v.pos_local, 1.0f)).xyz;
	
	//view�������� skybox�� localPos�� ����.
	ret.pos_NDC = mul(proj, v.pos_local);
    
	return ret;
}