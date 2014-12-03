#version 410

out vec3 outputColor;

// Light accumulation stage output parameters
struct StageOutput
{
    vec3    color;
};

// Light accumulation stage output
void stage(
    out StageOutput output);

void main()
{
    StageOutput output = StageOutput(vec3(0.0));
    stage(output);

    outputColor = output.color;
}