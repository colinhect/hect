#version 440

uniform sampler2D additive_texture;
uniform float intensity;

in vec2 vertex_texture_coords;

bool render_stage(
    out vec4    out_color)
{
    out_color = texture(additive_texture, vertex_texture_coords) * intensity;
    return true;
}