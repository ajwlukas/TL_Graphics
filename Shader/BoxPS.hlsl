

cbuffer Color : register(b0)
{
    float4 color;
};

struct VSout
{
    float4 pos : SV_POSITION;
};

struct PSout
{
    float4 out0 : SV_Target0;
    float4 out1 : SV_Target1;
};

PSout main(VSout input)
{
    PSout result;
    
    result.out0 = color;
    
    result.out1 = float4(1, 0, 1, 1);
    
    return result;
}