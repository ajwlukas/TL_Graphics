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


//인스턴싱 위한 임시 코드
Buffer<float4> SkinningInfos;

float4x4 GetBone(uint instanceID, uint boneIndex)
{
    //한 인스턴스당 본의 갯수
    uint boneNum = SkinningInfos.Load(0).x;
    
    float4 r0 = SkinningInfos.Load(1 + boneNum * instanceID + boneIndex + 0);
    float4 r1 = SkinningInfos.Load(1 + boneNum * instanceID + boneIndex + 1);
    float4 r2 = SkinningInfos.Load(1 + boneNum * instanceID + boneIndex + 2);
    float4 r3 = SkinningInfos.Load(1 + boneNum * instanceID + boneIndex + 3);
    
    
    
    return float4x4(r0, r1, r2, r3);
}