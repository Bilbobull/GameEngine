#include"Math_Headers.h"

struct Material
{
  glm::vec4 ambient; // ambient color of the surface/how much ambient light to absorb
  glm::vec4 diffuse; // diffuse color of the surface/how much diffuse light to absorb
  glm::vec4 specular;
};