#version 330 core

out vec4 fs_color;

in vec3 vs_color;
in vec2 vs_tex;

void main()
{
    fs_color = vec4(vs_color, 1.f);
}
