#version 440

out vec4 diffuseBuffer;
out vec3 materialBuffer;
out vec3 positionBuffer;
out vec4 normalBuffer;

bool renderStage(
    out vec3    color);

void main()
{
    vec3 color;

    if (renderStage(color))
    {
        diffuseBuffer = vec4(color, 0.0);
        materialBuffer = vec3(0.0, 0.0, 0.0);
        positionBuffer = vec3(0.0);
        normalBuffer = vec4(vec3(0.0), 0.0);
    }
    else
    {
        discard;
    }
}