#version 440

out vec4 color_buffer;

bool render_stage(
    out vec4    color);

void main()
{
    vec4 color;

    if (render_stage(color))
    {
        color_buffer = color;
    }
    else
    {
        discard;
    }
}