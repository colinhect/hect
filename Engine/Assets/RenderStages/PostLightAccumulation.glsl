#version 410

out vec4 outputColor;

void writePostLightAccumulation(
    in  vec3    color,
    in  float   depth)
{
    outputColor = vec4(color, depth);
    gl_FragDepth = depth;
}