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

    _pCamera = new Camera(glm::vec3(0.f, 0.f, 10.f));

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
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(_pCamera->getFov()), (float)width / (float)height, 0.1f, 100.0f);

    Shader shader("vertex.glsl", "fragment.glsl");
    Shader skyboxShader("skybox_vertex.glsl", "skybox_fragment.glsl");

    shader.setMat4("model", model);
    shader.setMat4("view", view);

    Model _model("models/backpack/backpack.obj");
    Model _floor("models/floor_tile/floor.obj");

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    Skybox skybox(SKYBOX);

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

        // directional light
        shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        shader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        shader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        // spotLight
        shader.setVec3("spotLight.position", _pCamera->getPosition());
        shader.setVec3("spotLight.direction", _pCamera->getFront());
        shader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setFloat("spotLight.constant", 1.0f);
        shader.setFloat("spotLight.linear", 0.09);
        shader.setFloat("spotLight.quadratic", 0.032);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        shader.setVec3("light.position", _pCamera->getPosition());
        shader.setVec3("light.direction", _pCamera->getFront());
        shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

        shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("light.specular", 1.f * glm::vec3(1.f, 1.f, 1.f));

        shader.setFloat("light.constant", 1.0f);
        shader.setFloat("light.linear", 0.09f);
        shader.setFloat("light.quadratic", 0.032f);

        shader.setVec3("view_position", _pCamera->getPosition());

        view = _pCamera->getViewMatrix();
        shader.setMat4("view", view);

        glm::mat4 projection = glm::perspective(glm::radians(_pCamera->getFov()), (float)width / (float)height, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
       
        shader.setMat4("model", model);
        _model.Draw(shader);

        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

        shader.setMat4("model", model);
        _floor.Draw(shader);

        view = glm::mat4(glm::mat3(_pCamera->getViewMatrix())); // remove translation from the view matrix
        
        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        skybox.Draw(skyboxShader);

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
