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
    vec3  direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 view_position;

void main()
{
    vec3 light_direction = normalize(light.position - fs_position);
   
    // Attenuation
    float distance = length(light.position - fs_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // vec3 light_direction = normalize(-light.direction);

    // Ambient
    vec3 ambient = attenuation * light.ambient * vec3(texture(material.diffuse, vs_tex));

    // Diffuse
    vec3 normal = normalize(vs_normal);
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = attenuation * light.diffuse * diff * vec3(texture(material.diffuse, vs_tex));

    // Specular
    vec3 viewDir = normalize(view_position - fs_position);
    vec3 reflectDir = reflect(-light_direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = attenuation * light.specular * spec * vec3(texture(material.specular, vs_tex));

    // spotlight (soft edges)
    float theta = dot(light_direction, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // Final color
    fs_color = vec4(ambient + diffuse + specular, 1.f);
}
