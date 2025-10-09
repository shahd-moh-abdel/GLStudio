#ifndef LOAD_SHADERS_H
#define LOAD_SHADERS_H

#include <glad/glad.h>
#include <string.h>
#include <iostream>

class ShaderProgram {
 public:
  ShaderProgram();
  ~ShaderProgram();

  bool LoadFromFiles(const std::string& vertexPath,
		     const std::string& fragmentPath);

  void Use();

  GLuint GetID() const { return m_ProgramID; }

 private:
  GLuint m_ProgramID;

  std::string readFile(const std::string& filePath);
  GLuint compileShader(GLenum type, const std::string& source);
  bool linkProgram(GLuint vertexShader, GLuint fragmentShader);
  void checkCompileErrors();
};

#endif
