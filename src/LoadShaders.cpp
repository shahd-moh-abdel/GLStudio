#include <LoadShaders.h>

#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram() : m_ProgramID(0) {}

ShaderProgram::~ShaderProgram()
{
  if (m_ProgramID != 0)
    {
      glDeleteProgram(m_ProgramID);
    }
}

std::string ShaderProgram::readFile(const std::string& filePath) {
  std::ifstream file(filePath);

  if (!file.is_open())
    {
      std::cerr << "ERROR: failed to open shader file: " << filePath << std::endl;
      return "";
    }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

GLuint ShaderProgram::compileShader(GLenum type, const std::string& source)
{
  GLuint shader = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);
  
  return shader;
}

bool ShaderProgram::linkProgram(GLuint vertexShader, GLuint fragmentShader)
{
  m_ProgramID = glCreateProgram();
  glAttachShader(m_ProgramID, vertexShader);
  glAttachShader(m_ProgramID, fragmentShader);
  glLinkProgram(m_ProgramID);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return true;
}

bool ShaderProgram::LoadFromFiles(const std::string& vertexPath,
				  const std::string& fragmentPath)
{

  std::string vertexCode = readFile(vertexPath);
  std::string fragmentCode = readFile(fragmentPath);

  if (vertexCode.empty() || fragmentCode.empty())
    {
      std::cerr << "ERROR: failed to read shader files" << std::endl;
      return false;
    }

  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

  linkProgram(vertexShader, fragmentShader);
  
  return true;
}

void ShaderProgram::Use()
{
  glUseProgram(m_ProgramID);
}

void ShaderProgram::SetInt(const std::string& name, int value)
{
  glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value)
{
  glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), value);
}

void ShaderProgram::SetVec2(const std::string& name, float x, float y)
{
  glUniform2f(glGetUniformLocation(m_ProgramID, name.c_str()), x, y);
}
