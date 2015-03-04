#version 410

uniform mat4 modelViewProjection;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 textureCoords;

out vec2 vertexTextureCoords;

void main()
{
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexTextureCoords = textureCoords;
}