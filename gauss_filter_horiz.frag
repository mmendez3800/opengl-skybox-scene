#version 440

layout(location = 2) uniform sampler2D base_tex;

in vec2 tex_pos;

// 1/width
uniform float x_unit;
// 1/height
uniform float y_unit;

const float total_weight = 62;

void main()
{
    float x_left = max(0, tex_pos.x - x_unit);
    float x_right = min(1, tex_pos.x + x_unit);
    float x_farleft = max(0, x_left - x_unit);
    float x_farright = min(1, x_right + x_unit);

    vec3 sample_farleft = texture2D(base_tex, vec2(x_farleft, tex_pos.y)).rgb / total_weight;
    vec3 sample_left = texture2D(base_tex, vec2(x_left, tex_pos.y)).rgb / total_weight;
    vec3 sample_mid = texture2D(base_tex, tex_pos).rgb / total_weight;
    vec3 sample_right = texture2D(base_tex, vec2(x_right, tex_pos.y)).rgb / total_weight;
    vec3 sample_farright = texture2D(base_tex, vec2(x_farright, tex_pos.y)).rgb / total_weight;

    gl_FragColor = vec4(sample_farleft * 6
        + sample_left * 15
        + sample_mid * 20
        + sample_right * 15
        + sample_farright * 6
        , 1);
    //gl_FragColor = vec4(1, 1, 1, 1);
}

