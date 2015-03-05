#version 440

uniform mat4 view;
uniform sampler2D diffuseBuffer;
uniform sampler2D materialBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform samplerCube lightProbeCubeMap;

in vec2 vertexTextureCoords;

out vec3 backBuffer;

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
        vec3 viewDirection = normalize(-position);

        // Compute the mip map level based on the roughness
        float mipMapLevel = roughness * 8.0 + 4.0;
   
        // Sample the reflectance from the light probe
        vec3 reflectDirection = normalize(reflect(viewDirection, normal));
        reflectDirection = normalize((inverse(view) * vec4(reflectDirection, 0.0)).xyz);
        vec3 reflectance = textureLod(lightProbeCubeMap, reflectDirection, mipMapLevel).rgb;

        // Sample the ambience from the light probe
        vec3 ambience = textureLod(lightProbeCubeMap, normal, 32.0).rgb * 0.5;

        // Compute the fresnel attenuated by the roughness
        vec3 fresnel = computeRoughFresnel(realSpecular, roughness * roughness, normal, viewDirection);

        // Output the total light accumulation for the environment
        backBuffer = fresnel * reflectance + realDiffuse * ambience;
    }
    else
    {
        discard;
    }
}