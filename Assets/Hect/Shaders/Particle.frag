#version 330

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