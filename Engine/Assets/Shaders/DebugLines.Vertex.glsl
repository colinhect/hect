#version 440

uniform mat4 modelViewProjection;
uniform float intensity;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 vertexColor;

void main()
{
    vertexColor = color * intensity;
    gl_Position = modelViewProjection * vec4(position, 1.0);
}