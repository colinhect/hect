#version 440

uniform sampler2D texture;

in vec2 vertexTextureCoords;

bool renderStage(
    out vec4    color)
{
    color = texture2D(texture, vertexTextureCoords);
    return true;
}