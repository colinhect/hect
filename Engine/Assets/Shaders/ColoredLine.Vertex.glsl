#version 440

uniform mat4 modelViewProjection;
uniform vec4 color;

layout(location = 0) in vec3 position;

out vec4 vertexColor;

void main()
{
    vertexColor = color;
    gl_Position = modelViewProjection * vec4(position, 1.0);
}