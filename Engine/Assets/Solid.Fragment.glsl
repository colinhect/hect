#version 410

uniform vec3 diffuse;
uniform float roughness;
uniform float metallic;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
out vec4 outputNormal;

void main()
{
    outputDiffuse = vec4(diffuse, 1.0);
    outputMaterial = vec3(roughness, metallic, 0.0);
    outputNormal = vec4(normalize(vertexWorldNormal), gl_FragCoord.z);
    outputPosition = vertexWorldPosition;
    gl_FragDepth = gl_FragCoord.z;
}