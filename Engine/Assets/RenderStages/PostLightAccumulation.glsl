#version 410

out vec4 outputColor;

void writePostLightAccumulation(
    in  vec3    color)
{
    outputColor = vec4(color, 1.0);
}