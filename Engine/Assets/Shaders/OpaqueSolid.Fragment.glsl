#version 440

uniform vec4 diffuse;
uniform float roughness;
uniform float metallic;

in vec3 vertex_position;
in vec3 vertex_normal;

bool render_stage(
    out vec4    out_diffuse,
    out float   out_roughness,
    out float   out_metallic,
    out vec3    out_position,
    out vec3    out_normal)
{
    out_diffuse = diffuse;
    out_roughness = roughness;
    out_metallic = metallic;
    out_position = vertex_position;
    out_normal = vertex_normal;
    return true;
}
