#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <WindowInit.h>

int main()
{
  GLFWwindow* window = WindowInit(700, 700);
  if (window == nullptr) return -1;

  while(!glfwWindowShouldClose(window))
    {
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  return 0;
}
