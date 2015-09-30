#include "Object.h"
#include "GraphicsSystem.h"


Object::Object(glm::vec3 pos, glm::vec3 sca, float angle, glm::vec3 rot, std::string filename, Meshes meshtype)
{
  mesh = MeshGenerator::Create_Shape(filename, meshtype);
  position = pos;
  rotation = rot;
  rotAngle = angle;
  scale = sca;
}

void Object::Draw(void)
{
  // If u do translation before rotation then it rotates around the center of the world
  // instead of around the center of the object

  // projection * translation * rotation
  glm::mat4 matrix = g_GraphicsSys->GetCurrentCamera().getProjectionMatrix() * g_GraphicsSys->GetCurrentCamera().getWorldToViewMatrix()  * glm::translate(position) * glm::rotate(rotAngle, rotation);
  mesh->Draw(matrix);
}

Object::~Object()
{
  delete mesh;
}