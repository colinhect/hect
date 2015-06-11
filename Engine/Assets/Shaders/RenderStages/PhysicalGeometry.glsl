#version 440

out vec4 diffuseBuffer;
out vec3 materialBuffer;
out vec3 positionBuffer;
out vec4 normalBuffer;

bool renderStage(
    out vec4    diffuse,
    out float   roughness,
    out float   metallic,
    out vec3    position,
    out vec3    normal);

void main()
{
    vec4 diffuse;
    float roughness;
    float metallic;
    vec3 position;
    vec3 normal;

    if (renderStage(diffuse, roughness, metallic, position, normal))
    {
        diffuseBuffer = diffuse;
        materialBuffer = vec3(roughness, metallic, 1.0);
        positionBuffer = position;
        normalBuffer = vec4(normal, 0.0);
    }
    else
    {
        discard;
    }
}