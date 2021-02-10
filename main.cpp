#include <iostream>

#include <opencv2\opencv.hpp>

#include <GL/glew.h> 
#include <GL/wglew.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    int width = 800;
    int height = 600;

    GLFWwindow* pWindow = glfwCreateWindow(width, height, "Context", NULL, NULL);
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

    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

    while (!glfwWindowShouldClose(pWindow))
    {
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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

void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}
