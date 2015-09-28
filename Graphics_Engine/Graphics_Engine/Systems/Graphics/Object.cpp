#include "Object.h"
#include "GraphicsSystem.h"


Object::Object(Meshes meshtype, glm::vec3 pos, glm::vec3 sca, float angle, glm::vec3 rot)
{
  mesh = MeshGenerator::Create_Shape(meshtype);
  position = pos;
  rotation = rot;
  rotAngle = angle;
  scale = sca;
}

Object::Object(std::string filename, glm::vec3 pos /*= glm::vec3(0.0f, 0.0f, 0.0f)*/, glm::vec3 sca /*= glm::vec3(1.0f, 1.0f, 1.0f)*/, float angle /*= 0.0f*/, glm::vec3 rot /*= glm::vec3(1.0f, 0.0f, 0.0f)*/) :
position(pos),
scale(sca),
rotation(rot),
rotAngle(angle)
{
  mesh = MeshGenerator::makeMeshFromObj(filename);
}

void Object::Draw(void)
{
  // If u do translation before rotation then it rotates around the center of the world
  // instead of around the center of the object

  // projection * translation * rotation
  glm::mat4 matrix = g_GraphicsSys->GetCurrentCamera().getProjectionMatrix() * g_GraphicsSys->GetCurrentCamera().getWorldToViewMatrix()  * glm::translate(position)/* * glm::rotate(rotAngle, rotation)*/;
  mesh->Draw(matrix);
}

Object::~Object()
{
  delete mesh;
}