#include "Object.h"
#include "GraphicsSystem.h"


Object::Object(Meshes meshtype, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
{
  mesh = MeshGenerator::Create_Shape(meshtype);
  position = pos;
  rotation = rot;
  scale = sca;
}

void Object::Draw(void)
{
  glm::mat4 matrix = g_GraphicsSys->GetCurrentCamera().getProjectionMatrix() * g_GraphicsSys->GetCurrentCamera().getWorldToViewMatrix()  * glm::translate(glm::vec3(-2.0f, 0.0f, -3.0f)) * glm::rotate(21.0f, glm::vec3(1.0f, 0.0f, 0.0f));

  mesh->Draw(matrix);
}

Object::~Object()
{
  delete mesh;
}