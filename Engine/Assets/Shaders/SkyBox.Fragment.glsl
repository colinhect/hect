#version 440

uniform samplerCube sky_box_texture;

in vec3 vertex_position;

bool render_stage(
    out vec3    out_color)
{
    out_color = texture(sky_box_texture, vertex_position).rgb;
    return true;
}
