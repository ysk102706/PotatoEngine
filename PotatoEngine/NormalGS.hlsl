#include "Shader.hlsli"

cbuffer meshConstantData : register(b0)
{
    matrix world;
    matrix invTranspose;
}; 

struct GSInput
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
};

struct NormalPSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

[maxvertexcount(2)]
void main(point GSInput input[1], inout LineStream<NormalPSInput> outStream)
{ 
    NormalPSInput output;
    
    float4 posWorld = mul(input[0].pos, world); 
    float3 normalWorld = normalize(mul(input[0].normal, invTranspose).xyz);
    
    output.pos = mul(posWorld, view);
    output.pos = mul(output.pos, proj);
    output.color = float4(1.0, 0.0, 0.0, 1.0);
    outStream.Append(output);
    
    output.pos = posWorld + float4(normalWorld, 0.0) * 0.01;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.color = float4(1.0, 1.0, 0.0, 1.0);
    outStream.Append(output); 
}