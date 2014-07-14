#version 330

layout(location = 0) in vec3 position;

uniform mat4 modelViewProjection;

out vec3 vertexPosition;

void main()
{
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexPosition = position;
}