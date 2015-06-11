#version 440

uniform sampler2D additiveTexture;
uniform float intensity;

in vec2 vertexTextureCoords;

bool renderStage(
    out vec4    color)
{
    color = texture(additiveTexture, vertexTextureCoords) * intensity;
    return true;
}