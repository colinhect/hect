#version 440

uniform float exposure;
uniform float one_over_gamma;
uniform sampler2D back_buffer;

in vec2 vertex_texture_coords;

out vec4 output_color;

vec3 correct_gamma(
    in  vec3    color,
    in  float   one_over_gamma)
{
    return pow(color, vec3(one_over_gamma));
}

vec3 expose(
    in  vec3    color,
    in  float   exposure)
{
    return vec3(1.0) - exp2(-exposure * color);
}

void main()
{
    vec3 color = texture(back_buffer, vertex_texture_coords).rgb;

    if (exposure > 0.0)
    {
        // Color correct
        color = expose(color, exposure);
        color = correct_gamma(color, one_over_gamma);
    }
    
    output_color = vec4(color, 1.0);
}