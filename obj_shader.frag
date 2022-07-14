#version 440

in vec2 tex_tex;

uniform sampler2D tex;

void main()
{
    gl_FragColor = texture2D(tex, tex_tex);
}

