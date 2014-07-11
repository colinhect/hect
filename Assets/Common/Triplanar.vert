#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelViewProjection;

out vec3 vertexPosition;
out vec3 vertexNormal;

void main()
{
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexPosition = position;
    vertexNormal = normal;
}