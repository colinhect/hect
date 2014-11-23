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

void writePhysicalGeometry(
    in vec3 diffuse,
    in float roughness,
    in float metallic,
    in vec3 worldNormal,
    in vec3 worldPosition,
    in float depth);

void main()
{
    vec3 color = texture(skyBoxTexture, -vertexPosition).rgb;
    writePhysicalGeometry(color, 0.0, 0.0, vec3(0.0), vec3(0.0), 0.0);
}

#endif