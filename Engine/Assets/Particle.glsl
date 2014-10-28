#version 330

#ifdef VERTEX

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

#endif

#ifdef FRAGMENT

uniform sampler2D colorMap;
uniform vec4 color;

in float vertexFinalIntensity;
in vec2 vertexTextureCoord;

out vec4 outputColor;

void main()
{
    if (vertexFinalIntensity <= 0.0)
    {
        discard;
    }
    else
    {
        outputColor = texture(colorMap, vertexTextureCoord) * color * vertexFinalIntensity;
    }
}

#endif

#ifdef GEOMETRY

uniform vec3 cameraPosition;
uniform vec3 cameraUp;
uniform mat4 viewProjection;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in float vertexRotation[];
in float vertexSize[];
in float vertexIntensity[];

out float vertexFinalIntensity;
out vec2 vertexTextureCoord;

mat4 rotationMatrix(vec3 axis, float angle);

void main()
{
    vec3 position = gl_in[0].gl_Position.xyz;
    float halfSize = vertexSize[0] * 0.5;
    float rotation = vertexRotation[0];
    vertexFinalIntensity = vertexIntensity[0];

    vec3 toCamera = normalize(cameraPosition - position);
    vec3 right = cross(toCamera, cameraUp);
    vec3 up = cameraUp;

    mat4 vertexRotation = rotationMatrix(toCamera, rotation);
    right = (vertexRotation* vec4(right, 1.0)).xyz;
    right *= halfSize;
    up = (vertexRotation* vec4(up, 1.0)).xyz;
    up *= halfSize;

    // Bottom left
    gl_Position = viewProjection * vec4(position - right - up, 1.0);
    vertexTextureCoord = vec2(1.0, 0.0);
    EmitVertex();

    // Top left
    gl_Position = viewProjection * vec4(position - right + up, 1.0);
    vertexTextureCoord = vec2(1.0, 1.0);
    EmitVertex();

    // Bottom right
    gl_Position = viewProjection * vec4(position + right - up, 1.0);
    vertexTextureCoord = vec2(0.0, 0.0);
    EmitVertex();

    // Top right
    gl_Position = viewProjection * vec4(position + right + up, 1.0);
    vertexTextureCoord = vec2(0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}

#endif