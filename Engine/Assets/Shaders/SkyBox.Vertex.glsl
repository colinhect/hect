#version 440

uniform mat4 modelViewProjection;

layout(location = 0) in vec3 position;

out vec3 vertexPosition;

void main()
{
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexPosition = position;
}