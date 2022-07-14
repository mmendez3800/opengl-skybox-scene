#version 400

in vec3 vertex;
in vec2 tex_coord;
uniform mat4 cam_inv;
uniform mat4 proj;
uniform vec4 clip;
out vec2 texcoord;
out vec4 clip_space;
out vec4 worldPosition;

void main()
{
    worldPosition = vec4(vertex, 1.0);
    clip_space = proj * cam_inv * worldPosition;
    gl_Position = clip_space;
    gl_ClipDistance[0] = dot(clip, worldPosition);
    texcoord = (tex_coord * 2 - 1) * 4;
}

