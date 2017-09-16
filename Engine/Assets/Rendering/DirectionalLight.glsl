#version 440

#define PI 3.1415926535897932384626433832795

uniform mat4 model_view;
uniform vec3 light_direction;
uniform vec4 light_color;
uniform sampler2D diffuse_buffer;
uniform sampler2D material_buffer;
uniform sampler2D position_buffer;
uniform sampler2D normal_buffer;

in vec2 vertex_texture_coords;

out vec3 back_buffer;

float compute_geometry(
    in  float   a,
    in  float   n_dot_v,
    in  float   n_dot_l,
    in  float   n_dot_h,
    in  float   v_dot_h)
{
    // Smith schlick-GGX
    float k = ((a + 1) * (a + 1)) / 8.0;
    float gv = n_dot_v / (n_dot_v * (1.0 - k) + k);
    float gl = n_dot_l / (n_dot_l * (1.0 - k) + k);

    return gv * gl;
}

float compute_normal_distribution(
    in  float   a,
    in  float   n_dot_h)
{
    // GGX
    float a2 = a * a;
    float d = (n_dot_h * n_dot_h) * (a2 - 1.0) + 1.0;
    d *= d;
    d *= PI;

    return a2 / d;
}

vec3 compute_fresnel(
    in  vec3    specular_color,
    in  float   v_dot_h)
{
    // Schlick
    return specular_color
        + (1.0 - specular_color)
        * pow((1.0 - clamp(v_dot_h, 0.0, 1.0)), 5.0);
}

vec3 compute_specular(
    in  vec3    specular_color,
    in  vec3    h,
    in  vec3    v,
    in  vec3    l,
    in  float   a,
    in  float   n_dot_l,
    in  float   n_dot_v,
    in  float   n_dot_h,
    in  float   v_dot_h,
    in  float   l_dot_v)
{
    float normal_distribution = compute_normal_distribution(a, n_dot_h);
    float geometry = compute_geometry(a, n_dot_v, n_dot_l, n_dot_h, v_dot_h);
    vec3 fresnel = compute_fresnel(specular_color, v_dot_h);
    return (fresnel * normal_distribution * geometry) / (4.0 * n_dot_l * n_dot_v + 0.0001);
}

vec3 compute_light(
    in  vec3    diffuse,
    in  vec3    specular,
    in  vec3    n,
    in  float   r,
    in  vec3    l,
    in  vec3    v)
{
    float n_dot_l = clamp(dot(n, l), 0.0, 1.0);
    float n_dot_v = clamp(dot(n, v), 0.0, 1.0);
    vec3 h = normalize(l + v);
    float n_dot_h = clamp(dot(n, h), 0.0, 1.0);
    float v_dot_h = clamp(dot(v, h), 0.0, 1.0);
    float l_dot_v = clamp(dot(l, v), 0.0, 1.0);
    float a = max(0.001, r * r);

    vec3 d = diffuse / PI;
    vec3 s = clamp(compute_specular(specular, h, v, l, a, n_dot_l, n_dot_v, n_dot_h, v_dot_h, l_dot_v), 0.0, 1.0);

    return light_color.rgb * n_dot_l * (d * (1.0 - s) + s);
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
        vec3 view_light_direction = normalize((model_view * vec4(light_direction, 0.0)).xyz);

        // Compute and output the total light accumulation
        back_buffer = compute_light(real_diffuse, real_specular, normal, roughness, -view_light_direction, view_direction);
    }
    else
    {
        discard;
    }
}