#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>

#include "WindowInit.h"
#include "LoadShaders.h"
#include "Quad.h"
#include "LoadTexture.h"

enum class Effect {
  NORMAL = 0,
  CRT = 1,
  PIXELATION = 2
};

Effect currentEffect = Effect::PIXELATION;
Effect previousEffect = Effect::PIXELATION;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
    {
      switch(key)
	{
	case GLFW_KEY_0:
	  currentEffect = Effect::NORMAL;
	  break;
	case GLFW_KEY_1:
	  currentEffect = Effect::CRT;
	  break;
	case GLFW_KEY_2:
	  currentEffect = Effect::PIXELATION;
	  break;
	case GLFW_KEY_ESCAPE:
	  glfwSetWindowShouldClose(window, true);
	  break;
	 
	}
    }
}

int main()
{
  GLFWwindow* window = WindowInit(800, 600);
  if (window == nullptr) return -1;

  glfwSetKeyCallback(window, keyCallback);
  
  ShaderProgram normalShader;
  if (!normalShader.LoadFromFiles("../shaders/vertex.glsl", "../shaders/frag.glsl"))
    {
      std::cerr << "Failed to load the normal shader" << std::endl;
      return -1;
    }

  ShaderProgram crtShader;
  if (!crtShader.LoadFromFiles("../shaders/vertex.glsl", "../shaders/crt.glsl"))
    {
      std::cerr << "Failed to load crt shader" << std::endl;
      return -1;
    }
  
  ShaderProgram pixelationShader;
  if (!pixelationShader.LoadFromFiles("../shaders/vertex.glsl", "../shaders/pixelation.glsl"))
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

  normalShader.Use();
  normalShader.SetInt("uTexture", 0);

  crtShader.Use();
  crtShader.SetInt("uTexture", 0);

  pixelationShader.Use();
  pixelationShader.SetInt("uTexture", 0);
  
  float time = 0.0f;
  
  while(!glfwWindowShouldClose(window))
    {
      time = (float)glfwGetTime();
      
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);   

      ShaderProgram* activeShader = nullptr;

      switch (currentEffect)
	{
	case Effect::NORMAL:
	  activeShader = &normalShader;
	  break;
	case Effect::PIXELATION:
	  activeShader = &pixelationShader;
	  break;
	case Effect::CRT:
	  activeShader = &crtShader;
	  break;
	}

      if (activeShader)
	{
	  activeShader->Use();
	  activeShader->SetFloat("uTime", time);
	  activeShader->SetVec2("uResolution", 800.0f, 600.0f);
	}
      
      quad.Draw();
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  LoadTexture::DeleteTexture(texture);
  glfwTerminate();
  return 0;
}
