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

uniform float exposure;
uniform float oneOverGamma;

uniform samplerCube skyBoxTexture;

in vec3 vertexPosition;

out vec3 outputColor;

vec3 correctGamma(
    in  vec3    color,
    in  float   oneOverGamma);

vec3 expose(
    in  vec3    color,
    in  float   exposure);

void main()
{
    vec3 color = texture(skyBoxTexture, -vertexPosition).rgb;

    color = expose(color, exposure);
    color = correctGamma(color, oneOverGamma);

    outputColor = color;
}

#endif