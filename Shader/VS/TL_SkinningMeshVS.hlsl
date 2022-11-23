#include "TL_IO_VS.hlsli"

#include "TL_ConstantsVS.hlsli"
#include "TL_Transform.hlsli"

VS_Out main(VS_In_SkinningMesh v)
{
    VS_Out ret;
    
    ret.pos_world = LocalToWorld(float4(v.pos_local, 1.0f));
    ret.pos_NDC = WorldToNDC(float4(ret.pos_world, 1.0f));
    
    ret.uv = v.uv;
    
    ret.normal_world = LocalToWorldDirOnly(v.normal_local);
    ret.tangent_world = LocalToWorldDirOnly(v.tangent_local);
    ret.bitangent_world = LocalToWorldDirOnly(v.bitangent_local);
    
    return ret;
}