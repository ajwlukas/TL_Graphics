
struct VSin
{
    float3 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
};



float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}