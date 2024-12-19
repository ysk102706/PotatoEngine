float3 Rim(RimEffect rim, float3 toEye, float3 normal)
{
    float r = (1.0 - dot(toEye, normal));
    
    if (rim.useSmoothStep)
    {
        r = smoothstep(0.0, 1.0, r);
    }
    
    return pow(abs(r), rim.factor) * rim.color * rim.strength; 
}