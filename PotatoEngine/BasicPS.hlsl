#include "Shader.hlsli"

Texture2D Tex : register(t0);

float3 Phong(float3 toEye, float3 toLight, float ndotl, float3 normal) 
{   
    float diff = ndotl;
    float spec = max(dot(toEye, reflect(-toLight, normal)), 0.0);
    spec = pow(spec, 200.0);
    
    return float3(diffuse, diffuse, diffuse) * diff + float3(specular, specular, specular) * spec;
} 

float4 main(PSInput input) : SV_TARGET
{
    float3 toEye = normalize(eyePos - input.posWorld);
    float3 toLight = normalize(lightPos - input.posWorld);
    float ndotl = max(dot(input.normalWorld, toLight), 0.0);
    
    float3 color = Phong(toEye, toLight, ndotl, input.normalWorld);
    
    //return float4(color, 1.0);
    return float4(color, 1.0) * Tex.Sample(linearWarpSS, input.texcoord);
}