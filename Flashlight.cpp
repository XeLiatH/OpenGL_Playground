#include "Flashlight.h"

void Flashlight::Render(Shader& shader)
{
    shader.use();

    shader.setVec3("flashlight.position", this->position);
    shader.setVec3("flashlight.direction", this->direction);
    shader.setVec3("flashlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.setVec3("flashlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setVec3("flashlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    
    // special sort of stuff to make the flashlight round
    shader.setFloat("flashlight.constant", 1.0f);
    shader.setFloat("flashlight.linear", 0.09);
    shader.setFloat("flashlight.quadratic", 0.032);
    shader.setFloat("flashlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(15.0f)));
}
