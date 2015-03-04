#version 410

uniform sampler2D particleTexture;

in vec2 vertexTextureCoords;

out vec4 diffuseBuffer;

void main()
{
    diffuseBuffer = texture(particleTexture, vertexTextureCoords);
}