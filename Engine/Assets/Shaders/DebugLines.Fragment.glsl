#version 440

in vec4 vertex_color;

bool render_stage(
    out vec4    out_color)
{
    out_color = vertex_color;
    return true;
}