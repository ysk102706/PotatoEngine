#include "Shader.hlsli" 

cbuffer BillboardConstantData : register(b0)
{ 
    float width; 
    float3 dummy; 
}

struct BillboardGSInput
{
    float4 pos : SV_POSITION;
}; 

struct BillboardPSInput
{
    float4 pos : SV_POSITION; 
    float4 color : COLOR; 
}; 

[maxvertexcount(5)] 
void main(point BillboardGSInput input[1], uint primID : SV_PrimitiveID, inout TriangleStream<BillboardPSInput> outStream) 
{
    BillboardPSInput output; 
    
    float4 center = input[0].pos;
    float d = width / 2.0;  
    
    float3 forward = normalize(eyePos - center.xyz); 
    float3 up = float3(0.0, 1.0, 0.0); 
    float3 right = normalize(cross(up, forward));
    
    output.pos = mul(mul(center + float4(-right * d + up * d, 0.0), view), proj);
    output.color = float4(1.0, primID % 2, primID % 2, 1.0); 
    
    outStream.Append(output); 
    
    output.pos = mul(mul(center + float4(-right * d - up * d, 0.0), view), proj);
    output.color = float4(1.0, primID % 2, primID % 2, 1.0);
    
    outStream.Append(output);
    
    output.pos = mul(mul(center + float4(right * d + up * d, 0.0), view), proj);
    output.color = float4(1.0, primID % 2, primID % 2, 1.0);
    
    outStream.Append(output);
    
    output.pos = mul(mul(center + float4(right * d - up * d, 0.0), view), proj);
    output.color = float4(1.0, primID % 2, primID % 2, 1.0);
    
    outStream.Append(output);
    
    outStream.RestartStrip();

}