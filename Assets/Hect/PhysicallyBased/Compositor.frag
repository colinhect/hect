#version 330

uniform float exposure;
uniform float oneOverGamma;

uniform sampler2D accumulationBuffer;

in vec2 vertexTextureCoords;

out vec4 outputColor;

vec3 correctGamma(vec3 color, float oneOverGamma)
{
    return pow(color, vec3(oneOverGamma));
}

vec3 expose(vec3 color, float exposure)
{
    return vec3(1.0) - exp2(-exposure * color);
}

void main()
{
	vec4 accumulationSample = texture(accumulationBuffer, vertexTextureCoords);
    vec3 color = accumulationSample.rgb;
    color = expose(color, exposure);
    color = correctGamma(color, oneOverGamma);
    outputColor = vec4(color, 1.0);
    gl_FragDepth = accumulationSample.a;
}