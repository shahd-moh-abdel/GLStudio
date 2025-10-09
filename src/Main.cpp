#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "WindowInit.h"
#include "LoadShaders.h"

int main()
{
  GLFWwindow* window = WindowInit(800, 600);
  if (window == nullptr) return -1;

  ShaderProgram shader;
  if (!shader.LoadFromFiles("../shaders/vertex.glsl", "../shaders/frag.glsl"))
    {
      std::cerr << "Failed to load shaders" << std::endl;
      return -1;
    }


  while(!glfwWindowShouldClose(window))
    {
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);   
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  return 0;
}
