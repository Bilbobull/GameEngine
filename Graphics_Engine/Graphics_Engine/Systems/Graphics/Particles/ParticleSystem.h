#pragma once
#include "../Graphics_Headers.h"
#include "../Math_Headers.h"
#include <vector>
#include "ParticleEmiter.h"
#include "ParticleUpdater.h"
#include "ParticleGenerator.h"

// Everything public, wanna be able to access these elements
struct ParticleArray{
  //Per particle data
  std::vector <glm::vec3> position;
  std::vector <glm::vec3> velocity;
  std::vector <glm::vec4> startcolor;
  std::vector <glm::vec4> endcolor;
  std::vector <glm::vec4> color;
  std::vector <float> timeleft;
  std::vector <bool> alive;
  const int max_Particles;
  int alive_Particles;


  ParticleArray(int maxParticles);
  void Wake(int index);
  void Kill(int index);

};

class ParticleSystem{
public:
  ParticleSystem(int maxParticles);

  void AddEmitter(ParticleEmitter* emitter);
  void AddUpdater(ParticleUpdater* updater);

  void Init(void);
  void Update(float dt);
  void Draw(void);
  void Free(void);


private:
  ParticleArray* particle_array;
  const int max_Particles;
  
  std::vector <ParticleEmitter*> emitters;
  std::vector <ParticleUpdater*> updaters;
};


