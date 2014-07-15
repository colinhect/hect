#version 330

uniform vec3 cameraPosition;
uniform mat4 view;
uniform sampler2D diffuseBuffer;
uniform sampler2D materialBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform samplerCube environmentMap;

mat3 normalMatrix = mat3(
    view[0][0], view[0][1], view[0][2],
    view[1][0], view[1][1], view[1][2],
    view[2][0], view[2][1], view[2][2]
);

in vec2 vertexTextureCoords;

out vec4 outputColor;

vec3 fresnel(vec3 specularColor, float a, vec3 h, vec3 v)
{
    return (specularColor + (max(vec3(1.0 - a), specularColor) - specularColor) * pow((1 - clamp(dot(v, h), 0.0, 1.0)), 5.0));
}

void main()
{
    vec4 normalSample = texture(normalBuffer, vertexTextureCoords);
    float depth = normalSample.w;
    if (depth > 0.0)
    {
        vec4 diffuseSample = texture(diffuseBuffer, vertexTextureCoords);
        vec4 materialSample = texture(materialBuffer, vertexTextureCoords);
        vec4 positionSample = texture(positionBuffer, vertexTextureCoords);

        vec3 diffuse = diffuseSample.rgb;

        float roughness = materialSample.r;
        float metallic = materialSample.g;

        vec3 realSpecular = mix(vec3(0.03), diffuse, metallic);

        vec3 n = normalize(normalMatrix * normalSample.xyz);
        vec3 v = normalize(normalMatrix * normalize(cameraPosition - positionSample.xyz));

        vec3 reflectVector = normalize(reflect(normalize(cameraPosition - positionSample.xyz), normalSample.xyz));

        float mipIndex =  roughness * roughness * 16.0f;

        vec3 environmentColor = textureLod(environmentMap, reflectVector, mipIndex).rgb;
        vec3 f = fresnel(realSpecular, roughness * roughness, n, v);

        outputColor = vec4(f * environmentColor, depth);
    }
    else
    {
        discard;
    }
}