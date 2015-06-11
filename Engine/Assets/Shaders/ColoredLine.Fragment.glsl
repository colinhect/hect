#version 440

in vec4 vertexColor;

bool renderStage(
    out vec4    color)
{
    color = vertexColor;
    return true;
}