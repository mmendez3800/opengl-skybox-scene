#version 440

// This is intended to be run in coordinates [0,1]x[0,1]

layout(location=0) in vec2 position;

out vec2 tex_pos;

layout(location = 3) uniform mat4 orth;

// 1/width
layout(location = 0) uniform float x_unit;
// 1/height
layout(location = 1) uniform float y_unit;

void main()
{
    gl_Position = orth*vec4(position.xy, 0, 1);

    tex_pos = position.xy;
}

