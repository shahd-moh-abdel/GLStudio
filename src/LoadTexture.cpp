#include "LoadTexture.h"
#include <LoadShaders.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint LoadTexture::LoadFromFile(const std::string& path)
{
  GLuint textureID;
  glGenTextures(1, &textureID);

  stbi_set_flip_vertically_on_load_thread(true);

  int width, height, nrChannels;
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  if (data)
    {
      GLenum format;
      if (nrChannels == 1)
	format = GL_RED;
      else if (nrChannels == 3)
	format = GL_RGB;
      else if (nrChannels == 4)
	format = GL_RGBA;
      else {
	std::cerr << "unsuported number of channels: " << nrChannels << std::endl;
	stbi_image_free(data);
	return 0;
      }

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      stbi_image_free(data);
      std::cout << "TEXTURE LOADED" << path << std::endl;
    }
  else {
    std::cerr << "FAILED TO LOAD TEXTURE: " << path << std::endl;
    std::cerr << stbi_failure_reason() << std::endl;
    return 0;
  }

  return textureID;
}


void LoadTexture::DeleteTexture(GLuint textureID)
{
  if (textureID != 0)
    glDeleteTextures(1, &textureID);
}
