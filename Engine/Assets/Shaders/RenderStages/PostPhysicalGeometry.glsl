#version 440

out vec4 colorBuffer;

bool renderStage(
    out vec4    color);

void main()
{
    vec4 color;

    if (renderStage(color))
    {
        colorBuffer = color;
    }
    else
    {
        discard;
    }
}