#version 440

uniform sampler2D texture;

in vec2 vertexTextureCoords;

out vec4 outputColor;

void main()
{
    outputColor = texture2D(texture, vertexTextureCoords);
}