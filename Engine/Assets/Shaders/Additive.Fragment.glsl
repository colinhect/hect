#version 440

uniform sampler2D additiveTexture;

in vec2 vertexTextureCoords;

out vec4 outputColor;

void main()
{
    outputColor = texture(additiveTexture, vertexTextureCoords);
}