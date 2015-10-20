#include "ParticleRenderer.h"
#include "ParticleSystem.h"
#include "../../Texture.h"
#include "../../LoadShader.h"
#include "../../GraphicsSystem.h"


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
  glBufferData(GL_ARRAY_BUFFER, 3 * p_sys->GetMaxParticles() * sizeof(float), nullptr, GL_STREAM_DRAW);
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  glGenBuffers(1, &vbo_col);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
  GLint colAttrib = glGetAttribLocation(ParticleProgram, "color");
  glBufferData(GL_ARRAY_BUFFER, 4 * p_sys->GetMaxParticles() * sizeof(float), nullptr, GL_STREAM_DRAW);
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

  vao.unBind();

  texture->TexBind();
  GLuint uniTex = glGetUniformLocation(ParticleProgram, "image");
  glUniform1i(uniTex, 0);
  texture->unBind();

  matrixLocation = glGetUniformLocation(ParticleProgram, "matrix");
}


void ParticleRenderer::InitCompute(ParticleSystem* sys)
{

  c_vao = new VAO();

  Posbuffer = new SSBO(p_sys->GetMaxParticles() * sizeof(glm::vec4));

  glm::vec4* verticesPos = (glm::vec4*)Posbuffer->MapBufferRange<glm::vec4>(0, p_sys->GetMaxParticles());
  for (int i = 0; i < p_sys->GetMaxParticles(); i++)
  {
    verticesPos[i] = glm::linearRand(glm::vec4(-0.25f, -0.5f, -0.5f, 1.0f), glm::vec4(0.25f, 0.5f, 0.5f, 1.0f));
  }
  Posbuffer->UnMapBuffer();
  Posbuffer->BindBufferBase(0);


  VelBuffer = new SSBO(p_sys->GetMaxParticles() * sizeof(glm::vec4));
  glm::vec4* verticesVel = VelBuffer->MapBufferRange<glm::vec4>(0, p_sys->GetMaxParticles());
  for (int i = 0; i < p_sys->GetMaxParticles(); i++)
  {
    verticesVel[i] = glm::linearRand(glm::vec4(-0.4f, -0.4f, -0.2f, 0.0f), glm::vec4(0.4f, 0.4f, 0.2f, 0.0f));
  }

  VelBuffer->UnMapBuffer();
  VelBuffer->BindBufferBase(1);


  computeshader = LoadComputeShader("Systems/Graphics/Shaders/Simple.cs.glsl");

  c_vao->unBind();
  //AccBuffer = new SSBO(NumParticles * sizeof(glm::vec4));
  //CreateAcc();
  //AccBuffer->BindBufferBase(2);

  //c_vao->unBind();
}

void ParticleRenderer::Render(void)
{
  if (p_sys->GetAlivePartCount() > 0)
  {
    texture->TexBind();
    matrix = g_GraphicsSys->GetCurrentCamera().getProjectionMatrix() * g_GraphicsSys->GetCurrentCamera().getWorldToViewMatrix();
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glUseProgram(ParticleProgram);


    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferSubData(GL_ARRAY_BUFFER, 0, p_sys->GetAlivePartCount() * 3 * sizeof(float), (void*)p_sys->GetPositionData());
    glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
    glBufferSubData(GL_ARRAY_BUFFER, 0, p_sys->GetAlivePartCount() * 4 * sizeof(float), (void*)p_sys->GetColorData());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPointSize(50);
    vao.Bind();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    //if (glfwGetKey(g_GraphicsSys->GetCurrentWindow().glfw_GetWindow(), GLFW_KEY_B))
    //{
    //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //}
    //else if (glfwGetKey(g_GraphicsSys->GetCurrentWindow().glfw_GetWindow(), GLFW_KEY_N))
    {
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }


    glDrawArrays(GL_POINTS, 0, p_sys->GetAlivePartCount());
    glUseProgram(0);
    vao.unBind();
    texture->unBind();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

  }
}

void ParticleRenderer::ComputeRender()
{
  if (p_sys->GetAlivePartCount() > 0)
  {
    computeshader->Bind();
    c_vao->Bind();
    Posbuffer->BindBufferBase(0);
    VelBuffer->BindBufferBase(1);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    int workingGroups = p_sys->GetAlivePartCount() / 16;
    computeshader->Dispatch_Compute(workingGroups + 2, 1, 1);
    computeshader->unBind();
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    glUseProgram(ParticleProgram);
    vao.Bind();
    texture->TexBind();
    matrix = g_GraphicsSys->GetCurrentCamera().getProjectionMatrix() * g_GraphicsSys->GetCurrentCamera().getWorldToViewMatrix();
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, p_sys->GetAlivePartCount() * 3 * sizeof(float), (void*)p_sys->GetPositionData());
    GLuint posAttrib = glGetAttribLocation(ParticleProgram, "position");
    glBindBuffer(GL_ARRAY_BUFFER, Posbuffer->Get_POS());
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
    glBufferSubData(GL_ARRAY_BUFFER, 0, p_sys->GetAlivePartCount() * 4 * sizeof(float), (void*)p_sys->GetColorData());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPointSize(10);
    glDrawArrays(GL_POINTS, 0, p_sys->GetAlivePartCount());

    glUseProgram(0);
    vao.unBind();
    texture->unBind();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

  }
}

