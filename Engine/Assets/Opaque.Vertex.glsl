#version 410

uniform mat4 model;
uniform mat4 modelViewProjection;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 textureCoords;

out vec3 vertexWorldPosition;
out vec3 vertexNormal;
out vec3 vertexTangent;
out vec2 vertexTextureCoords;

void main()
{
	vertexWorldPosition = (model * vec4(position, 1.0)).xyz;
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexNormal = normalize((model * vec4(normal, 0.0)).xyz);
    vertexTangent = normalize((model * vec4(tangent, 0.0)).xyz);
    vertexTextureCoords = textureCoords;
}