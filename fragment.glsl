#version 330 core

out vec4 fs_color;

in vec3 vs_color;
in vec2 vs_tex;

uniform sampler2D fs_texture;

void main()
{
    fs_color = texture(fs_texture, vs_tex); // * vec4(vs_color, 1.f);
}
