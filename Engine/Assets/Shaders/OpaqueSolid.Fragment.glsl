#version 440

uniform vec4 diffuse;
uniform float roughness;
uniform float metallic;

in vec3 vertexPosition;
in vec3 vertexNormal;

bool renderStage(
    out vec4    outDiffuse,
    out float   outRoughness,
    out float   outMetallic,
    out vec3    outPosition,
    out vec3    outNormal)
{
    outDiffuse = diffuse;
    outRoughness = roughness;
    outMetallic = metallic;
    outPosition = vertexPosition;
    outNormal = vertexNormal;
    return true;
}
