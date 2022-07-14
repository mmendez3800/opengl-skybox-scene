#version 130

in vec4 particle_color;
uniform mat4 cam_inv;
uniform mat4 proj;
uniform vec4 clip;

out vec4 part_color;

void main()
{
    vec4 worldPosition = gl_Vertex;
    gl_ClipDistance[0] = dot(clip, worldPosition);
    gl_Position = proj * cam_inv * worldPosition;
    part_color = particle_color;
}

