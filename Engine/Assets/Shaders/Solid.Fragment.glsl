#version 410

uniform vec4 diffuse;
uniform float roughness;
uniform float metallic;

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec4 diffuseBuffer;
out vec3 materialBuffer;
out vec3 positionBuffer;
out vec4 normalBuffer;

void main()
{
    diffuseBuffer = diffuse;
    materialBuffer = vec3(roughness, metallic, 0.0);
    normalBuffer = vec4(normalize(vertexNormal), 0.0);
    positionBuffer = vertexPosition;
}