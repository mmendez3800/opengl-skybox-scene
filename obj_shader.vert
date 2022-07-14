#version 130

in vec3 vertex;
in vec2 tex_pos;

out vec2 tex_tex;

uniform mat4 cam_inv;
uniform mat4 transform;
uniform mat4 proj;
uniform vec4 clip;

void main()
{
    vec4 worldPosition = transform * vec4(gl_Vertex.xyz, 1.0);
    gl_ClipDistance[0] = dot(clip, worldPosition);
    gl_Position = proj * cam_inv * worldPosition;
    tex_tex = tex_pos;
}

