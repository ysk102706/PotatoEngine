#include "Shader.hlsli"
#include "PhongShading.hlsli" 
#include "RimEffect.hlsli" 
#include "FresnelEffect.hlsli"

Texture2D Tex : register(t0); 
 
cbuffer MaterialConstantData : register(b0) 
{
    PhongShading mat;
    RimEffect rim; 
    FresnelEffect fresnel; 
}; 

float4 main(PSInput input) : SV_TARGET
{
    float3 toEye = normalize(eyePos - input.posWorld);
    
    //float3 color = float3(0.0, 0.0, 0.0);
    
    //color += DirectionalLight(light, toEye, input.normalWorld, mat);
    float4 diffuse = diffuseTex.Sample(linearWarpSS, input.normalWorld);
    float4 specular = specularTex.Sample(linearWarpSS, reflect(-toEye, input.normalWorld));
    
    specular *= pow((specular.x + specular.y + specular.z) / 3.0, mat.shininess);
    
    diffuse.xyz *= mat.diffuse;
    specular.xyz *= mat.specular;
    
    if (fresnel.useFresnel)
    { 
        specular.xyz *= SchlickFresnel(fresnel, input.normalWorld, toEye);
    }
    
    diffuse *= Tex.Sample(linearWarpSS, input.texcoord); 
    
    return diffuse + specular;
    
    //return Tex.Sample(linearWarpSS, input.texcoord); 
    //return float4(color, 1.0); 
    //return float4(color, 1.0) * Tex.Sample(linearWarpSS, input.texcoord);
}