#version 410

#ifdef VERTEX

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 modelViewProjection;

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

#endif

#ifdef FRAGMENT

uniform vec3 diffuse;
uniform float roughness;
uniform float metallic;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

void writeGeometry(
    in  vec3    diffuse,
    in  float   roughness,
    in  float   metallic,
    in  vec3    worldNormal,
    in  vec3    worldPosition,
    in  float   depth);

void main()
{
    writeGeometry(diffuse, roughness, metallic, vertexWorldNormal, vertexWorldPosition, gl_FragCoord.z);
}

#endif