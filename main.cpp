/*

  +-------------------------------------------------+
  |                                                 |
  | OPENGL GAME                                     |
  | ==============                                  |
  |                                                 |
  | * Author:       Matěj Beran<matej.beran@tul.cz> |
  | * Date:         03/2021                         |
  | * GL Version:   3.3                             |
  +-------------------------------------------------+

*/

#define GAME_NAME "** Super Awesome Game **"

#define SKYBOX "skybox_hot"

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
#include "Skybox.h";
#include "Sun.h";
#include "Flashlight.h";

float _deltaTime = 0.0f;
float _lastFrame = 0.0f;

bool _mouseLeftPressed = false;

float _lastX;
float _lastY;

struct Light
{
    glm::vec3 position = glm::vec3(1.2f, 1.0f, 2.0f);
} light;

GLFWwindow* _pWindow;
Camera* _pCamera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);

void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    const GLFWvidmode* pWindowMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int width = pWindowMode->width;
    int height = pWindowMode->height;

    _lastX = width / 2;
    _lastY = height / 2;

    _pCamera = new Camera(glm::vec3(0.f, 1.f, 10.f));

    _pWindow = glfwCreateWindow(width, height, GAME_NAME, NULL, NULL);
    if (!_pWindow)
    {
        std::cout << "Error making a window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(_pWindow);
    glViewport(0, 0, width, height);

    glewInit();
    wglewInit();

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(_pWindow, framebuffer_size_callback);
    glfwSetMouseButtonCallback(_pWindow, mouse_button_callback);
    glfwSetCursorPosCallback(_pWindow, mouse_callback);
    glfwSetScrollCallback(_pWindow, scroll_callback);
    glfwSetKeyCallback(_pWindow, keyboard_callback);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    Shader shader("vertex.glsl", "fragment.glsl");
    Shader skyboxShader("skybox_vertex.glsl", "skybox_fragment.glsl");

    Model backpack("models/backpack/backpack.obj", shader);
    Model floorTile("models/floor_tile/floor.obj", shader);
    Model gun("models/colt/colt.obj", shader);
    Model lamp("models/lamp/lampada.obj", shader);

    std::vector<glm::vec3> positions;
    std::vector<Model> floor;

    int a = 36;
    int step = 6;

    for (int i = -18; i <= a/2; i += step)
    {
        for (int j = -18; j <= a/2; j += step)
        {
            positions.push_back(glm::vec3((float)i, 0.f, (float)j));
            floor.push_back(floorTile);
        }
    }

    glm::vec3 pointLightPositions[] = {
        glm::vec3(6.f,  1.f,  6.f),
        glm::vec3(6.f, 1.f, -6.f),
        glm::vec3(-6.f,  1.f, 6.0f),
        glm::vec3(-6.0f,  1.f, -6.0f)
    };

    int pointLightSize = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);

    std::vector<Model> lamps;
    for (size_t i = 0; i < pointLightSize; i++)
    {
        lamps.push_back(lamp);
    }

    Skybox skybox(SKYBOX);
    Sun* sun = new Sun(); // nechapu, proc nefunguje Sun sun();
    Flashlight flashlight(_pCamera->getPosition(), _pCamera->getFront());

    while (!glfwWindowShouldClose(_pWindow))
    {
        float current_frame = glfwGetTime();
        _deltaTime = current_frame - _lastFrame;
        _lastFrame = current_frame;

        // Input
        processInput(_pWindow);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setFloat("material.shininess", 32.0f);
        shader.setVec3("view_position", _pCamera->getPosition());
        
        // Camera matrices
        view = _pCamera->getViewMatrix();
        shader.setMat4("view", view);
        projection = _pCamera->getProjectionMatrix((float)width, (float)height);
        shader.setMat4("projection", projection);
        
        // Handle Lighting
        flashlight.SetPosition(_pCamera->getPosition());
        flashlight.SetDirection(_pCamera->getFront());

        flashlight.Render(shader);
        sun->Render(shader);

        for (size_t i = 0; i < pointLightSize; i++)
        {
            shader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader.setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
            shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032);
            lamps.at(i).Translate(pointLightPositions[i] - glm::vec3(0.f, 1.f, 0.f));
            lamps.at(i).Scale(glm::vec3(0.03f, 0.03f, 0.03f));
            lamps.at(i).Draw();
        }

        // Render Blender models
        backpack.Translate(glm::vec3(0.f, 2.f, 0.f));
        backpack.Scale(glm::vec3(1.f, 1.f, 1.f));
        backpack.Draw();

        // Floor
        for (size_t i = 0; i < floor.size(); i++)
        {
            floor.at(i).Translate(positions.at(i));
            floor.at(i).Draw();
        }

        gun.Translate(glm::vec3(-3.0f, 2.f, 0.f));
        gun.Scale(glm::vec3(0.5f, 0.5f, 0.5f));
        gun.Draw();

        // Render skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyboxShader.setMat4("projection", projection);

        skybox.Draw(skyboxShader);
        glDepthFunc(GL_LESS);

        // Checking events
        glfwSwapBuffers(_pWindow);
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
    static bool first = true;
    if (first)
    {
        _lastX = xpos;
        _lastY = ypos;
        first = false;
    }

    _pCamera->ProcessMouseMovement(xpos - _lastX, _lastY - ypos);

    _lastX = xpos;
    _lastY = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        _mouseLeftPressed = true;
    }
    else
    {
        _mouseLeftPressed = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    _pCamera->procesZoom(yoffset);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE: //fallthrough
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;

        case GLFW_KEY_F:

            static bool fullscreen = true; // says where i am switching to
            const GLFWvidmode* pWindowMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            glfwSetWindowMonitor(window, fullscreen ? glfwGetPrimaryMonitor() : NULL, 0, 0, pWindowMode->width, pWindowMode->height, GLFW_DONT_CARE);

            fullscreen = !fullscreen;
            break;
        }
    }
}

void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void processInput(GLFWwindow* window)
{
    // Movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        _pCamera->ProcessKeyboard(CameraMovement::FORWARD, _deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        _pCamera->ProcessKeyboard(CameraMovement::BACKWARD, _deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        _pCamera->ProcessKeyboard(CameraMovement::LEFT, _deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        _pCamera->ProcessKeyboard(CameraMovement::RIGHT, _deltaTime);
}
