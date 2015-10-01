#include "ParticleGenerator.h"
#include "ParticleSystem.h"

CircleGenerator::CircleGenerator(glm::vec3 _center, glm::vec3 _radius /*= Default_Value()*/)
{
  center = _center;
  radius = _radius;
}

void CircleGenerator::Generate(int start, int end, ParticleArray* p_array)
{
  double ang = glm::linearRand(0.0, 2.0 * M_PI);

  // Check Z value, might be incorrect
  for (int i = start; i < end; ++i)
    p_array->position[i] = center + glm::vec3(radius.x * sin(ang), radius.y * cos(ang), radius.z * tan(ang));

}
