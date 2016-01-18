#version 440

in vec4 vertexColor;

bool renderStage(
    out vec4    outColor)
{
    outColor = vertexColor;
    return true;
}