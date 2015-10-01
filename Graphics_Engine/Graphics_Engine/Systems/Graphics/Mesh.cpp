#include "Mesh.h"
#include "LoadShader.h"
#include "GraphicsSystem.h"

GLuint SimpleProgram;
GLuint SimpleMatUniform;

GLuint ModelProgram;
GLuint ModelMatUniform;
GLuint DiffuseTextureUniform;
GLuint NormalTextureUniform;


void Mesh::Init_Mesh_Shader(void)
{
  ModelProgram = LoadShaders("ModelVertexShader.glsl", "ModelFragmentShader.glsl");
  ModelMatUniform = glGetUniformLocation(ModelProgram, "FullTransformMatrix");
  DiffuseTextureUniform = glGetUniformLocation(ModelProgram, "Texture");
  NormalTextureUniform = glGetUniformLocation(ModelProgram, "normalTexture");

  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  SimpleMatUniform = glGetUniformLocation(SimpleProgram, "FullTransformMatrix");
}


void Mesh::Draw(glm::mat4 matrix)
{

  switch (type)
  {
  case Models:
  {
    if (g_GraphicsSys->GetDebugDraw() == true || texture == nullptr)
    {
      glUseProgram(SimpleProgram);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    else
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glUseProgram(ModelProgram);

      glActiveTexture(GL_TEXTURE0);
      texture->TexBind();
      glUniform1i(DiffuseTextureUniform, 0);

      glActiveTexture(GL_TEXTURE1);
      texture->NorBind();
      glUniform1i(NormalTextureUniform, 1);

      glActiveTexture(GL_TEXTURE0);
    }

    vao->Bind();
    glUniformMatrix4fv(ModelMatUniform, 1, GL_FALSE, &matrix[0][0]);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_SHORT, 0);
    texture->unBind();
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