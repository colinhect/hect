#version 440

in vec3 position;
in vec2 texture_coords;

out vec2 vertex_texture_coords;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertex_texture_coords = texture_coords;
}