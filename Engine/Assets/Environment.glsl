#version 410

uniform vec3 cameraPosition;
uniform samplerCube lightProbeTexture;

bool sampleGeometryBuffer(
    out vec3    diffuse,
    out float   roughness,
    out float   metallic,
    out vec3    position,
    out vec3    normal,
    out float   depth);

void writePhysicalLightAccumulation(
    in  vec3    accumulation,
    in  float   depth);

vec3 computeRoughFresnel(
    in  vec3    specularColor,
    in  float   a,
    in  vec3    h,
    in  vec3    v)
{
    // Schlick
    return specularColor
        + (max(vec3(1.0 - a), specularColor) - specularColor)
        * pow((1 - clamp(dot(v, h), 0.0, 1.0)), 5.0);
}

void main()
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

        // Compute the mip map level based on the roughness
        float mipMapLevel = roughness * 8.0 + 4.0;
   
        // Sample the reflectance from the light probe
        vec3 reflectDirection = normalize(reflect(viewDirection, normal));
        vec3 reflectance = textureLod(lightProbeTexture, reflectDirection, mipMapLevel).rgb;

        // Sample the ambience from the light probe
        vec3 ambience = textureLod(lightProbeTexture, normal, 32.0).rgb * 0.5;

        // Compute the fresnel attenuated by the roughness
        vec3 fresnel = computeRoughFresnel(realSpecular, roughness * roughness, normal, viewDirection);

        // Write the total light accumulation for the environment
        vec3 light = fresnel * reflectance + realDiffuse * ambience;
        writePhysicalLightAccumulation(light, depth);
    }
    else
    {
        discard;
    }
}