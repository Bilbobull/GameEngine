#include "Mesh.h"
#include "LoadShader.h"


GLuint SimpleProgram;
GLuint MatUniform;

void Mesh::Init_Mesh_Shader(void)
{
  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  MatUniform = glGetUniformLocation(SimpleProgram, "FullTransformMatrix");
}


void Mesh::Draw(glm::mat4 matrix)
{
  glUseProgram(SimpleProgram);
  vao->Bind();
  glUniformMatrix4fv(MatUniform, 1, GL_FALSE, &matrix[0][0]);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

}