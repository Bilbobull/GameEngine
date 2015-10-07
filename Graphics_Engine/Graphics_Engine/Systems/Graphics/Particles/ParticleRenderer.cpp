#include "ParticleRenderer.h"
#include "ParticleSystem.h"
#include "..\Texture.h"
#include "..\LoadShader.h"
#include "..\GraphicsSystem.h"

Texture* texture;
GLuint ParticleProgram;
GLuint matrixLocation = 0;
glm::mat4 matrix;


void ParticleRenderer::Specify_Attributes(void)
{
  GLuint posAttrib = glGetAttribLocation(ParticleProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  GLuint colAttrib = glGetAttribLocation(ParticleProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
}

void ParticleRenderer::Init(ParticleSystem* sys)
{
  p_sys = sys;
  if (!ParticleProgram)
  {
    texture = new Texture("Particle");
    ParticleProgram = LoadShaders("ParticleVertexShader.glsl", "ParticleFragmentShader.glsl");
  }

  vao.Bind();

  glGenBuffers(1, &vbo_pos);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
  GLint posAttrib = glGetAttribLocation(ParticleProgram, "position");
  glBufferData(GL_ARRAY_BUFFER, 3 * p_sys->GetMaxParticles() * sizeof (float), nullptr, GL_STREAM_DRAW);
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), 0);

  glGenBuffers(1, &vbo_col);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
  GLint colAttrib = glGetAttribLocation(ParticleProgram, "color");
  glBufferData(GL_ARRAY_BUFFER, 4 * p_sys->GetMaxParticles() * sizeof (float), nullptr, GL_STREAM_DRAW);
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 4 * sizeof (float), 0);
 
  vao.unBind();

  texture->TexBind();
  GLuint uniTex = glGetUniformLocation(ParticleProgram, "image");
  glUniform1i(uniTex, 0);
  texture->unBind();

  matrixLocation = glGetUniformLocation(ParticleProgram, "matrix");
}

void ParticleRenderer::Render(void)
{
  if (p_sys->GetAlivePartCount() > 0)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_ALPHA);
    texture->TexBind();
    matrix = g_GraphicsSys->GetCurrentCamera().getProjectionMatrix() * g_GraphicsSys->GetCurrentCamera().getWorldToViewMatrix();
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glUseProgram(ParticleProgram);


    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferSubData(GL_ARRAY_BUFFER, 0, p_sys->GetAlivePartCount() * 3 * sizeof (float), (void*)p_sys->GetPositionData());
    glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
    glBufferSubData(GL_ARRAY_BUFFER, 0, p_sys->GetAlivePartCount() * 4 * sizeof(float), (void*)p_sys->GetColorData());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPointSize(20);
    vao.Bind();
    glDrawArrays(GL_POINTS, 0, p_sys->GetAlivePartCount());
    glUseProgram(0);
    vao.unBind();
    texture->unBind();
    glDisable(GL_BLEND);
  }
}