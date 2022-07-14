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
    float y_down = max(0, tex_pos.y - y_unit);
    float y_up = min(1, tex_pos.y + y_unit);
    float y_farup = min(1, y_up + y_unit);
    float y_fardown = max(0, y_down - y_unit);

    vec3 sample_farup = texture2D(base_tex, vec2(tex_pos.x, y_farup)).rgb / total_weight;
    vec3 sample_up = texture2D(base_tex, vec2(tex_pos.x, y_up)).rgb / total_weight;
    vec3 sample_mid = texture2D(base_tex, tex_pos).rgb / total_weight;
    vec3 sample_down = texture2D(base_tex, vec2(tex_pos.x, y_down)).rgb / total_weight;
    vec3 sample_fardown = texture2D(base_tex, vec2(tex_pos.x, y_fardown)).rgb / total_weight;

    gl_FragColor = vec4(sample_fardown * 6
        + sample_down * 15
        + sample_mid * 20
        + sample_up * 15
        + sample_farup *6
        , 1);
    //gl_FragColor = vec4(1, 1, 1, 1);
}

