#version 410

uniform samplerCube skyBoxCubeMap;

in vec3 vertexPosition;

out vec4 diffuseBuffer;
out vec3 materialBuffer;
out vec3 positionBuffer;
out vec4 normalBuffer;

void main()
{
    diffuseBuffer = vec4(texture(skyBoxCubeMap, -vertexPosition).rgb, 0.0);
    materialBuffer = vec3(0.0, 0.0, 0.0);
    normalBuffer = vec4(vec3(0.0), gl_FragCoord.z);
    positionBuffer = vec3(0.0);
}