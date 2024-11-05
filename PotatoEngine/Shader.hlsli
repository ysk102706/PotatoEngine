SamplerState linearWarpSS : register(s0);
SamplerState linearClampSS : register(s1);

#define MAXLIGHT 1 

struct Material
{
    float3 ambient;
    float shininess;
    float3 diffuse;
    float dummy1;
    float3 specular;
    float dummy2;
};

struct Light
{
    float3 pos; 
    float fallStart; 
    float3 dir; 
    float fallEnd;
    float3 strength; 
    float spotFactor; 
    float lightType;
    float3 dummy;
};

cbuffer globalConstantData : register(b1)
{
    matrix view;
    matrix proj;
    float3 eyePos;
    float dummy; 
    Light light;
}; 

float CalcFallOff(Light light, float d)
{
    return saturate(light.fallEnd - d / light.fallEnd - light.fallStart); 
}

float3 Phong(Light light, float3 toEye, float3 normal, Material mat)
{
    float diff = max(dot(-light.dir, normal), 0.0);
    float spec = pow(max(dot(toEye, reflect(light.dir, normal)), 0.0), mat.shininess);
    
    return mat.ambient + mat.diffuse * diff + mat.specular * spec;
}

float3 BlinnPhong(float3 lightStrength, float3 toEye, float3 toLight, float3 normal, Material mat)
{
    float3 h = normalize(toEye + toLight);
    float3 spec = mat.specular * pow(max(dot(normal, h), 0.0), mat.shininess * 2);
    
    return mat.ambient + (mat.diffuse + spec) * lightStrength;
} 

float3 DirectionalLight(Light light, float3 toEye, float3 normal, Material mat)
{ 
    float ndotl = max(dot(normal, -light.dir), 0.0);
    float3 lightStrength = light.strength * ndotl; 
    
    return BlinnPhong(lightStrength, toEye, -light.dir, normal, mat); 
}

float3 PointLight(Light light, float3 toEye, float3 pos, float3 normal, Material mat)
{
    float3 lightVec = light.pos - pos;
    float d = length(lightVec);
    
    if (d > light.fallEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        float ndotl = max(dot(normal, -light.dir), 0.0); 
        float3 lightStrength = light.strength * CalcFallOff(light, d) * ndotl; 
        
        return BlinnPhong(lightStrength, toEye, -light.dir, normal, mat); 
    }
} 

float3 SpotLight(Light light, float3 toEye, float3 pos, float3 normal, Material mat)
{
    float3 lightVec = light.pos - pos;
    float d = length(lightVec);
    
    if (d > light.fallEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        lightVec /= d;
        
        float ndotl = max(dot(normal, -light.dir), 0.0);
        float3 lightStrength = light.strength * CalcFallOff(light, d) * ndotl;
        
        float3 spotFactor = pow(max(dot(lightVec, -light.dir), 0.0), light.spotFactor);
        lightStrength *= spotFactor;
        
        return BlinnPhong(lightStrength, toEye, -light.dir, normal, mat);
    } 
} 

struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct PSInput
{
    float4 pos : SV_POSITION; 
    float3 posWorld : POSITION0;
    float3 normalWorld : POSITION1;
    float2 texcoord : TEXCOORD;
};