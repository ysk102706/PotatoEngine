#include "Shader.hlsli"

Texture2D Tex : register(t0); 
 
cbuffer MaterialConstantData : register(b0) 
{
    Material mat;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 toEye = normalize(eyePos - input.posWorld);
    
    float3 color = float3(0.0, 0.0, 0.0);
    
    color += DirectionalLight(light, toEye, input.normalWorld, mat);
    
    //return Tex.Sample(linearWarpSS, input.texcoord); 
    return float4(color, 1.0);
    //return float4(color, 1.0) * Tex.Sample(linearWarpSS, input.texcoord);
}