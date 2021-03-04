#include "Pointlight.h"

void Pointlight::Render(int i)
{
    this->shader.setVec3("pointLights[" + std::to_string(i) + "].position", this->position);
    this->shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", this->ambient);
    this->shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", this->diffuse);
    this->shader.setVec3("pointLights[" + std::to_string(i) + "].specular", this->specular);
    this->shader.setFloat("pointLights[" + std::to_string(i) + "].constant", this->constant);
    this->shader.setFloat("pointLights[" + std::to_string(i) + "].linear", this->linear);
    this->shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", this->quadratic);
}
