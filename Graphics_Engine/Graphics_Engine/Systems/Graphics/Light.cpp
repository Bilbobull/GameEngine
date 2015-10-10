#include "Light.h"

//Light::Light(glm::vec4 const &_direction, glm::vec4 const &_ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
//  glm::vec4 const &_diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) : direction(_direction), ambient(_ambient), diffuse(_diffuse)
//{}
glm::vec4 Lightdirection[MAX_LIGHTS];
glm::vec4 Lightambient[MAX_LIGHTS];
glm::vec4 Lightdiffuse[MAX_LIGHTS];
glm::vec4 Lightspecular[MAX_LIGHTS];
Material MaterialVal;
int LightNum = 0;