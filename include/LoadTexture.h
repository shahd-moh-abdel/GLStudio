#ifndef LOAD_TEXTURE_H
#define LOAD_TEXTURE_H

#include <glad/glad.h>
#include <string>

class LoadTexture {
 public:
  static GLuint LoadFromFile(const std::string& path);
  static void DeleteTexture(GLuint textureID);
};


#endif
