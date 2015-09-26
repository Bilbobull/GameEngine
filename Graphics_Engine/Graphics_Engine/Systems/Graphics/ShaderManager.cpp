#include "ShaderManager.h"
#include "LoadShader.h"

void Shader_Manager::Specify_Attributes_Simple(void)
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 6, 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 6, (char*)(sizeof(GL_FLOAT)* 3));
}

void Shader_Manager::Upload_Shader(char* vertex, char* fragment, char* name)
{
  std::string vertexpath = ShaderPath;
  std::string fragmentpath = ShaderPath;

  vertexpath += vertex;
  fragmentpath += fragment;

  GLuint program = LoadShaders(vertexpath.c_str(), fragmentpath.c_str());
  ShaderList[name] = program;
}