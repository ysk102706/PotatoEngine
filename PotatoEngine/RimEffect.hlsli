float3 Rim(RimEffect rim, float3 toEye, float3 normal)
{
    float3 r = (1 - dot(toEye, normal));
    
    if (rim.useSmoothStep)
    {
        r = smoothstep(0.0, 1.0, r);
    }
    
    r = pow(abs(r), rim.factor) * rim.color * rim.strength;
    return r;
}