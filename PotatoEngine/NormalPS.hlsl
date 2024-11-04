#include "Shader.hlsli" 

struct NormalPSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(NormalPSInput input) : SV_TARGET
{
    return input.color;
}