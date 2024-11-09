#include "Shader.hlsli"

struct PostProcessPSInput
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

PostProcessPSInput main(VSInput input)
{
    PostProcessPSInput output;
    output.pos = float4(input.pos, 1.0);
    output.texcoord = input.texcoord; 

    return output; 
}