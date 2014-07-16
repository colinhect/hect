#version 410

uniform vec3 diffuse;
uniform float roughness;
uniform float metallic;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec3 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
out vec4 outputNormal;

void main()
{
    outputDiffuse = diffuse;
    outputMaterial = vec3(roughness, metallic, 0.0);

    float depth = gl_FragCoord.z;
    outputNormal = vec4(normalize(vertexWorldNormal), depth);

    outputPosition = vertexWorldPosition;
}