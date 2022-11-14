

cbuffer Color : register(b0)
{
    float4 color;
};

struct VSout
{
    float4 pos : SV_POSITION;
};

float4 main(VSout input) : SV_Target
{
    
    return color;
}