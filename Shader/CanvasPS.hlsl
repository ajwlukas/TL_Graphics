SamplerState Sampler : register(s0);

Texture2D diffuseMap : register(t0);
struct VSout
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

float4 main(VSout input) : SV_Target0
{
    float4 ret = float4(1, 0, 1, 1);
    
    ////////////////////////////////
    
    ret = diffuseMap.Sample(Sampler, input.uv);
    
    ////////////////////////////////
    
    ret.a = 1.0f;
    
    return ret;
}