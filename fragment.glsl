#version 330 core

out vec4 fs_color;

in vec3 fs_position;
in vec3 vs_normal;
in vec2 vs_tex;

struct Material {
    sampler2D   diffuse;
    sampler2D   specular;
    float       shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform vec3 view_position;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vs_tex));

    // Diffuse
    vec3 normal = normalize(vs_normal);
    vec3 light_direction = normalize(light.position - fs_position);
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vs_tex));

    // Specular
    vec3 viewDir = normalize(view_position - fs_position);
    vec3 reflectDir = reflect(-light_direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vs_tex));

    // Final color
    fs_color = vec4(ambient + diffuse + specular, 1.f);
}
