#include "Shader.hlsli"

Texture2D Tex : register(t0);

struct PostProcessPSInput
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PostProcessPSInput input) : SV_TARGET
{ 
    float x = input.texcoord.x;
    float y = input.texcoord.y;
    
    float3 a = Tex.Sample(linearClampSS, float2(x - 2 * pixelDelta.x, y + 2 * pixelDelta.y));
    float3 b = Tex.Sample(linearClampSS, float2(x, y + 2 * pixelDelta.y));
    float3 c = Tex.Sample(linearClampSS, float2(x + 2 * pixelDelta.x, y + 2 * pixelDelta.y));
    
    float3 d = Tex.Sample(linearClampSS, float2(x - 2 * pixelDelta.x, y));
    float3 e = Tex.Sample(linearClampSS, float2(x, y));
    float3 f = Tex.Sample(linearClampSS, float2(x + 2 * pixelDelta.x, y));
    
    float3 g = Tex.Sample(linearClampSS, float2(x - 2 * pixelDelta.x, y - 2 * pixelDelta.y));
    float3 h = Tex.Sample(linearClampSS, float2(x, y - 2 * pixelDelta.y));
    float3 i = Tex.Sample(linearClampSS, float2(x + 2 * pixelDelta.x, y - 2 * pixelDelta.y));
    
    float3 j = Tex.Sample(linearClampSS, float2(x - pixelDelta.x, y + pixelDelta.y));
    float3 k = Tex.Sample(linearClampSS, float2(x + pixelDelta.x, y + pixelDelta.y));
    float3 l = Tex.Sample(linearClampSS, float2(x - pixelDelta.x, y - pixelDelta.y));
    float3 m = Tex.Sample(linearClampSS, float2(x + pixelDelta.x, y - pixelDelta.y));
                                
    float3 color = e * 0.125;   
    color += (a + c + g + i) * 0.03125; 
    color += (b + d + f + h) * 0.0625; 
    color += (j + k + l + m) * 0.125;
    
    return float4(color, 1.0);
}