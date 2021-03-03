#version 330 core

out vec4 fs_color;

in vec3 fs_position;
in vec3 vs_normal;
in vec2 vs_tex;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct Sun {
    vec3 direction;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Flashlight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

uniform vec3 view_position;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform Material material;
uniform Sun sun;
uniform Flashlight flashlight;
uniform PointLight pointLights[NR_POINT_LIGHTS]; 

vec3 CalcSunLight(Sun light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcFlashLight(Flashlight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(vs_normal);
    vec3 viewDir = normalize(view_position - fs_position);

    // phase 1: Directional lighting
    vec3 result = CalcSunLight(sun, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fs_position, viewDir);
    // phase 3: Spot light
    result += CalcFlashLight(flashlight, norm, fs_position, viewDir);

    fs_color = vec4(result, 1.0);
}

vec3 CalcSunLight(Sun light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, vs_tex));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, vs_tex));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, vs_tex));

    return (ambient + diffuse + specular) * light.color;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, vs_tex));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, vs_tex));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, vs_tex));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
} 

vec3 CalcFlashLight(Flashlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, vs_tex));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, vs_tex));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, vs_tex));
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}
