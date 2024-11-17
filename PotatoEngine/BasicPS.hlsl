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
    bool useAmbient;
    float mipLevel; 
    float2 dummy; 
}; 

float4 main(PSInput input) : SV_TARGET
{ 
    if (useAmbient)
    {
        return float4(mat.ambient, 1.0); 
    }
    
    float3 toEye = normalize(eyePos - input.posWorld);
    float3 toLight = normalize(light.pos - input.posWorld); 
    float3 halfway = normalize(toEye + toLight);
    
    float NdotV = saturate(dot(input.normalWorld, toEye)); 
    float VdotH = saturate(dot(toEye, halfway)); 
    
    //float3 color = float3(0.0, 0.0, 0.0);
    
    //color += DirectionalLight(light, toEye, input.normalWorld, mat); 
    
    float4 diffuse = diffuseTex.Sample(linearWarpSS, input.normalWorld); 
    float4 specular = specularTex.Sample(linearWarpSS, reflect(-toEye, input.normalWorld));
    
    specular *= pow((specular.x + specular.y + specular.z) / 3.0, mat.shininess);
    
    diffuse.xyz *= mat.diffuse;
    specular.xyz *= mat.specular; 
    
    if (fresnel.useFresnel)
    { 
        specular.xyz *= SchlickFresnel_UnrealEngine4(fresnel.fresnelR0, VdotH);
    }
    
    diffuse *= Tex.SampleLevel(linearWarpSS, input.texcoord, mipLevel); 
    
    return diffuse + specular; 
    
    //return Tex.Sample(linearWarpSS, input.texcoord); 
    //return float4(color, 1.0); 
    //return float4(color, 1.0) * Tex.Sample(linearWarpSS, input.texcoord);
}