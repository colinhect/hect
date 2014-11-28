#version 410

#ifdef VERTEX

layout(location = 0) in vec3 position;

uniform mat4 modelViewProjection;

out vec3 vertexPosition;

void main()
{
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexPosition = position;
}

#endif

#ifdef FRAGMENT

uniform samplerCube skyBoxTexture;

in vec3 vertexPosition;

out vec3 outputColor;

// Opaque geometry stage output parameters
struct StageOutput
{
    vec3    diffuse;
    float   lighting;
    float   roughness;
    float   metallic;
    vec3    normal;
    vec3    position;
    float   depth;
};

// Opaque geometry stage output
void stage(
    out StageOutput output)
{
    output.diffuse = texture(skyBoxTexture, -vertexPosition).rgb;
    output.lighting = 0.0;
}

#endif