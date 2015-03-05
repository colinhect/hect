#version 440

in vec3 position;
in vec2 textureCoords;

out vec2 vertexTextureCoords;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertexTextureCoords = textureCoords;
}