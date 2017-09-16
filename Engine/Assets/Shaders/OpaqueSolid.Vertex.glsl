#version 440

uniform mat4 model_view;
uniform mat4 model_view_projection;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 vertex_position;
out vec3 vertex_normal;

void main()
{
    gl_Position = model_view_projection * vec4(position, 1.0);

    vertex_position = (model_view * vec4(position, 1.0)).xyz;
    vertex_normal = normalize((model_view * vec4(normal, 0.0)).xyz);
}
