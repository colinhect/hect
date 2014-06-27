#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 vertexTextureCoords;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertexTextureCoords = textureCoords;
}