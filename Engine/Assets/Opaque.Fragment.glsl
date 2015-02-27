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
    diffuseBuffer = texture(diffuseTexture, vertexTextureCoords);
    materialBuffer = texture(materialTexture, vertexTextureCoords).rgb;
    
    vec3 normalSample = 2.0 * texture(normalTexture, vertexTextureCoords).xyz - vec3(1.0, 1.0, 1.0);

    vec3 normal = normalize(vertexNormal);
    vec3 tangent = normalize(vertexTangent);
    diffuseBuffer = vec4(tangent * 10.0, 1.0);

    //tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = normalize(cross(tangent, normal));
    mat3 tbn = transpose(mat3(tangent, bitangent, normal));
    normalSample *= tbn;

    normalBuffer = vec4(normalize(normal), gl_FragCoord.z);
    positionBuffer = vertexWorldPosition;
}