#include "Shader.hlsli"

cbuffer meshConstantData : register(b0) 
{
    matrix world;
    matrix invTranspose;
}; 

PSInput main(VSInput input)
{
    PSInput output;
    output.pos = float4(input.pos, 1.0); 
    
    output.pos = mul(output.pos, world);
    output.posWorld = output.pos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.normalWorld = mul(float4(input.normal, 0.0), invTranspose).xyz; 
    normalize(output.normalWorld);
    
    output.texcoord = input.texcoord;
    
    return output;
}