#ifndef QUAD_H
#define QUAD_H

#include <glad/glad.h>

class Quad {
public:
  Quad();
  ~Quad();
  
  void Draw();

private:
  GLuint m_VAO;
  GLuint m_VBO;
  
  void setupQuad();
  void cleanup();
  
};

#endif
