#include <Quad.h>

Quad::Quad() : m_VAO(0), m_VBO(0)
{
  setupQuad();
}

Quad::~Quad()
{
  cleanup();
}

void Quad::setupQuad()
{
  GLfloat verticies[] = {
    -1.0f, +1.0f,
    -1.0f, -1.0f,
    +1.0f, -1.0f,

    -1.0f, +1.0f,
    +1.0f, -1.0f,
    +1.0f, +1.0f,
  };

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Quad::Draw()
{
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void Quad::cleanup()
{
  if (m_VBO != 0)
    {
      glDeleteBuffers(1, &m_VBO);
      m_VBO = 0;
    }
  if (m_VAO != 0)
    {
      glDeleteVertexArrays(1, &m_VAO);
      m_VAO = 0;
    }
}

