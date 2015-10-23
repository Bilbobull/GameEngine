#include "ParticleUpdater.h"
#include "ParticleSystem.h"

void VelocityUpdater::Update(ParticleArray *p, float dt)
{
  for (int i = 0; i < p->alive_Particles; ++i)
  {
    p->particles[i].position += p->particles[i].velocity * dt;
  }
}

void LifeUpdater::Update(ParticleArray *p_array, float dt)
{
  int end = p_array->alive_Particles;

  for (int i = 0; i < end; ++i)
  {
    p_array->particles[i].timeleft -= dt;
    if (p_array->particles[i].timeleft < 0.f)
    {
      p_array->Kill(i);
      end = p_array->alive_Particles < p_array->max_Particles ? p_array->alive_Particles : p_array->max_Particles;
    }
  }
}

void ColorUpdater::Update(ParticleArray *p, float dt)
{
  for (int i = 0; i < p->alive_Particles; ++i)
  {
    p->particles[i].color = glm::mix(p->particles[i].startcolor, p->particles[i].endcolor, p->particles[i].timeleft);
  }
}

void TimeBombUpdater::Update(ParticleArray *p_array, float dt)
{
  if (m_maxTime != -1.0)
  {
    int endId = p_array->alive_Particles;
    m_timeCounter += dt;
    if (m_maxTime <= m_timeCounter)
    {
      p_array->alive_Particles = 0;
    }
  }
}