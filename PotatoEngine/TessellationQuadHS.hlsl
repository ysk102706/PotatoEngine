#include "Shader.hlsli" 

cbuffer TessellationQuadConstantData : register(b0) 
{
    float distance; 
    float3 dummy; 
};

struct TessellationQuadHSInput
{
    float4 pos : POSITION;
}; 

struct TessellationQuadDSInput
{
    float3 pos : POSITION; 
}; 

struct PatchOutput 
{
    float edges[4] : SV_TessFactor; 
    float inside[2] : SV_InsideTessFactor; 
}; 

PatchOutput PatchFunc(InputPatch<TessellationQuadHSInput, 4> patch, uint patchID : SV_PrimitiveID)
{
    PatchOutput output; 
    
    float3 a = lerp(patch[0].pos.xyz, patch[1].pos.xyz, 0.5);
    float3 b = lerp(patch[2].pos.xyz, patch[3].pos.xyz, 0.5); 
    float3 c = lerp(a, b, 0.5); 
    
    float dist = length(c - eyePos); 
    
    float tess = 64.0 * (1 - saturate((dist - 1) / distance)) + 1; 
    
    output.edges[0] = tess;
    output.edges[1] = tess;
    output.edges[2] = tess;
    output.edges[3] = tess;
    
    output.inside[0] = tess; 
    output.inside[1] = tess; 
    
    return output; 
} 

[domain("quad")] 
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchFunc")] 
[maxtessfactor(64.0f)]
TessellationQuadDSInput main(InputPatch<TessellationQuadHSInput, 4> p, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    TessellationQuadDSInput output; 
    output.pos = p[i].pos.xyz; 
    
    return output; 
}