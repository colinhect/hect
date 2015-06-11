#version 440

uniform sampler2D additiveTexture;

in vec2 vertexTextureCoords;

bool renderStage(
    out vec4    color)
{
    color = texture(additiveTexture, vertexTextureCoords);
    return true;
}