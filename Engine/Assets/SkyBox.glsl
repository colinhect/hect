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

void writePostLightAccumulation(
    in  vec3    color,
    in  float   depth);

void main()
{
    vec3 color = texture(skyBoxTexture, -vertexPosition).rgb;
    writePostLightAccumulation(color, 1.0);
}

#endif