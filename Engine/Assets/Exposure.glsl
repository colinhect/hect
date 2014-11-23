#version 410

vec3 correctGamma(
    in  vec3    color,
    in  float   oneOverGamma)
{
    return pow(color, vec3(oneOverGamma));
}

vec3 expose(
    in  vec3    color,
    in  float   exposure)
{
    return vec3(1.0) - exp2(-exposure * color);
}