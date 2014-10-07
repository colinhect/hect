#version 330

uniform vec3 cameraPosition;
uniform mat4 model;

uniform vec2 sizeRange;
uniform vec2 intensityRange;
uniform vec2 fadeRange;

layout(location = 0) in vec3 position;
layout(location = 1) in float rotation;
layout(location = 2) in float size;
layout(location = 3) in float intensity;

out float vertexRotation;
out float vertexSize;
out float vertexIntensity;

float fromRange(vec2 range, float value)
{
    return range.x + (range.y - range.x) * value;
}

void main()
{
    vec4 vertexPosition = model * vec4(position, 1.0);
    gl_Position = vertexPosition;

    vertexRotation = rotation * 6.28;
    vertexSize = fromRange(sizeRange, size);
    vertexIntensity = fromRange(intensityRange, size);

    float d = length(vertexPosition.xyz - cameraPosition);
    if (d < fadeRange.x)
    {
        vertexIntensity = 0.0;
    }
    else if (d >= fadeRange.x && d <= fadeRange.y)
    {
        vertexIntensity *= (d - fadeRange.x) / (fadeRange.y - fadeRange.x);
    }
}