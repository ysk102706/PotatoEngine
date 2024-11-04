#include "Shader.hlsli"

Texture2D Tex : register(t0); 

float4 main(PSInput input) : SV_TARGET
{
    float3 toEye = normalize(eyePos - input.posWorld);
    
    return Tex.Sample(linearWarpSS, input.texcoord); 
    //return float4(1.0, 1.0, 1.0, 1.0);
    //return float4(color, 1.0) * Tex.Sample(linearWarpSS, input.texcoord);
}