struct BillboardVSInput
{ 
    float4 pos : POSITION; 
}; 

struct BillboardGSInput
{
    float4 pos : SV_POSITION;
};

BillboardGSInput main(BillboardVSInput input)
{
    BillboardGSInput output; 
    output.pos = input.pos; 
    
    return output; 
}