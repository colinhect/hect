#version 330

uniform sampler2D albedoTexture;
uniform vec2 textureScale;

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec4 outputColor;

void main()
{
    vec3 blendWeights = abs(vertexNormal);
    blendWeights = (blendWeights - 0.2) * 7;
    blendWeights = max(blendWeights, 0);
    blendWeights /= blendWeights.x + blendWeights.y + blendWeights.z;

    vec2 coordX = vertexPosition.yz * textureScale;
    vec2 coordY = vertexPosition.zx * textureScale;
    vec2 coordZ = vertexPosition.xy * textureScale;

    vec4 colorX = texture(albedoTexture, coordX);
    vec4 colorY = texture(albedoTexture, coordY);
    vec4 colorZ = texture(albedoTexture, coordZ);

    outputColor = colorX * blendWeights.x + colorY * blendWeights.y + colorZ * blendWeights.z;
}