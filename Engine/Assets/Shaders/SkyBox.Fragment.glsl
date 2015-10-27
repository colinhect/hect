#version 440

uniform samplerCube skyBoxTexture;

in vec3 vertexPosition;

bool renderStage(
    out vec3    outColor)
{
    outColor = texture(skyBoxTexture, vertexPosition).rgb;
    return true;
}
