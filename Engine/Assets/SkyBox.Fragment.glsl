#version 410

uniform samplerCube skyBoxTexture;

in vec3 vertexPosition;

out vec4 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
out vec4 outputNormal;

void main()
{
    outputDiffuse = vec4(texture(skyBoxTexture, -vertexPosition).rgb, 0.0);
    outputMaterial = vec3(0.0, 0.0, 0.0);
    outputNormal = vec4(vec3(0.0), gl_FragCoord.z);
    outputPosition = vec3(0.0);
    gl_FragDepth = gl_FragCoord.z;
}