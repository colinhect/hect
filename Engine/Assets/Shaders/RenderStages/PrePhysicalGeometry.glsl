#version 440

out vec4 diffuse_buffer;
out vec3 material_buffer;
out vec3 position_buffer;
out vec4 normal_buffer;

bool render_stage(
    out vec3    color);

void main()
{
    vec3 color;

    if (render_stage(color))
    {
        diffuse_buffer = vec4(color, 0.0);
        material_buffer = vec3(0.0, 0.0, 0.0);
        position_buffer = vec3(0.0);
        normal_buffer = vec4(vec3(0.0), 0.0);
    }
    else
    {
        discard;
    }
}