#version 410

out vec4 outputColor;

void writePhysicalLightAccumulation(
    in  vec3    accumulation,
    in  float   depth)
{
    outputColor = vec4(accumulation, depth);
}