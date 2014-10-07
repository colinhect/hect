#version 410

uniform samplerCube skyBoxTexture;

in vec3 vertexPosition;

out vec3 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
out vec4 outputNormal;

void main()
{
    vec3 color = texture(skyBoxTexture, -vertexPosition).rgb;
    outputDiffuse = vec3(color);
    outputMaterial = vec3(0.0);
    outputPosition = vec3(0.0);
    outputNormal = vec4(0.0);
}