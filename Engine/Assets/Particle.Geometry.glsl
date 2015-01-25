#version 440

const vec2 corners[4] =
{ 
    vec2(0.0, 1.0),
    vec2(0.0, 0.0),
    vec2(1.0, 1.0),
    vec2(1.0, 0.0)
};

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projection;
uniform float particleSize;

out vec2 vertexTextureCoords;

void main()
{
    for (int i = 0; i < 4; ++i)
    {
        vec4 eyePosition = gl_in[0].gl_Position;
        eyePosition.xy += particleSize * (corners[i] - vec2(0.5));
        gl_Position = projection * eyePosition;
        vertexTextureCoords = corners[i];
        EmitVertex();
    }

    EndPrimitive();
}
