#version 440

in vec2 tex_pos;

layout (location = 10) uniform sampler2D base_img;
layout (location = 11) uniform sampler2D bloom_img;

void main()
{
    vec3 base_color = texture2D(base_img, tex_pos).rgb;
    vec3 bloom_color = texture2D(bloom_img, tex_pos).rgb / 1;
    float r = min(1, base_color.r + bloom_color.r);
    float g = min(1, base_color.g + bloom_color.g);
    float b = min(1, base_color.b + bloom_color.b);
    gl_FragColor = vec4(r, g, b, 1);
}

