struct PSInput
{
    float4 Position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    return float4(1, 1, 1, 1);
}