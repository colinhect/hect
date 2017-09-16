#version 440

uniform sampler2D diffuse_buffer;
uniform sampler2D back_buffer;

in vec2 vertex_texture_coords;

out vec3 output_color;

// Composite the light accumulation with unlit pixels
void main()
{
    vec4 diffuse_sample = texture(diffuse_buffer, vertex_texture_coords);

    // If the pixel is lit
    if (diffuse_sample.a > 0.0)
    {
        // Output the light accumulation
        output_color = texture(back_buffer, vertex_texture_coords).rgb;
    }
    else
    {
        // Output the diffuse color
        output_color = diffuse_sample.rgb;
    }
}