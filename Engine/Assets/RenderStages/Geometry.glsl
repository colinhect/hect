#version 410

out vec3 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
out vec4 outputNormal;

void writeGeometry(
    in  vec3    diffuse,
    in  float   roughness,
    in  float   metallic,
    in  vec3    worldNormal,
    in  vec3    worldPosition,
    in  float   depth)
{
    outputDiffuse = diffuse;
    outputMaterial = vec3(roughness, metallic, 0.0);
    outputNormal = vec4(normalize(worldNormal), depth);
    outputPosition = worldPosition;
    gl_FragDepth = depth;
}