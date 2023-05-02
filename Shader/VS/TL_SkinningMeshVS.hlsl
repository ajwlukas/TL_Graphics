#include "TL_IO_VS.hlsli"

#include "TL_ConstantsVS.hlsli"
#include "TL_Transform.hlsli"

VS_Out main(VS_In_SkinningMesh v)
{
    VS_Out ret;

    // skin

    float3 posW = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);

    float totalWeight = 0;
    
    for (int i = 0; i < 4; i++)
    {
        int boneIndex = v.boneIndex[i];
        float boneWeight = v.boneWeight[i];

        if (boneIndex > -1 && boneIndex < 128)
        {
            totalWeight += boneWeight;
            
            posW += boneWeight * mul(skinnedMatrices.g_BoneTransforms[boneIndex], float4(v.pos_local, 1.0f)).xyz;
            normalL += boneWeight * mul((float3x3) skinnedMatrices.g_BoneTransforms[boneIndex], v.normal_local).xyz;
            tangentL += boneWeight * mul((float3x3) skinnedMatrices.g_BoneTransforms[boneIndex], v.tangent_local).xyz;
        }
    }

    // TODO: normalize랑 무엇이 다르지..?
    posW /= totalWeight;
    normalL /= totalWeight;
    tangentL /= totalWeight;

    ret.pos_world = posW;
    ret.pos_view = WorldToView(ret.pos_world);
    ret.pos_NDC = ViewToNDC(ret.pos_view);

    ret.uv = v.uv;

    ret.normal_world = normalize(normalL);
    ret.tangent_world = normalize(tangentL);
    ret.bitangent_world = LocalToWorldDirOnly(tangentL);
    
    ret.instanceID = 0;
    
    return ret;
}