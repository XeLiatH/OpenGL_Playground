#version 330 core

out vec4 fs_color;

// in vec3 vs_color;
in vec2 vs_tex;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    fs_color = mix(texture(texture1, vs_tex), texture(texture2, vs_tex), 0.2f); // * vec4(vs_color, 1.f);
}
