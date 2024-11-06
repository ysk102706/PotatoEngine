#include "Shader.hlsli"

struct CubeMapPSInput
{
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
};

float4 main(CubeMapPSInput input) : SV_TARGET
{
    return specularTex.Sample(linearWarpSS, input.posWorld) * envStrength;
}