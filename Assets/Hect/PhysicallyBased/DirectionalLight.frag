#version 330

#define PI 3.1415926535897932384626433832795

uniform vec3 color;
uniform vec3 direction;
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

vec3 computeDiffuse(vec3 diffuse)
{
    return diffuse / PI;
}

float computeSpecular_G(float a, float NdV, float NdL, float NdH, float VdH)
{
    float k = a * 0.5f;
    float GV = NdV / (NdV * (1 - k) + k);
    float GL = NdL / (NdL * (1 - k) + k);

    return GV * GL;
}

float computeSpecular_D(float a, float NdH)
{
    float a2 = a*a;
    float NdH2 = NdH * NdH;

    float denominator = NdH2 * (a2 - 1.0) + 1.0;
    denominator *= denominator;
    denominator *= PI;

    return a2 / denominator;
}

vec3 computeSpecular_F(vec3 specularColor, vec3 h, vec3 v)
{
    return (specularColor + (1.0f - specularColor) * pow((1.0f - clamp(dot(v, h), 0.0, 1.0)), 5));
}

vec3 computeSpecular_F_Rough(vec3 specularColor, float a, vec3 h, vec3 v)
{
    return (specularColor + (max(vec3(1.0 - a), specularColor) - specularColor) * pow((1 - clamp(dot(v, h), 0.0, 1.0)), 5));
}

vec3 computeSpecular(vec3 specularColor, vec3 h, vec3 v, vec3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV)
{
    return ((computeSpecular_D(a, NdH) * computeSpecular_G(a, NdV, NdL, NdH, VdH)) * computeSpecular_F(specularColor, h, v) ) / (4.0 * NdL * NdV + 0.0001);
}

vec3 computeLight(vec3 diffuse, vec3 specular, vec3 normal, float roughness, vec3 lightDir, vec3 viewDir)
{
    float NdL = clamp(dot(normal, lightDir), 0.0, 1.0);
    float NdV = clamp(dot(normal, viewDir), 0.0, 1.0);
    vec3 h = normalize(lightDir + viewDir);
    float NdH = clamp(dot(normal, h), 0.0, 1.0);
    float VdH = clamp(dot(viewDir, h), 0.0, 1.0);
    float LdV = clamp(dot(lightDir, viewDir), 0.0, 1.0);
    float a = max(0.001, roughness * roughness);

    vec3 d = computeDiffuse(diffuse);
    vec3 s = clamp(computeSpecular(specular, h, viewDir, lightDir, a, NdL, NdV, NdH, VdH, LdV), 0.0, 1.0);

    return color * NdL * (d * (1.0 - s) + s);
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

        vec3 realDiffuse = diffuse - diffuse * metallic;
        vec3 realSpecular = mix(vec3(0.03), diffuse, metallic);

        vec3 normal = normalize(normalMatrix * normalSample.xyz);
        vec3 lightDir = -normalize(normalMatrix * direction);
        vec3 viewDir = normalize(normalMatrix * normalize(cameraPosition - positionSample.xyz));

        vec3 light1 = computeLight(realDiffuse, realSpecular, normal, roughness, lightDir, viewDir);
        vec3 envFresnel = computeSpecular_F_Rough(realSpecular, roughness * roughness, normal, viewDir);

        vec3 reflectVector = normalize(reflect(normalize(cameraPosition - positionSample.xyz), normalSample.xyz));

        float mipIndex =  roughness * roughness * 8.0f;
        vec3 environmentColor = textureLod(environmentMap, reflectVector, mipIndex).rgb;


        float reflectivity = 1.0;
        outputColor = vec4(light1 + envFresnel * environmentColor * reflectivity, depth);
    }
    else
    {
        discard;
    }
}