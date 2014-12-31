#version 410

uniform mat4 modelViewProjection;
uniform vec3 color;

in vec3 position;

out vec3 vertexColor;

void main()
{
    vertexColor = color;
    gl_Position = modelViewProjection * vec4(position, 1.0);
}