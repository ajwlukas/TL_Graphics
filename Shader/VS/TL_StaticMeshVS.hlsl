#include "TL_IO_VS.hlsli"

#include "TL_ConstantsVS.hlsli"
#include "TL_Transform.hlsli"

VS_Out main(VS_In_StaticMesh v)
{
    VS_Out ret;
    
    ret.pos_world = LocalToWorld(float4(v.pos_local, 1.0f));
    ret.pos_NDC = WorldToNDC(float4(ret.pos_world, 1.0f));
    
    ret.uv = v.uv;
    
    
    float3 normal = normalize(v.normal_local);
    
    float3 tangent = normalize(v.tangent_local - (dot(v.tangent_local, v.normal_local) * v.normal_local));
    
    float3 bitangent = normalize(cross(tangent, v.normal_local));
    
    
    ret.normal_world = LocalToWorldDirOnly(normal);
    ret.tangent_world = LocalToWorldDirOnly(tangent);
    ret.bitangent_world = LocalToWorldDirOnly(bitangent);
    
    
	return ret;
}