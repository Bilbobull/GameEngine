#pragma once
#include "Graphics_Headers.h"
#include "Math_Headers.h"
#include "Buffer_Manager.h"

struct Vertex
{
  glm::vec3 position;
  //glm::vec3 normal;
  glm::vec3 color;
};

class Mesh
{
public:
  Mesh(){}
  ~Mesh(){}
  void Draw(glm::mat4 matrix);
  static void Init_Mesh_Shader(void);

  std::vector <Vertex> vertices;
  std::vector <GLushort> indices;

  VAO* vao;
  VBO* vbo;
  EBO* ebo;
};