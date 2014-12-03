#version 410

out vec4 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
out vec4 outputNormal;

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
    out StageOutput output);

void main()
{
    StageOutput output = StageOutput(vec3(0.0), 0.0, 0.0, 0.0, vec3(0.0), vec3(0.0), 0.0);
    stage(output);

    outputDiffuse = vec4(output.diffuse, output.lighting);
    outputMaterial = vec3(output.roughness, output.metallic, 0.0);
    outputNormal = vec4(normalize(output.normal), output.depth);
    outputPosition = output.position;
    gl_FragDepth = output.depth;
}