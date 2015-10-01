#pragma once
#include "../Math_Headers.h"

struct ParticleArray;

//glm::vec3 Default_Value(float x = 0.2f, float y = 0.2f, float z = 0.2f, float w = 0.0f)
//{
//  return  glm::vec3(x, y, z);
//}

struct ParticleGenerator{
  virtual void Generate(int start, int end, ParticleArray* p_array) = 0;

private:
  glm::vec3 min_pos;
  glm::vec3 max_pos;
};

struct CircleGenerator : ParticleGenerator{
  CircleGenerator(glm::vec3 _center, glm::vec3 _radius = glm::vec3(0.2f, 0.2f, 0.2f));
  virtual void Generate(int start, int end, ParticleArray* p_array);

  glm::vec3 radius;
  glm::vec3 center;
};
// Add different generators for different effects here