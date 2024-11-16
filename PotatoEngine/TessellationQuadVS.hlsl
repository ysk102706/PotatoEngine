#include "Shader.hlsli" 

struct TessellationQuadHSInput
{
    float4 pos : POSITION; 
}; 

TessellationQuadHSInput main(Pos4VSInput input)
{
    TessellationQuadHSInput output; 
    output.pos = input.pos; 
    
    return output; 
}