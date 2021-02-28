#include "Camera.h"

Camera::Camera(glm::vec3 position)
{
    this->position = position;
    this->Update();
}

Camera::~Camera()
{

}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = this->speed * deltaTime;
    if (direction == CameraMovement::FORWARD)
        this->position += this->front * velocity;
    if (direction == CameraMovement::BACKWARD)
        this->position -= this->front * velocity;
    if (direction == CameraMovement::LEFT)
        this->yaw -= 50.f * velocity;
    if (direction == CameraMovement::RIGHT)
        this->yaw += 50.f * velocity;

    this->Update();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= this->sensitivity;
    yoffset *= this->sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->pitch > 89.0f) this->pitch = 89.0f;
        if (this->pitch < -89.0f) this->pitch = -89.0f;
    }

    // glm::vec4 position = glm::rotate()

    this->Update();
}

void Camera::procesZoom(float yoffset)
{
    this->fov -= (float)yoffset;
    
    if (this->fov < 1.0f) this->fov = 1.0f;
    if (this->fov > 45.0f) this->fov = 45.0f;
}

float Camera::getFov() const
{
    return this->fov;
}

glm::vec3 Camera::getPosition() const
{
    return this->position;
}

glm::vec3 Camera::getFront() const
{
    return this->front;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::Update()
{
    glm::vec3 front;

    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
    
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
