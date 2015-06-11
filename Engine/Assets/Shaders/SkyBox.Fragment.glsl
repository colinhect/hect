#version 440

uniform samplerCube skyBoxTexture;

in vec3 vertexPosition;

bool renderStage(
    out vec3    color)
{
    color = texture(skyBoxTexture, -vertexPosition).rgb;
    return true;
}
