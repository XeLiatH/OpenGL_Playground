#version 330 core

out vec4 fs_color;

in vec3 fs_position;
in vec3 vs_normal;
in vec2 vs_tex;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 view_position;
uniform vec3 light_position;
uniform vec3 light_color;

void main()
{
    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * light_color;

    // Diffuse
    vec3 normal = normalize(vs_normal);
    vec3 light_direction = normalize(light_position - fs_position);
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    // Specular
    float specularStrength = 10.5f;
    vec3 viewDir = normalize(view_position - fs_position);
    vec3 reflectDir = reflect(-light_direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_color;

    // Final light
    //vec4 final_light = vec4(ambient + diffuse + specular, 1.f);

    // Final color
    fs_color = mix(texture(texture1, vs_tex), texture(texture2, vs_tex), 0.2f) * vec4(ambient + diffuse + specular, 1.f);
}
