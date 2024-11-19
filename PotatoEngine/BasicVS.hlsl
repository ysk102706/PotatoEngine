#include "Shader.hlsli"

Texture2D heightMap : register(t0);

cbuffer meshConstantData : register(b0) 
{
    matrix world;
    matrix invTranspose; 
    int useHeightMap;
    float3 dummy; 
}; 

PSInput main(VSInput input)
{
    PSInput output;
    output.pos = mul(float4(input.pos, 1.0), world); 
    
    output.normalWorld = normalize(mul(float4(input.normal, 0.0), invTranspose).xyz);
    
    if (useHeightMap)
    {
        float height = heightMap.SampleLevel(linearClampSS, input.texcoord, 0.0).r; 
        height = 2.0 * height - 1.0; 
        output.pos += float4(height * output.normalWorld, 0.0);
    }
    
    output.posWorld = output.pos.xyz; 
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.texcoord = input.texcoord; 
    output.tangent = mul(float4(input.tangent, 0.0), world).xyz;
    
    return output;
}