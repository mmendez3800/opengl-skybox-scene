#version 400

in vec3 vertex;
in vec2 tex_coord;
uniform mat4 cam_inv;
uniform mat4 proj;
uniform vec4 clip;
out vec2 texcoord;
uniform mat4 rotation;

void main()
{
    vec4 worldPosition = rotation * vec4(vertex, 1.0);
    gl_Position = proj * cam_inv * worldPosition;
    gl_ClipDistance[0] = dot(clip, worldPosition);
    texcoord = tex_coord;
}

