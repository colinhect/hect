#version 330

uniform sampler2D diffuseTexture;
uniform vec2 textureScale;

in vec3 vertexPosition;
in vec3 vertexWorldPosition;
in vec3 vertexNormal;
in vec3 vertexWorldNormal;

out vec3 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
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

    vec3 colorX = texture(diffuseTexture, coordX).rgb;
    vec3 colorY = texture(diffuseTexture, coordY).rgb;
    vec3 colorZ = texture(diffuseTexture, coordZ).rgb;

    outputDiffuse = colorX * blendWeights.x + colorY * blendWeights.y + colorZ * blendWeights.z;

    float roughness = 1.0;  
    float metallic = 1.0;
    float specular = 0.0;
    outputMaterial = vec3(roughness, metallic, specular);

    outputPosition = vertexWorldPosition;

    float depth = gl_FragCoord.z;
    outputNormal = vec4(vertexWorldNormal, depth);
}