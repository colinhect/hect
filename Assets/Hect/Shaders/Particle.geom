#version 330

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