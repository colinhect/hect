#version 440

uniform sampler2D particleTexture;

in vec2 vertexTextureCoords;

out vec4 color;

void main()
{
    color = texture(particleTexture, vertexTextureCoords) * vec4(vec3(1.0), 0.5);
}