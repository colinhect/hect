#version 410

uniform sampler2D diffuseBuffer;
uniform sampler2D materialBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;

in vec2 vertexTextureCoords;

bool sampleGeometryBuffer(
    out vec3    diffuse,
    out float   lighting,
    out float   roughness,
    out float   metallic,
    out vec3    position,
    out vec3    normal,
    out float   depth)
{
    vec4 diffuseSample = texture(diffuseBuffer, vertexTextureCoords);
    lighting = diffuseSample.a;
    if (lighting > 0.0)
    {
        vec4 normalSample = texture(normalBuffer, vertexTextureCoords);
        normal = normalSample.xyz;
        depth = normalSample.w;

        diffuse = texture(diffuseBuffer, vertexTextureCoords).rgb;
        position = texture(positionBuffer, vertexTextureCoords).rgb;

        vec4 materialSample = texture(materialBuffer, vertexTextureCoords);
        roughness = materialSample.r;
        metallic = materialSample.g;

        return true;
    }

    return false;
}