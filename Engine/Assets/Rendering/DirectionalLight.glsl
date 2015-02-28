#version 410

#define PI 3.1415926535897932384626433832795

uniform vec3 lightDirection;
uniform vec4 lightColor;
uniform vec3 cameraPosition;
uniform sampler2D diffuseBuffer;
uniform sampler2D materialBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;

in vec2 vertexTextureCoords;

out vec3 backBuffer;

float computeGeometry(
    in  float   a,
    in  float   nDotV,
    in  float   nDotL,
    in  float   nDotH,
    in  float   vDotH)
{
    // Smith schlick-GGX
    float k = ((a + 1) * (a + 1)) / 8.0;
    float gv = nDotV / (nDotV * (1.0 - k) + k);
    float gl = nDotL / (nDotL * (1.0 - k) + k);

    return gv * gl;
}

float computeNormalDistribution(
    in  float   a,
    in  float   nDotH)
{
    // GGX
    float a2 = a * a;
    float d = (nDotH * nDotH) * (a2 - 1.0) + 1.0;
    d *= d;
    d *= PI;

    return a2 / d;
}

vec3 computeFresnel(
    in  vec3    specularColor,
    in  float   vDotH)
{
    // Schlick
    return specularColor
        + (1.0 - specularColor)
        * pow((1.0 - clamp(vDotH, 0.0, 1.0)), 5.0);
}

vec3 computeSpecular(
    in  vec3    specularColor,
    in  vec3    h,
    in  vec3    v,
    in  vec3    l,
    in  float   a,
    in  float   nDotL,
    in  float   nDotV,
    in  float   nDotH,
    in  float   vDotH,
    in  float   lDotV)
{
    float normalDistribution = computeNormalDistribution(a, nDotH);
    float geometry = computeGeometry(a, nDotV, nDotL, nDotH, vDotH);
    vec3 fresnel = computeFresnel(specularColor, vDotH);
    return (fresnel * normalDistribution * geometry) / (4.0 * nDotL * nDotV + 0.0001);
}

vec3 computeLight(
    in  vec3    diffuse,
    in  vec3    specular,
    in  vec3    n,
    in  float   r,
    in  vec3    l,
    in  vec3    v)
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

    return lightColor.rgb * nDotL * (d * (1.0 - s) + s);
}

void main()
{
    // If this pixel is physically lit
    vec4 diffuseSample = texture(diffuseBuffer, vertexTextureCoords);
    if (diffuseSample.a > 0.0)
    {
        vec3 diffuse = diffuseSample.rgb;
        vec3 position = texture(positionBuffer, vertexTextureCoords).rgb;
        vec3 normal = texture(normalBuffer, vertexTextureCoords).rgb;

        vec4 materialSample = texture(materialBuffer, vertexTextureCoords);
        float roughness = materialSample.r;
        float metallic = materialSample.g;

        // Compute real diffuse/specular colors
        vec3 realDiffuse = diffuse - diffuse * metallic;
        vec3 realSpecular = mix(vec3(0.03), diffuse, metallic);

        // Compute the view direction
        vec3 viewDirection = normalize(cameraPosition - position);

        // Compute and output the total light accumulation
        backBuffer = computeLight(realDiffuse, realSpecular, normal, roughness, -lightDirection, viewDirection);
    }
    else
    {
        discard;
    }
}