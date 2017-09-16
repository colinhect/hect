#version 440

uniform mat4 model_view_projection;

layout(location = 0) in vec3 position;
layout(location = 3) in vec2 texture_coords;

out vec2 vertex_texture_coords;

void main()
{
    gl_Position = model_view_projection * vec4(position, 1.0);
    vertex_texture_coords = texture_coords;
}