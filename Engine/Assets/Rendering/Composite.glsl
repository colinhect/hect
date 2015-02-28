#version 410

uniform sampler2D diffuseBuffer;
uniform sampler2D backBuffer;

in vec2 vertexTextureCoords;

out vec3 outputColor;

// Composite the light accumulation with unlit pixels
void main()
{
    vec4 diffuseSample = texture(diffuseBuffer, vertexTextureCoords);

    // If the pixel is lit
    if (diffuseSample.a > 0.0)
    {
        // Output the light accumulation
        outputColor = texture(backBuffer, vertexTextureCoords).rgb;
    }
    else
    {
        // Output the diffuse color
        outputColor = diffuseSample.rgb;
    }
}