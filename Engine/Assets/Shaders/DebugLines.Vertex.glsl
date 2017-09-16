#version 440

uniform mat4 model_view_projection;
uniform float intensity;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 vertex_color;

void main()
{
    vertex_color = color * intensity;
    gl_Position = model_view_projection * vec4(position, 1.0);
}