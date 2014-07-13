#version 330

uniform sampler2D diffuseTexture;
uniform vec2 textureScale;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexWorldNormal;

out vec4 outputDiffuse;
out vec4 outputSpecular;
out vec4 outputNormal;

void main()
{
    vec3 blendWeights = abs(vertexNormal);
    blendWeights = (blendWeights - 0.2) * 7;
    blendWeights = max(blendWeights, 0);
    blendWeights /= blendWeights.x + blendWeights.y + blendWeights.z;

    vec2 coordX = vertexPosition.yz * textureScale;
    vec2 coordY = vertexPosition.zx * textureScale;
    vec2 coordZ = vertexPosition.xy * textureScale;

    vec4 colorX = texture(diffuseTexture, coordX);
    vec4 colorY = texture(diffuseTexture, coordY);
    vec4 colorZ = texture(diffuseTexture, coordZ);

    outputDiffuse = colorX * blendWeights.x + colorY * blendWeights.y + colorZ * blendWeights.z;
    outputSpecular = vec4(0.0);

    float depth = gl_FragCoord.z;
    outputNormal = vec4(vertexWorldNormal, depth);
}