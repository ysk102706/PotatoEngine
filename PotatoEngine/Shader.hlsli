SamplerState linearWarpSS : register(s0);
SamplerState linearClampSS : register(s1);

TextureCube envTex : register(t10); 
TextureCube diffuseTex : register(t11); 
TextureCube specularTex : register(t12); 

#define MAXLIGHT 1 

struct PhongShading
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

struct RimEffect
{
    int useRim;
    float3 color;
    float strength;
    float factor;
    int useSmoothStep;
    float dummy;
};

struct FresnelEffect
{ 
    int useFresnel; 
    float3 fresnelR0;
}; 

struct TextureSetting
{
    int useAmbient;
    int useTextureLOD; 
    float mipLevel; 
    int useNormalMap; 
    int invertNormalMapY; 
    int useAO; 
    float2 dummy; 
};

cbuffer globalConstantData : register(b1)
{
    matrix view;
    matrix proj;
    float3 eyePos;
    float envStrength; 
    Light light;
}; 

cbuffer PostProcessConstantData : register(b2)
{
    float2 pixelDelta;
    float bloomStrength; 
    float threshold; 
};

struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL; 
    float2 texcoord : TEXCOORD; 
    float3 tangent : TANGENT; 
}; 

struct Pos4VSInput
{
    float4 pos : POSITION; 
};

struct PSInput
{
    float4 pos : SV_POSITION; 
    float3 posWorld : POSITION0;
    float3 normalWorld : POSITION1;
    float2 texcoord : TEXCOORD; 
    float3 tangent : TANGENT; 
};