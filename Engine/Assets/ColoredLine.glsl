#version 330

#ifdef VERTEX

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 modelViewProjection;
uniform vec3 colorOverride;

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

#endif

#ifdef FRAGMENT

in vec3 vertexColor;

out vec4 outputColor;

void main()
{
    outputColor = vec4(vertexColor, 1.0);
}

#endif