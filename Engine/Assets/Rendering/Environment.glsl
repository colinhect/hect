#version 440

uniform mat4 view;
uniform sampler2D diffuse_buffer;
uniform sampler2D material_buffer;
uniform sampler2D position_buffer;
uniform sampler2D normal_buffer;
uniform samplerCube light_probe_texture;

in vec2 vertex_texture_coords;

out vec3 back_buffer;

vec3 compute_rough_fresnel(
    in  vec3    specular_color,
    in  float   a,
    in  vec3    h,
    in  vec3    v)
{
    // Schlick
    return specular_color
        + (max(vec3(1.0 - a), specular_color) - specular_color)
        * pow((1 - clamp(dot(v, h), 0.0, 1.0)), 5.0);
}

void main()
{
    // If this pixel is physically lit
    vec4 diffuse_sample = texture(diffuse_buffer, vertex_texture_coords);
    if (diffuse_sample.a > 0.0)
    {
        vec3 diffuse = diffuse_sample.rgb;
        vec3 position = texture(position_buffer, vertex_texture_coords).rgb;
        vec3 normal = texture(normal_buffer, vertex_texture_coords).rgb;

        vec4 material_sample = texture(material_buffer, vertex_texture_coords);
        float roughness = material_sample.r;
        float metallic = material_sample.g;
        
        // Compute real diffuse/specular colors
        vec3 real_diffuse = diffuse - diffuse * metallic;
        vec3 real_specular = mix(vec3(0.03), diffuse, metallic);

        // Compute the view direction
        vec3 view_direction = normalize(-position);

        // Compute the mip map level based on the roughness
        float mip_map_level = roughness * 8.0 + 4.0;
   
        // Sample the reflectance from the light probe
        vec3 reflect_direction = normalize(reflect(view_direction, normal));
        reflect_direction = normalize((inverse(view) * vec4(reflect_direction, 0.0)).xyz);
        vec3 reflectance = textureLod(light_probe_texture, -reflect_direction, mip_map_level).rgb;

        // Sample the ambience from the light probe
        vec3 ambience = textureLod(light_probe_texture, -normal, 32.0).rgb * 0.5;

        // Compute the fresnel attenuated by the roughness
        vec3 fresnel = compute_rough_fresnel(real_specular, roughness * roughness, normal, view_direction);

        // Output the total light accumulation for the environment
        back_buffer = fresnel * reflectance + real_diffuse * ambience;
    }
    else
    {
        discard;
    }
}