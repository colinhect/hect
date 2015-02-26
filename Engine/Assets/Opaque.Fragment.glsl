#version 410

uniform sampler2D diffuseTexture;
uniform sampler2D materialTexture;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;
in vec3 vertexWorldTangent;
in vec2 vertexTextureCoords;

out vec4 diffuseBuffer;
out vec3 materialBuffer;
out vec3 positionBuffer;
out vec4 normalBuffer;

void main()
{
    diffuseBuffer = texture(diffuseTexture, vertexTextureCoords);
    materialBuffer = texture(materialTexture, vertexTextureCoords).rgb;
    normalBuffer = vec4(normalize(vertexWorldNormal), gl_FragCoord.z);
    positionBuffer = vertexWorldPosition;
}