#version 410

uniform mat4 model;
uniform mat4 modelViewProjection;

in vec3 position;
in vec3 normal;

out vec3 vertexWorldPosition;
out vec3 vertexNormal;

void main()
{
	vertexWorldPosition = (model * vec4(position, 1.0)).xyz;
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexNormal = normalize((model * vec4(normal, 0.0)).xyz);
}