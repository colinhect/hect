#version 440

uniform sampler2D diffuseTexture;
uniform sampler2D materialTexture;
uniform sampler2D normalTexture;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec2 vertexTextureCoords;

bool renderStage(
    out vec4    diffuse,
    out float   roughness,
    out float   metallic,
    out vec3    position,
    out vec3    normal)
{
    vec3 diffuseSample = texture(diffuseTexture, vertexTextureCoords).rgb;
    diffuse = vec4(diffuseSample, 1.0);

    vec2 materialSample = texture(materialTexture, vertexTextureCoords).rg;
    roughness = materialSample.r;
    metallic = materialSample.g;

    position = vertexPosition;

    vec3 normalSample = texture(normalTexture, vertexTextureCoords).xyz * 2.0 - 1.0;
    normalSample.y = -normalSample.y;

    vec3 tangent = normalize(vertexTangent);
    vec3 bitangent = normalize(cross(normalize(vertexNormal), tangent));
    normal = mat3(tangent, bitangent, normalize(vertexNormal)) * normalize(normalSample);

    return true;
}