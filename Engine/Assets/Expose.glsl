#version 410

uniform float exposure;
uniform float oneOverGamma;

uniform sampler2D backBuffer;

in vec2 vertexTextureCoords;

out vec3 outputColor;

vec3 correctGamma(
    in  vec3    color,
    in  float   oneOverGamma)
{
    return pow(color, vec3(oneOverGamma));
}

vec3 expose(
    in  vec3    color,
    in  float   exposure)
{
    return vec3(1.0) - exp2(-exposure * color);
}

void main()
{
    vec3 color = texture(backBuffer, vertexTextureCoords).rgb;

    // Color-correct and output the final pixel
    color = expose(color, exposure);
    color = correctGamma(color, oneOverGamma);
    outputColor = color;
}