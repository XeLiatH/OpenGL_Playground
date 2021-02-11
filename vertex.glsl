#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tex;
//layout (location = 2) in vec2 v_tex;

//out vec3 vs_color;

out vec3 fs_position;
out vec3 vs_normal;
out vec2 vs_tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(v_position, 1.f);
    // vs_color = v_color;
    fs_position = vec3(model * vec4(v_position, 1.f));
    vs_normal = mat3(transpose(inverse(model))) * v_normal;
    vs_tex = v_tex;
}
