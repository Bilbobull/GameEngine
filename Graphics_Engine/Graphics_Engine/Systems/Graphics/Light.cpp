#include "Light.h"

//Light::Light(glm::vec4 const &_direction, glm::vec4 const &_ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
//  glm::vec4 const &_diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) : direction(_direction), ambient(_ambient), diffuse(_diffuse)
//{}
std::vector<Object*> LightObjects;
int Lighttype[MAX_LIGHTS];
glm::vec4 Lightposition[MAX_LIGHTS];
glm::vec4 Lightdirection[MAX_LIGHTS];
glm::vec4 Lightambient[MAX_LIGHTS];
glm::vec4 Lightdiffuse[MAX_LIGHTS];
glm::vec4 Lightspecular[MAX_LIGHTS];
Material MaterialVal;
int LightNum = 0;
float Shininess = 0.8;

void RotateLights(void)
{
  unsigned i = 0;
  float rotation_rads = 0.0436332;
  for (int i = 0; i < LightNum; ++i)
  {
    glm::mat4 Translate (1.0f), Rotation(1.0f);

    glm::vec4 old_pos = Lightposition[i];
    glm::vec4 center = glm::vec4(ObjectManager::GetObjectList().at(0)->position.x, ObjectManager::GetObjectList().at(0)->position.y, ObjectManager::GetObjectList().at(0)->position.z, 1);
    glm::vec4 rotation_around = center;
    glm::vec4 old_min_rotate_a = old_pos - rotation_around;

  //  Vector4 new_pos = Vector4(old_min_rotate_a.x * cos(rotation_rads) - old_min_rotate_a.z * sin(rotation_rads),
  //    old_min_rotate_a.y,
  //    old_min_rotate_a.z * cos(rotation_rads) + old_min_rotate_a.x * sin(rotation_rads),
  //    1);

  //  it.second.position = new_pos + rotation_around;

  //  all_lights[i].direction = it.second.position - rotation_around;
  //  all_lights[i].direction.Normalize();

  //  Matrix4 proj = Matrix4::CreateProjection(60, app->GetWindowWidth(), app->GetWindowHeight(), 1, 50);
  }
}