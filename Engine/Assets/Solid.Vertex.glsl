#version 410

uniform mat4 model;
uniform mat4 modelViewProjection;

in vec3 position;
in vec3 normal;

out vec3 vertexWorldPosition;
out vec3 vertexWorldNormal;

mat3 normalMatrix = mat3(
    model[0][0], model[0][1], model[0][2],
    model[1][0], model[1][1], model[1][2],
    model[2][0], model[2][1], model[2][2]
);

void main()
{
	vertexWorldPosition = (model * vec4(position, 1.0)).xyz;
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexWorldNormal = normalize(normalMatrix * normal);
}