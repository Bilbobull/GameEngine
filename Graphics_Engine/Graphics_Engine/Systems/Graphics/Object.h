#pragma once
#include "MeshGenerator.h"
#include <vector>

class Object
{
public:
  ~Object();
  Object(Meshes meshtype = Meshes::Cube_Mesh, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 sca = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0.0f, glm::vec3 rot = glm::vec3(1.0f, 0.0f, 0.0f));
  Object(std::string filename, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 sca = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0.0f, glm::vec3 rot = glm::vec3(1.0f, 0.0f, 0.0f));
  void Draw(void);
  
  Mesh* mesh;
  glm::vec3 position;
  float rotAngle;
  glm::vec3 rotation;
  glm::vec3 scale;

};

