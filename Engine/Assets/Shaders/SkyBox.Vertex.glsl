#version 440

uniform mat4 model_view_projection;

layout(location = 0) in vec3 position;

out vec3 vertex_position;

void main()
{
    gl_Position = model_view_projection * vec4(position, 1.0);
    vertex_position = position;
}