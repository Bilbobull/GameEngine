#pragma once
#include <map>
#include "Graphics_Headers.h"
typedef void(*callback)(void);

class Shader_Manager{
public:
  Shader_Manager()
  {
    ShaderPath = "Shaders/";
  }
  ~Shader_Manager()
  {
    Unload_Shaders();
  }

  void Upload_Shader(char* vertex, char* fragment, char* name);

  void Specify_Atributes(char* name, callback );
  void Specify_Attributes_Simple(void);
  void Unload_Shaders(void);
private:
  std::map <std::string, GLuint> ShaderList;
  std::string ShaderPath;
};