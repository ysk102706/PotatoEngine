#include "Shader.hlsli"

cbuffer meshConstantData : register(b0) 
{
    matrix world;
    matrix view; 
    matrix proj; 
};

struct VSInput 
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct PSInput
{ 
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

PSInput main(VSInput input)
{
    PSInput output;
    output.position = float4(input.position, 1.0); 
    output.position = mul(output.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    output.texcoord = input.texcoord;
    return output;
}