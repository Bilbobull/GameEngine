#include "ParticleRenderer.h"
#include "ParticleSystem.h"
#include "../../Texture.h"
#include "../../LoadShader.h"
#include "../../GraphicsSystem.h"


Texture* texture;
GLuint ParticleProgram;
GLuint matrixLocation = 0;
glm::mat4 matrix;

void ParticleRenderer::Init(ParticleSystem* sys)
{
  vao = new VAO();
  vao->unBind();
  p_sys = sys;

  texture = new Texture("Particle");
  ParticleProgram = LoadShaders("ParticleVertexShader.glsl", "ParticleFragmentShader.glsl");

  glUseProgram(ParticleProgram);
  texture->TexBind();
  GLint uniTex = glGetUniformLocation(ParticleProgram, "uniTexture");
  unsigned err = glGetError();

  glUniform1i(uniTex, 0);
  texture->unBind();

  matrixLocation = glGetUniformLocation(ParticleProgram, "uniViewProjection");
  glUseProgram(0);
}

glm::vec3 getRandomVector3(float mn, float max)
{
  return glm::vec3(glm::linearRand(mn, max), glm::linearRand(mn, max), glm::linearRand(mn, max));
}

glm::vec4 getRandomVector4(float mn, float max)
{
  return glm::vec4(glm::linearRand(mn, max), glm::linearRand(mn, max), glm::linearRand(mn, max), 1.0f);
}

void ParticleRenderer::InitCompute(ParticleSystem* sys)
{
  p_sys = sys;

  auto particleSize = sizeof(ComputeParticle);
  particleBuffer = new SSBO(p_sys->GetMaxParticles() * particleSize, GL_DYNAMIC_DRAW);

  auto* particles = particleBuffer->MapBufferRange <ComputeParticle>(0, p_sys->GetMaxParticles());
  for (auto i = 0; i < p_sys->GetMaxParticles(); ++i)
  {
    particles[i].position = p_sys->GetParticleArray()->particles[i].position;
    particles[i].color = p_sys->GetParticleArray()->particles[i].color;
    particles[i].velocity = p_sys->GetParticleArray()->particles[i].velocity;
  }
  particleBuffer->UnMapBuffer();
  particleBuffer->BindBufferBase(0);

  computeshader = LoadComputeShader("Systems/Graphics/Shaders/Simple.cs.glsl");
}

void ParticleRenderer::ComputeRender()
{
  auto particleSize = sizeof(ComputeParticle);
  particleBuffer = new SSBO(p_sys->GetMaxParticles() * particleSize, GL_DYNAMIC_DRAW);

  auto* particles = particleBuffer->MapBufferRange <ComputeParticle>(0, p_sys->GetMaxParticles());
  for (auto i = 0; i < p_sys->GetMaxParticles(); ++i)
  {
    particles[i].position = p_sys->GetParticleArray()->particles[i].position;
    particles[i].color = p_sys->GetParticleArray()->particles[i].color;
    particles[i].velocity = p_sys->GetParticleArray()->particles[i].velocity;
  }
  particleBuffer->UnMapBuffer();
  particleBuffer->BindBufferBase(0);


  particleBuffer->BindBufferBase(0);

  computeshader->Bind();
  {
    int workingGroups = p_sys->GetAlivePartCount() / 16;
    computeshader->Dispatch_Compute(workingGroups + 2, 1, 1);
  }
  computeshader->unBind();

  glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  
  glUseProgram(ParticleProgram);
  {
    texture->TexBind();
    matrix = g_GraphicsSys->GetCurrentCamera().getProjectionMatrix() * g_GraphicsSys->GetCurrentCamera().getWorldToViewMatrix();
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &matrix[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer->Get_POS());

    auto particleAttribute = glGetAttribLocation(ParticleProgram, "Position");
    glEnableVertexAttribArray(particleAttribute);
    glVertexAttribPointer(particleAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ComputeParticle), 0);

    glEnableVertexAttribArray(particleAttribute + 1);
    glVertexAttribPointer(particleAttribute + 1, 4, GL_FLOAT, GL_FALSE, sizeof(ComputeParticle), (void*)(sizeof (glm::vec3)));

    glEnableVertexAttribArray(particleAttribute + 2);
    glVertexAttribPointer(particleAttribute + 2, 4, GL_FLOAT, GL_FALSE, sizeof(ComputeParticle), (void*)(2 * sizeof(glm::vec4)));

    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5);
    glDrawArrays(GL_POINTS, 0, p_sys->GetAlivePartCount());
  }
  glUseProgram(0);
}