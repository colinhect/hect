#version 410

uniform float exposure;
uniform float oneOverGamma;

uniform sampler2D accumulationBuffer;

in vec2 vertexTextureCoords;

out vec3 outputColor;

vec3 correctGamma(
    in  vec3    color,
    in  float   oneOverGamma);

vec3 expose(
    in  vec3    color,
    in  float   exposure);

bool sampleAccumulationBuffer(
    out vec3    color)
{
    vec4 accumulationSample = texture(accumulationBuffer, vertexTextureCoords);
    color = accumulationSample.rgb;
    if (accumulationSample.a > 0.0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void main()
{
    vec3 color;
    if (sampleAccumulationBuffer(color))
    {
        color = expose(color, exposure);
        color = correctGamma(color, oneOverGamma);

        outputColor = color;
    }
    else
    {
        discard;
    }
}