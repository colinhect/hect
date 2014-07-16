#version 410

uniform float exposure;
uniform float oneOverGamma;

uniform sampler2D diffuseBuffer;
uniform sampler2D accumulationBuffer;

in vec2 vertexTextureCoords;

out vec3 outputColor;

bool sampleAccumulationBuffer(
    out vec3    light,
    out float   depth)
{
    vec4 accumulationSample = texture(accumulationBuffer, vertexTextureCoords);
    
    depth = accumulationSample.a;
    if (depth > 0.0)
    {
        light = accumulationSample.rgb;
        return true;
    }
    else
    {
        return false;
    }
}

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
    vec3 color;
    float depth;

    // If this pixel is not physically lit
    if (!sampleAccumulationBuffer(color, depth))
    {
        color = texture(diffuseBuffer, vertexTextureCoords).rgb;
    }

    color = expose(color, exposure);
    color = correctGamma(color, oneOverGamma);

    outputColor = color;
    gl_FragDepth = depth;
}