#version 440

uniform mat4 model_view;
uniform mat4 model_view_projection;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texture_coords;

out vec3 vertex_position;
out vec3 vertex_normal;
out vec3 vertex_tangent;
out vec2 vertex_texture_coords;

void main()
{
    gl_Position = model_view_projection * vec4(position, 1.0);

    vertex_position = (model_view * vec4(position, 1.0)).xyz;
    vertex_normal = normalize((model_view * vec4(normal, 0.0)).xyz);
    vertex_tangent = normalize((model_view * vec4(tangent, 0.0)).xyz);
    vertex_texture_coords = texture_coords;
}