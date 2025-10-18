#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "WindowInit.h"
#include "LoadShaders.h"
#include "Quad.h"
#include "LoadTexture.h"
#include "stb_image.h"

#include <cstdlib>
#include <array>
#include <memory>

enum class Effect {
  NORMAL = 0,
  CRT = 1,
  PIXELATION = 2
};

Effect currentEffect = Effect::NORMAL;
Effect previousEffect = Effect::NORMAL;

GLuint g_currentTexture = 0;
GLFWwindow *g_window = nullptr;
int g_imageWidth = 0;
int g_imageHeight = 0;

std::string OpenFileDialog()
{
  //zenity file dialog
  std::array<char, 512> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(
    popen("zenity --file-selection --title='select an image' --file-filter='Images | *.jpg *jpeg *.png *.bmp'", "r"),
    pclose
    );

  if (pipe)
    {
      while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
	{
	  result += buffer.data();
	}

      if (!result.empty() && result.back() == '\n')
	{
	  result.pop_back();
	}
      return result;
    }
  return "";
}

bool GetImageDimensions(const std::string& path, int& width, int& height)
{
  stbi_set_flip_vertically_on_load_thread(false);
  int channels;
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if (data)
    {
      stbi_image_free(data);
      return true;
    }
  return false;
}

void LoadNewImage(const std::string& imagePath)
{
  if (imagePath.empty())
    {
      std::cout << "No image selected" << std::endl;
      return;
    }

  int width, height;
  if (!GetImageDimensions(imagePath, width, height))
    {
      std::cerr << "failed to get image dimentions:" << imagePath << std::endl;
      return;
    }

  if (g_currentTexture != 0)
      LoadTexture::DeleteTexture(g_currentTexture);


  GLuint newTexture = LoadTexture::LoadFromFile(imagePath);
  if (newTexture == 0)
    {
      std::cerr << "failed to load image:"<< imagePath << std::endl;
      return;
    }

  g_currentTexture = newTexture;
  g_imageWidth = width;
  g_imageHeight = height;

  glfwSetWindowSize(g_window, width, height);
  glViewport(0, 0, width, height);

  std::cout << "loaded image: " << imagePath << std::endl;
}

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
	case GLFW_KEY_U:
	  {
	    std::string imagePath = OpenFileDialog();
	    LoadNewImage(imagePath);
	  }
	  break;
	case GLFW_KEY_ESCAPE:
	  glfwSetWindowShouldClose(window, true);
	  break;
	 
	}
    }
}

int main()
{
  std::string initialImagePath = "../images/test.jpg";
  int initialWidth = 800;
  int initialHeight = 600;

  if (GetImageDimensions(initialImagePath, initialWidth, initialHeight))
    {
      g_imageWidth = initialWidth;
      g_imageHeight = initialHeight;

      std::cout << "initial image dimensions: " << initialWidth << initialHeight << std::endl;
    }
  
  g_window = WindowInit(g_imageWidth, g_imageHeight);
  if (g_window == nullptr) return -1;

  glfwSetKeyCallback(g_window, keyCallback);
  
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

  g_currentTexture =  LoadTexture::LoadFromFile(initialImagePath);
  if (g_currentTexture == 0)
    {
      std::cerr << "failed to load initial image" << std::endl;
      return -1;
    }

  normalShader.Use();
  normalShader.SetInt("uTexture", 0);

  crtShader.Use();
  crtShader.SetInt("uTexture", 0);

  pixelationShader.Use();
  pixelationShader.SetInt("uTexture", 0);
  
  float time = 0.0f;
  
  while(!glfwWindowShouldClose(g_window))
    {
      time = (float)glfwGetTime();
      
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, g_currentTexture);
      
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
	  activeShader->SetVec2("uResolution", (float)g_imageWidth, (float)g_imageHeight);
	}
      
      quad.Draw();
      
      glfwSwapBuffers(g_window);
      glfwPollEvents();
    }

  LoadTexture::DeleteTexture(g_currentTexture);
  glfwTerminate();
  return 0;
}
