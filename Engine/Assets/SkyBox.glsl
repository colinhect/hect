#version 410

#ifdef VERTEX

layout(location = 0) in vec3 position;

uniform mat4 modelViewProjection;

out vec3 vertexPosition;

void main()
{
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexPosition = position;
}

#endif

#ifdef FRAGMENT

uniform samplerCube skyBoxTexture;

in vec3 vertexPosition;

out vec3 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
out vec4 outputNormal;

void main()
{
    vec3 color = texture(skyBoxTexture, -vertexPosition).rgb;
    outputDiffuse = vec3(color);
    outputMaterial = vec3(0.0);
    outputPosition = vec3(0.0);
    outputNormal = vec4(0.0);
}

#endif