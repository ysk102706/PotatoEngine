#include "Shader.hlsli"

Texture2D Tex : register(t0); 

float4 main(PSInput input) : SV_TARGET
{
    float3 toEye = normalize(eyePos - input.posWorld);
    
    float3 color = 0.0; 
    
    [unroll] 
    for (int i = 0; i < MAXLIGHT; i++)
    {
        if (light[i].lightType == 0)
            color += DirectionalLight(light[i], toEye, input.normalWorld, mat);
        if (light[i].lightType == 1)
            color += PointLight(light[i], toEye, input.posWorld, input.normalWorld, mat);
        if (light[i].lightType == 2)
            color += SpotLight(light[i], toEye, input.posWorld, input.normalWorld, mat);
        
    }
    
    //return float4(color, 1.0);
    return float4(color, 1.0) * Tex.Sample(linearWarpSS, input.texcoord);
}