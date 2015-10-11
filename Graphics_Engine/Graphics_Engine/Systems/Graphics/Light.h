#include"Math_Headers.h"
#include "ObjectManager.h"
#include <vector>
#define MAX_LIGHTS 8
#define  DIRECTIONAL 1
#define  SPOT 2
#define  POINT 3

struct Material
{
  glm::vec4 ambient; // ambient color of the surface/how much ambient light to absorb
  glm::vec4 diffuse; // diffuse color of the surface/how much diffuse light to absorb
  glm::vec4 specular;
};

void RotateLights(void);

extern std::vector<Object*> LightObjects;
extern int Lighttype[MAX_LIGHTS];
extern glm::vec4 Lightposition[MAX_LIGHTS];
extern glm::vec4 Lightdirection[MAX_LIGHTS];
extern glm::vec4 Lightambient[MAX_LIGHTS];
extern glm::vec4 Lightdiffuse[MAX_LIGHTS];
extern glm::vec4 Lightspecular[MAX_LIGHTS];
extern Material MaterialVal;
extern int LightNum;
extern float Shininess;
extern float DistanceAttConstants[3];
extern int DistanceAtt;