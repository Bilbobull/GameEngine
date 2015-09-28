#include "ObjectManager.h"

Object* ObjectManager::CreateObject(Meshes meshtype, glm::vec3 pos, glm::vec3 sca, float angle, glm::vec3 rot)
{
  Object* object = new Object(meshtype, pos, sca, angle, rot);
  Object_List.push_back(object);

  return object;
}

Object* ObjectManager::CreateMeshObject(std::string filename, glm::vec3 pos, glm::vec3 sca, float angle, glm::vec3 rot)
{
  Object* object = new Object(filename, pos, sca, angle, rot);
  Object_List.push_back(object);

  return object;
}

std::vector <Object*> ObjectManager::Object_List;

