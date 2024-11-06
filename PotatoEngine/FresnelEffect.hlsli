// Schlick approximation: Eq. 9.17 in "Real-Time Rendering 4th Ed."
// fresnelR0는 물질의 고유 성질
// Water : (0.02, 0.02, 0.02)
// Glass : (0.08, 0.08, 0.08)
// Plastic : (0.05, 0.05, 0.05)
// Gold: (1.0, 0.71, 0.29)
// Silver: (0.95, 0.93, 0.88)
// Copper: (0.95, 0.64, 0.54) 
float3 SchlickFresnel(FresnelEffect fresnel, float3 normal, float3 toEye)
{
    float f0 = 1.0 - saturate(dot(normal, toEye));
    
    return fresnel.fresnelR0 + (1.0 - fresnel.fresnelR0) * pow(f0, 5.0);
}