#pragma once
#include <vector>

struct ParticleGenerator;
struct ParticleArray;

class ParticleEmitter{
public:
  ParticleEmitter();
  void AddGenerator(ParticleGenerator* generator);
  void Emit(ParticleArray* p_array);
private:
  int min_Particles;
  std::vector <ParticleGenerator*> generators;
};