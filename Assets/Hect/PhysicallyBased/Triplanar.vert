#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 modelView;
uniform mat4 modelViewProjection;

out vec3 vertexPosition;
out vec3 vertexWorldPosition;
out vec3 vertexNormal;
out vec3 vertexWorldNormal;

mat3 normalMatrix = mat3(
    modelView[0][0], modelView[0][1], modelView[0][2],
    modelView[1][0], modelView[1][1], modelView[1][2],
    modelView[2][0], modelView[2][1], modelView[2][2]
);

void main()
{
	vertexWorldPosition = (model * vec4(position, 1.0)).xyz;
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexPosition = position;
    vertexNormal = normal;
    vertexWorldNormal = normalize(normalMatrix * normal);
}