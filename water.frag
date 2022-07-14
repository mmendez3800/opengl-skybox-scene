#version 400

uniform sampler2D reflect;
uniform sampler2D refract;
uniform sampler2D dudv;
uniform float dudvOffset;

in vec2 texcoord;
in vec4 clip_space;
in vec4 worldPosition;
uniform vec3 cam_pos;

void main()
{
    vec3 cam_vec = normalize(cam_pos - worldPosition.xyz);
    vec2 distortedTexCoords =
        texture2D(dudv, vec2(texcoord.x + dudvOffset, texcoord.y)).rg * 0.2;
    distortedTexCoords =
        texcoord + vec2(distortedTexCoords.x, distortedTexCoords.y + dudvOffset);
    vec2 distort = (texture2D(dudv, distortedTexCoords).rg * 2.0 - 1) * 0.2;

    vec2 devspace = ((clip_space.xy / clip_space.w)/2.0 + 0.5) + distort;
    devspace.x = clamp(devspace.x, 0.001, 0.999);
    devspace.y = clamp(devspace.y, 0.001, 0.999);
    vec2 refspace = vec2(devspace.x, -devspace.y);
    vec4 refract_color = texture2D(refract, devspace);
    if (refract_color.r == 0 && refract_color.g == 0 && refract_color.b == 0)
    {
        refract_color = texture2D(refract, devspace - distort);
    }
    vec4 reflect_color = texture2D(reflect, refspace);
    if (reflect_color.r == 0 && reflect_color.g == 0 && reflect_color.b == 0)
    {
        reflect_color =
            texture2D(reflect,
                    vec2(devspace.x - distort.x, -devspace.y + distort.y));
    }
    gl_FragColor = mix(reflect_color, refract_color, dot(vec3(0, 1, 0),
                cam_vec));
}

