#include "Shader.hlsli"

Texture2D Tex : register(t0);

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    return Tex.Sample(linearWarpSS, input.texcoord);
}