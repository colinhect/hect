#version 410

uniform mat4 modelViewProjection;
uniform vec3 colorOverride;

in vec3 position;
in vec3 color;

out vec3 vertexColor;

void main()
{
    // Use the color override if there is one
    if (length(colorOverride) > 0.0001)
    {
        vertexColor = colorOverride;
    }

    // Otherwise just use the color of the vertex
    else
    {
        vertexColor = color;
    }
    
    gl_Position = modelViewProjection * vec4(position, 1.0);
}