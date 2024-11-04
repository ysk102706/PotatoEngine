#include "Shader.hlsli" 

struct GSInput
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
};

GSInput main(VSInput input)
{
    GSInput output;
    output.pos = float4(input.pos, 1.0);
    output.normal = float4(input.normal, 0.0);
    
    return output; 
}