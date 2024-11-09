#include "Shader.hlsli"

Texture2D Tex : register(t0);

struct PostProcessPSInput
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PostProcessPSInput input) : SV_TARGET
{
    float4 color = Tex.Sample(linearClampSS, input.texcoord); 
    float l = (color.x + color.y + color.z) / 3.0;
    
    if (l > threshold)
    {
        return color;
    }
    else
    {
        return float4(0.0, 0.0, 0.0, 0.0); 
    }
}