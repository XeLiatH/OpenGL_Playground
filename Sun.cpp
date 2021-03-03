#include "Sun.h"

void Sun::Render(Shader& shader)
{
    shader.use();

    shader.setVec3("sun.direction", this->direction);
    shader.setVec3("sun.color", this->color);
    shader.setVec3("sun.ambient", this->ambient);
    shader.setVec3("sun.diffuse", this->diffuse);
    shader.setVec3("sun.specular", this->specular);
}
