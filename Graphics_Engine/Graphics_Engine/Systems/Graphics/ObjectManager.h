#pragma once
#include "Object.h"

class ObjectManager
{

public:
  static Object* CreateObject(Meshes meshtype = Meshes::Cube_Mesh, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 sca = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0.0f, glm::vec3 rot = glm::vec3(1.0f, 0.0f, 0.0f));

public:
  static std::vector <Object*>& GetObjectList(){ return Object_List; }
  static Object* CreateMeshObject(std::string filename, glm::vec3 pos, glm::vec3 sca, float angle, glm::vec3 rot);
private:
  static std::vector <Object*> Object_List;
};