#version 330

uniform samplerCube skyBoxCubeMap;

in vec3 vertexPosition;

out vec3 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
out vec4 outputNormal;

void main()
{
    vec3 color = texture(skyBoxCubeMap, -vertexPosition).rgb;
    outputDiffuse = vec3(color);
    outputMaterial = vec3(0.0);
    outputPosition = vec3(0.0);
    outputNormal = vec4(0.0);
}