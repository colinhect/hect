#version 330

in vec3 vertexColor;

out vec4 outputColor;

void main()
{
    outputColor = vec4(vertexColor, 1.0);
}