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

mat3 normalMatrix = mat3(
    view[0][0], view[0][1], view[0][2],
    view[1][0], view[1][1], view[1][2],
    view[2][0], view[2][1], view[2][2]
);

in vec2 vertexTextureCoords;

out vec4 outputColor;

float geometry(float a, float nDotV, float nDotL, float nDotH, float vDotH)
{
    float k = a * 0.5;
    float gv = nDotV / (nDotV * (1.0 - k) + k);
    float gl = nDotL / (nDotL * (1.0 - k) + k);

    return gv * gl;
}

float normalDistribution(float a, float nDotH)
{
    float a2 = a * a;
    float d = (nDotH * nDotH) * (a2 - 1.0) + 1.0;
    d *= d;
    d *= PI;

    return a2 / d;
}

vec3 fresnel(vec3 specularColor, float vDotH)
{
    return (specularColor + (1.0 - specularColor) * pow((1.0 - clamp(vDotH, 0.0, 1.0)), 5.0));
}

vec3 computeSpecular(vec3 specularColor, vec3 h, vec3 v, vec3 l, float a, float nDotL, float nDotV, float nDotH, float vDotH, float lDotV)
{
    return ((normalDistribution(a, nDotH) * geometry(a, nDotV, nDotL, nDotH, vDotH)) * fresnel(specularColor, vDotH) ) / (4.0 * nDotL * nDotV + 0.0001);
}

vec3 computeLight(vec3 diffuse, vec3 specular, vec3 n, float r, vec3 l, vec3 v)
{
    float nDotL = clamp(dot(n, l), 0.0, 1.0);
    float nDotV = clamp(dot(n, v), 0.0, 1.0);
    vec3 h = normalize(l + v);
    float nDotH = clamp(dot(n, h), 0.0, 1.0);
    float vDotH = clamp(dot(v, h), 0.0, 1.0);
    float lDotV = clamp(dot(l, v), 0.0, 1.0);
    float a = max(0.001, r * r);

    vec3 d = diffuse / PI;
    vec3 s = clamp(computeSpecular(specular, h, v, l, a, nDotL, nDotV, nDotH, vDotH, lDotV), 0.0, 1.0);

    return color * nDotL * (d * (1.0 - s) + s);
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

        vec3 n = normalize(normalMatrix * normalSample.xyz);
        vec3 l = -normalize(normalMatrix * direction);
        vec3 v = normalize(normalMatrix * normalize(cameraPosition - positionSample.xyz));

        vec3 light = computeLight(realDiffuse, realSpecular, n, roughness, l, v);

        outputColor = vec4(light, depth);
    }
    else
    {
        discard;
    }
}