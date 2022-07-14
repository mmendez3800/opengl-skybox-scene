#version 400

in vec3 vertex;
uniform mat4 cam_inv;
uniform mat4 proj;
uniform vec4 clip;

void main()
{
    vec4 worldPosition = vec4(vertex, 1.0);
    gl_ClipDistance[0] = dot(clip, worldPosition);
    gl_Position = proj * cam_inv * worldPosition;
}

