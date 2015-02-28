#version 410

uniform sampler2D diffuseTexture;
uniform sampler2D materialTexture;
uniform sampler2D normalTexture;

in vec3 vertexWorldPosition;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec2 vertexTextureCoords;

out vec4 diffuseBuffer;
out vec3 materialBuffer;
out vec3 positionBuffer;
out vec4 normalBuffer;

void main()
{
    diffuseBuffer = vec4(texture(diffuseTexture, vertexTextureCoords).rgb, 1.0);
    materialBuffer = texture(materialTexture, vertexTextureCoords).rgb;

    vec3 normalSample = texture(normalTexture, vertexTextureCoords).xyz * 2.0 - 1.0;
    normalSample.y = -normalSample.y;

    vec3 normal = normalize(vertexNormal);
    vec3 tangent = normalize(vertexTangent);
    vec3 bitangent = normalize(cross(normal, tangent));
    normalSample = mat3(tangent, bitangent, normal) * normalize(normalSample);

    normalBuffer = vec4(normalize(normalSample), gl_FragCoord.z);
    positionBuffer = vertexWorldPosition;
}