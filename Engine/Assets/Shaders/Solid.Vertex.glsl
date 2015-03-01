#version 410

uniform mat4 modelView;
uniform mat4 modelViewProjection;

in vec3 position;
in vec3 normal;

out vec3 vertexPosition;
out vec3 vertexNormal;

void main()
{
    gl_Position = modelViewProjection * vec4(position, 1.0);
    
    vertexPosition = (modelView * vec4(position, 1.0)).xyz;
    vertexNormal = normalize((modelView * vec4(normal, 0.0)).xyz);
}