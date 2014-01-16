#version 330

uniform sampler2D sourceTexture;

out vec4 outputColor;

in vec2 vertexTextureCoords;

vec4 srgb(vec4 color, float gamma)
{
    return vec4(pow(color.rgb, vec3(1.0 / gamma)), color.a);
}

void main() {
    float gamma = 2.2;
    //outputColor = srgb(texture(sourceTexture, vertexTextureCoords), gamma);
    outputColor = texture(sourceTexture, vertexTextureCoords);
}