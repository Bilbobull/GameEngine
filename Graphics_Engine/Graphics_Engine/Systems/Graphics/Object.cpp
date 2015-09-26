#include "Object.h"


Object::Object(Meshes meshtype, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
{
  mesh = ShapeGenerator::Create_Shape(meshtype);
  position = pos;
  rotation = rot;
  scale = sca;
}

Object::~Object()
{
  delete mesh;
}