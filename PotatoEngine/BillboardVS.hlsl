#include "Shader.hlsli"

struct BillboardGSInput
{
    float4 pos : SV_POSITION;
};

BillboardGSInput main(Pos4VSInput input)
{
    BillboardGSInput output; 
    output.pos = input.pos; 
    
    return output; 
}