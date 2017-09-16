#version 440

out vec4 diffuse_buffer;
out vec3 material_buffer;
out vec3 position_buffer;
out vec4 normal_buffer;

bool render_stage(
    out vec4    diffuse,
    out float   roughness,
    out float   metallic,
    out vec3    position,
    out vec3    normal);

void main()
{
    vec4 diffuse;
    float roughness;
    float metallic;
    vec3 position;
    vec3 normal;

    if (render_stage(diffuse, roughness, metallic, position, normal))
    {
        diffuse_buffer = diffuse;
        material_buffer = vec3(roughness, metallic, 1.0);
        position_buffer = position;
        normal_buffer = vec4(normal, 0.0);
    }
    else
    {
        discard;
    }
}