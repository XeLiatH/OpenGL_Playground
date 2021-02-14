/*

  +-------------------------------------------------+
  |                                                 |
  | BONE BREAKAAAR                                  |
  | ==============                                  |
  |                                                 |
  | * Author:       Matěj Beran<matej.beran@tul.cz> |
  | * Date:         02/2021                         |
  | * GL Version:   3.3                             |
  |                                                 |
  | SPECIFICATION                                   |
  | =============                                   |
  | [ ] ground                                      |
  | [ ] with borders                                |
  | [ ] player spawns inside                        |
  | [ ] player has to dodge falling blocks          |
  | [ ] light in each corner                        |
  | [ ] red sun                                     |
  | [ ] space sky box                               |
  | [ ] collision                                   |
  | [ ] transparent blocks (ice looking)            |
  | [ ] swapping textures at runtime                |
  | [ ] rotating cubes changing color over time     |
  | [ ] speed increased with time                   |
  +-------------------------------------------------+

*/

#define GAME_NAME "** Super Awesome Game **"

#include <iostream>

#include <opencv2\opencv.hpp>

#include <GL/glew.h> 
#include <GL/wglew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Camera.h";
#include "Model.h";

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 400.f;
float lastY = 300.f;

struct Light
{
    glm::vec3 position = glm::vec3(1.2f, 1.0f, 2.0f);
} light;

Camera* camera; 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void error_callback(int error, const char* description);

void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int width = 1366; // mode->width;
    int height = 768; // mode->height;

    camera = new Camera(glm::vec3(0.f, 0.f, 10.f));

    lastX = width / 2;
    lastY = height / 2;

    GLFWwindow* pWindow = glfwCreateWindow(width, height, GAME_NAME, NULL, NULL);
    if (!pWindow)
    {
        std::cout << "Error making a window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(pWindow);
    glViewport(0, 0, width, height);

    glewInit();
    wglewInit();

    // switch to full screen
    //glfwSetWindowMonitor(pWindow, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
    //glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(pWindow, mouse_callback);
    glfwSetScrollCallback(pWindow, scroll_callback);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera->getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

    Shader shader("vertex.glsl", "fragment.glsl");

    shader.setMat4("model", model);
    shader.setMat4("view", view);

    Model _model("models/backpack/backpack.obj");

    while (!glfwWindowShouldClose(pWindow))
    {
        float current_frame = glfwGetTime();
        deltaTime = current_frame - lastFrame;
        lastFrame = current_frame;

        // Input
        processInput(pWindow);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        view = camera->getViewMatrix();
        shader.setMat4("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
       
        shader.setMat4("model", model);
        _model.Draw(shader);

        // Checking events
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->procesZoom(yoffset);
}

void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
}
