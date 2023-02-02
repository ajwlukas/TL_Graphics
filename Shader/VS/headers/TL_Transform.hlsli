#ifndef TL_Transform
#define TL_Transform



#include "TL_ConstantsVS.hlsli"

///버텍스용
float3 LocalToWorld(float3 localPos)//로컬 포지션을 월드로
{
	return mul(world, float4(localPos, 1.0f)).xyz;
}

float4 WorldToNDC(float3 worldPos)
{
	float4 pos_view = mul(view, float4(worldPos, 1.0f));
    
	return mul(proj, pos_view);
}
float4 LocalToNDC(float3 localPos)//로컬 포지션을 월드로
{
    float3 pos_world = LocalToWorld(localPos);
    
	return WorldToNDC(pos_world);
}

///벡터용
float3 LocalToWorldDirOnly(float3 localDir)//로컬 포지션을 월드로
{
    return mul((float3x3) world, localDir);
}

float3 WorldToNDCDirOnly(float3 worldDir)
{
    float3 ret = mul((float3x3) view, worldDir);
    ret = mul((float3x3) proj, ret);
    
    return ret;
}

float3 LocalToNDCDirOnly(float3 localDir)//로컬 포지션을 월드로
{
    float3 ret = LocalToWorldDirOnly(localDir);
    ret = WorldToNDCDirOnly(ret);
    
    return ret;
}

/////버텍스용
//float3 WorldToLocal(float3 localPos)//월드 포지션을 월드로
//{
//	return mul(world, float4(localPos, 1.0f));
//}

//float4 NDCToWorld(float4 worldPos)
//{
//    float4 ret = mul(view, worldPos);
//    ret = mul(proj, ret);
    
//    return ret;
//}
//float4 NDCToLocal(float4 localPos)//로컬 포지션을 월드로
//{
//    float4 ret = LocalToWorld(localPos);
//    ret = WorldToNDC(ret);
    
//    return ret;
//}

///쓰지 말것 구현한 것 아님, 
/////벡터용
//float3 WorldToLocalDirOnly(float3 localDir)//로컬 포지션을 월드로
//{
//    return mul((float3x3) world, localDir);
//}

//float3 NDCToWorldDirOnly(float3 worldDir)
//{
//    float3 ret = mul((float3x3) view, worldDir);
//    ret = mul((float3x3) proj, ret);
    
//    return ret;
//}

//float3 NDCToLocalDirOnly(float3 localDir)//로컬 포지션을 월드로
//{
//    float3 ret = LocalToWorldDirOnly(localDir);
//    ret = WorldToNDCDirOnly(ret);
    
//    return ret;
//}

#endif