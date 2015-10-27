#version 440

uniform sampler2D diffuseTexture;
uniform sampler2D materialTexture;
uniform sampler2D normalTexture;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec2 vertexTextureCoords;

bool renderStage(
    out vec4    outDiffuse,
    out float   outRoughness,
    out float   outMetallic,
    out vec3    outPosition,
    out vec3    outNormal)
{
    vec3 diffuseSample = texture(diffuseTexture, vertexTextureCoords).rgb;
    outDiffuse = vec4(diffuseSample, 1.0);

    vec2 materialSample = texture(materialTexture, vertexTextureCoords).rg;
    outRoughness = materialSample.r;
    outMetallic = materialSample.g;

    outPosition = vertexPosition;

    vec3 normalSample = texture(normalTexture, vertexTextureCoords).xyz * 2.0 - 1.0;
    normalSample.y = -normalSample.y;

    vec3 tangent = normalize(vertexTangent);
    vec3 bitangent = normalize(cross(normalize(vertexNormal), tangent));
    outNormal = mat3(tangent, bitangent, normalize(vertexNormal)) * normalize(normalSample);

    return true;
}