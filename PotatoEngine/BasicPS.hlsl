#include "Shader.hlsli"
#include "PhongShading.hlsli" 
#include "RimEffect.hlsli" 
#include "FresnelEffect.hlsli"

Texture2D Tex : register(t0); 
Texture2D normalMap : register(t1); 

cbuffer MaterialConstantData : register(b0) 
{
    PhongShading mat;
    RimEffect rim; 
    FresnelEffect fresnel; 
    TextureSetting textureSetting; 
}; 

float3 GetNormal(PSInput input)
{ 
    float3 normalWorld = input.normalWorld; 
    
    if (textureSetting.useNormalMap)
    {
        float3 normal = normalMap.Sample(linearWarpSS, input.texcoord).rgb; 
        normal = 2.0 * normal - 1.0; 
        
        normal.y = textureSetting.invertNormalMapY ? -normal.y : normal.y;
        
        float3 n = normalWorld; 
        float3 t = normalize(input.tangent - dot(input.tangent, n) * n);
        float3 b = cross(n, t); 
        
        float3x3 tbn = float3x3(t, b, n); 
        normalWorld = normalize(mul(normal, tbn)); 
    } 
    return normalWorld; 
}

float4 main(PSInput input) : SV_TARGET
{ 
    
    if (textureSetting.useAmbient)
    {
        return float4(mat.ambient, 1.0); 
    } 
    
    float3 normal = GetNormal(input); 
    
    float3 toEye = normalize(eyePos - input.posWorld); 
    float3 toLight = normalize(light.pos - input.posWorld); 
    float3 halfway = normalize(toEye + toLight);
    
    float NdotV = saturate(dot(normal, toEye)); 
    float VdotH = saturate(dot(toEye, halfway)); 
    
    float3 color = float3(0.0, 0.0, 0.0); 
    
    color += DirectionalLight(light, toEye, normal, mat); 
    
    //float4 diffuse = diffuseTex.Sample(linearWarpSS, normal) + float4(color, 1.0); 
    float4 diffuse = float4(color, 1.0); 
    float4 specular = specularTex.Sample(linearWarpSS, reflect(-toEye, normal));
    
    specular *= pow((specular.x + specular.y + specular.z) / 3.0, mat.shininess);
    
    diffuse.xyz *= mat.diffuse; 
    specular.xyz *= mat.specular; 
    
    if (fresnel.useFresnel)
    { 
        specular.xyz *= SchlickFresnel_UnrealEngine4(fresnel.fresnelR0, VdotH);
    }
    
    if (textureSetting.useTextureLOD) 
    {
        diffuse *= Tex.SampleLevel(linearWarpSS, input.texcoord, 10 * saturate((length(eyePos - input.posWorld) - 3) / 10.0));
    }
    else 
    {
        diffuse *= Tex.SampleLevel(linearWarpSS, input.texcoord, textureSetting.mipLevel);
    } 
    
    return diffuse + specular; 
    
    //return Tex.Sample(linearWarpSS, input.texcoord); 
    //return float4(color, 1.0); 
    //return float4(color, 1.0) * Tex.Sample(linearWarpSS, input.texcoord);
}