#include "ParticleSystem.h"


ParticleArray::ParticleArray(int maxParticles) : max_Particles(maxParticles)
{
  particles = new Particle[maxParticles];
  for (int i = 0; i < max_Particles; ++i)
  {
    particles[i].position = glm::vec3(0.0f);
    particles[i].velocity = glm::vec3(0.0f);
    particles[i].startcolor = glm::vec4(0.0f);
    particles[i].endcolor = glm::vec4(0.0f);
    particles[i].color = glm::vec4(0.0f);
    particles[i].timeleft = 1;
    particles[i].alive = false;
    particles[i].size = 0;
  }
}

void ParticleArray::Wake(int index)
{
  ++alive_Particles;
  particles[index].alive = true;
}

void ParticleArray::Kill(int index)
{
  // This might fail
  particles[index].position = particles[alive_Particles - 1].position;
  particles[index].velocity = particles[alive_Particles - 1].velocity;
  particles[index].startcolor = particles[alive_Particles - 1].startcolor;
  particles[index].endcolor = particles[alive_Particles - 1].endcolor;
  particles[index].color = particles[alive_Particles - 1].color;
  particles[index].timeleft = particles[alive_Particles - 1].timeleft;
  particles[index].alive = particles[alive_Particles - 1].alive;
  particles[index].size = particles[alive_Particles - 1].size;
  --alive_Particles;
}

ParticleSystem::ParticleSystem(int maxParticles) : max_Particles(maxParticles)
{
  particle_array = new ParticleArray(maxParticles);
  particle_array->alive_Particles = 0;
}

void ParticleSystem::AddEmitter(ParticleEmitter* emitter)
{
  emitters.push_back(emitter);
}

void ParticleSystem::AddUpdater(ParticleUpdater* updater)
{
  updaters.push_back(updater);
}

void ParticleSystem::AddRenderer(ParticleRenderer* render)
{
  renderer = render;
}

void ParticleSystem::Init(void)
{
  renderer->Init(this);
  renderer->InitCompute(this);
}

void ParticleSystem::Update(float dt)
{
  for (auto& i : emitters)
  {
    i->Emit(particle_array);
  }

  for (auto& i : updaters)
  {
    i->Update(particle_array, dt);
  }

  if (particle_array->alive_Particles == 0)
    Free();
}


void ParticleSystem::Draw(void)
{
  renderer->ComputeRender();
}


void ParticleSystem::Free(void)
{
  emitters.clear();
  updaters.clear();
}
