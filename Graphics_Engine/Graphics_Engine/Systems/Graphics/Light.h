#include"Math_Headers.h"
#define MAX_LIGHTS 8

struct Material
{
  glm::vec4 ambient; // ambient color of the surface/how much ambient light to absorb
  glm::vec4 diffuse; // diffuse color of the surface/how much diffuse light to absorb
  glm::vec4 specular;
};

extern glm::vec4 Lightdirection[MAX_LIGHTS];
extern glm::vec4 Lightambient[MAX_LIGHTS];
extern glm::vec4 Lightdiffuse[MAX_LIGHTS];
extern glm::vec4 Lightspecular[MAX_LIGHTS];
extern Material MaterialVal;
extern int LightNum;