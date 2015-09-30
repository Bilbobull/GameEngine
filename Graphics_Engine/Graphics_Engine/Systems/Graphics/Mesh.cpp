#include "Mesh.h"
#include "LoadShader.h"

GLuint SimpleProgram;
GLuint ModelProgram;
GLuint ModelMatUniform;
GLuint SimpleMatUniform;

void Mesh::Init_Mesh_Shader(void)
{
  ModelProgram = LoadShaders("ModelVertexShader.glsl", "ModelFragmentShader.glsl");
  ModelMatUniform = glGetUniformLocation(ModelProgram, "FullTransformMatrix");

  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  SimpleMatUniform = glGetUniformLocation(SimpleProgram, "FullTransformMatrix");
}


void Mesh::Draw(glm::mat4 matrix)
{

  switch (type)
  {
  case Models:
    glUseProgram(ModelProgram);
    vao->Bind();
    glUniformMatrix4fv(ModelMatUniform, 1, GL_FALSE, &matrix[0][0]);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_SHORT, 0);

  case Shape:
    glUseProgram(SimpleProgram);
    vao->Bind();
    glUniformMatrix4fv(SimpleMatUniform, 1, GL_FALSE, &matrix[0][0]);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_SHORT, 0);
  }

}