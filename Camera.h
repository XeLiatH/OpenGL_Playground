#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class CameraMovement { LEFT, RIGHT, FORWARD, BACKWARD };

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.f, 1.f, 3.f));
    ~Camera();

    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void procesZoom(float offset);

    float getFov() const;

    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;

    glm::mat4 getViewMatrix() const;

private:
    glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);

    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 right;

    // no rolling in here
    float yaw = -90.f;
    float pitch = 0.f;

    float fov = 45.f;
    float speed = 3.5f;
    float sensitivity = 0.25f;
    
    void Update();
};

#endif
