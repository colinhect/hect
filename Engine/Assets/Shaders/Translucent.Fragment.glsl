#version 410

uniform sampler2D diffuseTexture;

in vec2 vertexTextureCoords;

out vec4 diffuseBuffer;

void main()
{
    diffuseBuffer = texture(diffuseTexture, vertexTextureCoords);
}