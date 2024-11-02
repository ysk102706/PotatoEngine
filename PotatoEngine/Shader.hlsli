SamplerState linearWarpSS : register(s0);
SamplerState linearClampSS : register(s1);

cbuffer globalConstantData : register(b1)
{
    matrix view;
    matrix proj;
    float3 eyePos;
    float dummy;
    float3 lightPos;
    float diffuse;
    float specular;
    float3 dummy1;
};

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