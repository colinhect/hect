#version 440

uniform sampler2D diffuse_texture;
uniform sampler2D material_texture;
uniform sampler2D normal_texture;

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texture_coords;

bool render_stage(
    out vec4    out_diffuse,
    out float   out_roughness,
    out float   out_metallic,
    out vec3    out_position,
    out vec3    out_normal)
{
    vec3 diffuse_sample = texture(diffuse_texture, vertex_texture_coords).rgb;
    out_diffuse = vec4(diffuse_sample, 1.0);

    vec2 material_sample = texture(material_texture, vertex_texture_coords).rg;
    out_roughness = material_sample.r;
    out_metallic = material_sample.g;

    out_position = vertex_position;

    vec3 normal_sample = texture(normal_texture, vertex_texture_coords).xyz * 2.0 - 1.0;
    normal_sample.y = -normal_sample.y;

    vec3 tangent = normalize(vertex_tangent);
    vec3 bitangent = normalize(cross(normalize(vertex_normal), tangent));
    out_normal = mat3(tangent, bitangent, normalize(vertex_normal)) * normalize(normal_sample);

    return true;
}