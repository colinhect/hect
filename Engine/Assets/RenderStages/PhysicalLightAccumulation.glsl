#version 410

out vec3 outputColor;

void writePhysicalLightAccumulation(
    in  vec3    accumulation,
    in  float   depth)
{
    outputColor = accumulation;
    gl_FragDepth = depth;
}