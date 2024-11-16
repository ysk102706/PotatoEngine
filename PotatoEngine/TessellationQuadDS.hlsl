#include "Shader.hlsli" 

struct PatchOutput
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
}; 

struct TessellationQuadDSInput
{
    float3 pos : POSITION; 
}; 

struct TessellationQuadPSInput 
{
    float4 pos : SV_POSITION; 
}; 

[domain("quad")] 
TessellationQuadPSInput main(PatchOutput patch, float2 uv : SV_DomainLocation, const OutputPatch<TessellationQuadDSInput, 4> quad)
{
    TessellationQuadPSInput output; 
    
    float3 a = lerp(quad[0].pos, quad[1].pos, uv.x); 
    float3 b = lerp(quad[2].pos, quad[3].pos, uv.x); 
    float3 c = lerp(a, b, uv.y); 
    
    output.pos = mul(mul(float4(c, 1.0), view), proj); 
    
    return output; 
}