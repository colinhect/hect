#version 410

#define PI 3.1415926535897932384626433832795

uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

bool sampleGeometryBuffer(
    out vec3    diffuse,
    out float   roughness,
    out float   metallic,
    out vec3    position,
    out vec3    normal,
    out float   depth);

void writeLightAccumulation(
    in  vec3    color);

void discardLightAccumulation();

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

    return lightColor * nDotL * (d * (1.0 - s) + s);
}

// Light accumulation stage output parameters
struct StageOutput
{
    vec3    color;
};

// Light accumulation stage output
void stage(
    out StageOutput output)
{
    vec3 diffuse;
    float roughness;
    float metallic;
    vec3 position;
    vec3 normal;
    float depth;

    // If this pixel is physically lit
    if (sampleGeometryBuffer(diffuse, roughness, metallic, position, normal, depth))
    {
        // Compute real diffuse/specular colors
        vec3 realDiffuse = diffuse - diffuse * metallic;
        vec3 realSpecular = mix(vec3(0.03), diffuse, metallic);

        // Compute the view direction
        vec3 viewDirection = normalize(cameraPosition - position);

        // Compute and output the total light accumulation
        output.color = computeLight(realDiffuse, realSpecular, normal, roughness, -lightDirection, viewDirection);
    }
    else
    {
        discard;
    }
}