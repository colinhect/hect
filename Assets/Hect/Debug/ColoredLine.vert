#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 modelViewProjection;

out vec3 vertexColor;

void main()
{
    vertexColor = color;
    gl_Position = modelViewProjection * vec4(position, 1.0);
}