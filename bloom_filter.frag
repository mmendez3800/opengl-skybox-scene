#version 440

in vec2 tex_pos;

layout(location = 2) uniform sampler2D base_tex;

void main()
{
    vec3 color = texture2D(base_tex, tex_pos).rgb;
    if (length(color) > 1.5)
    {
        gl_FragColor = vec4(color, 1);
    }
    else
    {
        gl_FragColor = vec4(0, 0, 0, 1);
    }
}

