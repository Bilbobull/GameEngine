#include "Mesh.h"
#include "LoadShader.h"
#include "GraphicsSystem.h"

GLuint SimpleProgram;
GLuint SimpleMatUniform;

GLuint ModelProgram;
GLuint ModelMatUniform;
GLuint ModelTextureUniform;


void Mesh::Init_Mesh_Shader(void)
{
  ModelProgram = LoadShaders("ModelVertexShader.glsl", "ModelFragmentShader.glsl");
  ModelMatUniform = glGetUniformLocation(ModelProgram, "FullTransformMatrix");
  ModelTextureUniform = glGetUniformLocation(ModelProgram, "Texture");

  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  SimpleMatUniform = glGetUniformLocation(SimpleProgram, "FullTransformMatrix");
}


void Mesh::Draw(glm::mat4 matrix)
{

  switch (type)
  {
  case Models:
  {
    if (g_GraphicsSys->GetDebugDraw() == true)
    {
      glUseProgram(SimpleProgram);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    else
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glUseProgram(ModelProgram);
      texture->Bind();
      glUniform1i(ModelTextureUniform, 0);
    }

    vao->Bind();
    glUniformMatrix4fv(ModelMatUniform, 1, GL_FALSE, &matrix[0][0]);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_SHORT, 0);
    break;
  }
  case Shape:
    if (g_GraphicsSys->GetDebugDraw())
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glUseProgram(SimpleProgram);
    vao->Bind();
    glUniformMatrix4fv(SimpleMatUniform, 1, GL_FALSE, &matrix[0][0]);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_SHORT, 0);
  }

}