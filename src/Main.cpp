#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "WindowInit.h"
#include "LoadShaders.h"
#include "Quad.h"
#include "LoadTexture.h"

GLuint loadTexture(const char* path);

int main()
{
  GLFWwindow* window = WindowInit(800, 600);
  if (window == nullptr) return -1;

  ShaderProgram shader;
  if (!shader.LoadFromFiles("../shaders/vertex.glsl", "../shaders/crt.glsl"))
    {
      std::cerr << "Failed to load shaders" << std::endl;
      return -1;
    }

  Quad quad;

  GLuint texture = LoadTexture::LoadFromFile("../images/test.jpg");
  if (texture == 0)
    {
      std::cerr << "failed to load image" << std::endl;
      return -1;
    }

  shader.Use();
  shader.SetInt("uTexture", 0);

  float time = 0.0f;
  
  while(!glfwWindowShouldClose(window))
    {
      time = (float)glfwGetTime();
      
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);   

      shader.Use();

      shader.SetFloat("uTime", time);
      shader.SetVec2("uResolution", 800.0f, 600.0f);

      quad.Draw();
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  LoadTexture::DeleteTexture(texture);
  glfwTerminate();
  return 0;
}
