struct TessellationQuadPSInput
{ 
    float4 pos : SV_POSITION; 
};

float4 main(TessellationQuadPSInput input) : SV_TARGET
{
    return float4(1.0, 1.0, 1.0, 1.0); 
}