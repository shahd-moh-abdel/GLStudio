#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <WindowInit.h>

void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

GLFWwindow* WindowInit(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
  glfwInit();

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLStudio", NULL, NULL);

  if (window == NULL)
    {
      std::cout << "ERROR: window not created" << std::endl;
      glfwTerminate();
      return nullptr;
    }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "ERROR: glad not initialized" << std::endl;
      return nullptr;
    }

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
  
  return window;
}
