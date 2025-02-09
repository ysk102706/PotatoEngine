// Schlick approximation: Eq. 9.17 in "Real-Time Rendering 4th Ed." 
// 9.5.2 Typical Fresnel Reflectance Values
// Water : (0.02, 0.02, 0.02)
// Glass : (0.08, 0.08, 0.08)
// Plastic : (0.05, 0.05, 0.05)
// Gold: (1.0, 0.71, 0.29)
// Silver: (0.95, 0.93, 0.88)
// Copper: (0.95, 0.64, 0.54) 

float3 SchlickFresnel_NdotV(float3 F0, float NdotV)
{ 
    return F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0); 
} 

// Real Shading in Unreal Engine 4 
float3 SchlickFresnel_UnrealEngine4(float3 F0, float VdotH) 
{
    return F0 + (1.0 - F0) * pow(2.0, (-5.55473 * VdotH - 6.98316) * VdotH); 
} 

// UnrealEngine 
// 엔진 내부 코드 
float3 SchlickFresnel_UnrealEngine(float3 F0, float VdotH)
{
    float Fc = pow(1 - VdotH, 5);
    //return Fc + (1 - Fc) * F0;
    
    return saturate(50.0 * F0.g) * Fc + (1 - Fc) * F0;
}

float3 CookTorranceFresnel(float3 SpecularColor, float VdotH)
{
    float3 SpecularColorSqrt = sqrt(clamp(SpecularColor, float3(0, 0, 0), float3(0.99, 0.99, 0.99)));
    float3 n = (1 + SpecularColorSqrt) / (1 - SpecularColorSqrt);
    float3 g = sqrt(n * n + VdotH * VdotH - 1); 
    return 0.5 * pow((g - VdotH) / (g + VdotH), 2) * (1 + pow(((g + VdotH) * VdotH - 1) / ((g - VdotH) * VdotH + 1), 2));
}