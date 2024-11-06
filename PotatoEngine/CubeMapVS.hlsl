#include "Shader.hlsli" 

struct CubeMapPSInput
{
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
}; 

CubeMapPSInput main(VSInput input)
{ 
    CubeMapPSInput output; 
    output.posWorld = input.pos; 
    output.pos = mul(float4(input.pos, 0.0), view);
    output.pos = mul(float4(output.pos.xyz, 1.0), proj); 

    return output; 
}