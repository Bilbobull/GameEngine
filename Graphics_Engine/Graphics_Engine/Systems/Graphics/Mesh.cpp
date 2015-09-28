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

  //int size;
  //glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

}