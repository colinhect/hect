#version 330

uniform vec3 color;
uniform vec3 direction;
uniform mat4 view;
uniform sampler2D diffuseBuffer;
uniform sampler2D specularBuffer;
uniform sampler2D normalBuffer;

mat3 normalMatrix = mat3(
    view[0][0], view[0][1], view[0][2],
    view[1][0], view[1][1], view[1][2],
    view[2][0], view[2][1], view[2][2]
);

in vec2 vertexTextureCoords;

out vec3 outputColor;

void main()
{
    vec4 normalSample = texture(normalBuffer, vertexTextureCoords);
    float depth = normalSample.w;
    if (depth > 0.0)
    {
        vec3 diffuse = texture(diffuseBuffer, vertexTextureCoords).rgb;

        vec3 normal = normalSample.xyz;
        float lambert = max(dot(normal, -normalize(normalMatrix * direction)), 0.0);
        outputColor = diffuse * color * lambert;
    }
    else
    {
        discard;
    }
}