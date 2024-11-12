struct BillboardPSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(BillboardPSInput input) : SV_TARGET
{
    return input.color; 
}