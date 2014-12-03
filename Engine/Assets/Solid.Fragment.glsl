#version 410

uniform vec3 diffuse;
uniform float roughness;
uniform float metallic;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 diffuseBuffer;
out vec3 materialBuffer;
out vec3 positionBuffer;
out vec4 normalBuffer;

void main()
{
    diffuseBuffer = vec4(diffuse, 1.0);
    materialBuffer = vec3(roughness, metallic, 0.0);
    normalBuffer = vec4(normalize(vertexWorldNormal), gl_FragCoord.z);
    positionBuffer = vertexWorldPosition;
}