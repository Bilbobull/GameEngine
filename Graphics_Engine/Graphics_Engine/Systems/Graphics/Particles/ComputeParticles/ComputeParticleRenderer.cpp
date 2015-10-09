#include "ComputeParticleRenderer.h"
#include "../../GraphicsSystem.h"
#include "../../Camera.h"
#include "../../LoadShader.h"

namespace ComputeShaders
{

  static bool borderEnabled = true;
  static bool colorFade = true;
  static bool pause = false;

  static bool draw = true;
  static double cursorX = 0, cursorY = 0;
  static double prevcursorX = 0, prevcursorY = 0;
  static GLuint fbo, rbo;
  static float decayRate = 2.0f;
  static float breathRate = 0.01f;
  static float offset = 0.75f;
  static float t = 1.0f;

  static float timepassed = 0.0f;

  static float random(float fMin, float fMax)
  {
    float fRandNum = (float)rand() / RAND_MAX;
    return fMin + (fMax - fMin) * fRandNum;
  }


  CS_Renderer::CS_Renderer()
  {
    NumParticles = 100000;
    ParticleSize = 2;
    speedMultiplier = 0.01f;
    srand((unsigned)time(NULL));
    color[0] = 255;
    color[1] = 80;
    color[2] = 0;
    color[3] = 0.05f;
  }

  CS_Renderer::~CS_Renderer()
  {
    delete Posbuffer, VelBuffer, vao;
    Posbuffer = nullptr;
    VelBuffer = nullptr;
    vao = nullptr;
  }

  void CS_Renderer::Initialize()
  {
    CreateBuffers();
    InitShaders();
    LoadTexture();
  }

  void CS_Renderer::Draw()
  {
    Mix_Colors();
    vao->Bind();
    Posbuffer->BindBufferBase(0);
    VelBuffer->BindBufferBase(1);
    AccBuffer->BindBufferBase(2);

    double frameTimeStart = glfwGetTime();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Run compute shader

    //double cursorX, cursorY;
    int windowWidth, windowHeight;
    glfwGetCursorPos(g_GraphicsSys->GetCurrentWindow().glfw_GetWindow(), &cursorX, &cursorY);
    glfwGetWindowSize(g_GraphicsSys->GetCurrentWindow().glfw_GetWindow(), &windowWidth, &windowHeight);

    destPos.x = (float)(cursorX / (windowWidth)-0.5f) * 2.0f;
    destPos.y = (float)((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;

    computeshader->Use();
    computeshader->uni1f("deltaT", 10 * speedMultiplier * (pause ? 0 : 1));
    computeshader->uni3f("destPos", destPos.x, destPos.y, 0);
    computeshader->uni2f("vpDim", 1, 1);
    computeshader->uni1i("borderClamp", 1);
    computeshader->uni1f("et", (float)glfwGetTime());

    int workingGroups = NumParticles / 16;

    computeshader->Dispatch_Compute(workingGroups + 2, 1, 1);

    computeshader->Disable();

    //// Set memory barrier on per vertex base to make sure we get what was written by the compute shaders
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    //// Render scene

    glUseProgram(shader);
    GLuint colLocation = glGetUniformLocation(shader, "Color");
    glUniform4f(colLocation, colVec.x, colVec.y, colVec.z, 1.0f);
    glEnable(GL_PROGRAM_POINT_SIZE);
    texture->TexBind();
    glUniform1i(glGetUniformLocation(shader, "image"), 0);
    GLuint posAttrib = glGetAttribLocation(shader, "position");
    glm::mat4 mvp = g_GraphicsSys->GetCurrentCamera().getProjectionMatrix() * g_GraphicsSys->GetCurrentCamera().getWorldToViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
    glBindBuffer(GL_ARRAY_BUFFER, Posbuffer->Get_POS());
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    glPointSize(ParticleSize);
    glDrawArrays(GL_POINTS, 0, NumParticles);

    texture->unBind();
    glUseProgram(0);

    DeltaFrame = (float)(glfwGetTime() - frameTimeStart);


    //vao->unBind();
  }


  void CS_Renderer::InitShaders()
  {
    shader = LoadShaders("Vertex.vs.glsl", "Fragment.frag.glsl");
    computeshader = LoadComputeShader ("Systems/Graphics/Shaders/MouseParticleEffect.cs.glsl");
  }


  void CS_Renderer::CreateBuffers()
  {
    // OpenGL 3.3+
    // Create a VAO and never use it again!!!
    vao = new VAO();
    Posbuffer = new SSBO(NumParticles * sizeof(glm::vec4));

    // Fill
    ResetPosition();
    // Bind buffer to target index 0
    Posbuffer->BindBufferBase(0);


    VelBuffer = new SSBO(NumParticles * sizeof(glm::vec4));
    ResetVelocity();
    // Bind buffer to target index 1
    VelBuffer->BindBufferBase(1);

    AccBuffer = new SSBO(NumParticles * sizeof(glm::vec4));
    ResetAcceleration();
    AccBuffer->BindBufferBase(2);

    vao->unBind();
  }

  void CS_Renderer::ResetBuffers()
  {
    Posbuffer->BindBuffer();
    ResetPosition();
    VelBuffer->BindBuffer();
    ResetVelocity();
  }

  void CS_Renderer::LoadTexture()
  {
    texture = new Texture("Particle");
  }

  void CS_Renderer::ResetPosition()
  {
    // Reset to mouse cursor pos
    //double cursorX, cursorY;
    int windowWidth, windowHeight;
    glfwPollEvents();
    glfwGetCursorPos(g_GraphicsSys->GetCurrentWindow().glfw_GetWindow(), &cursorX, &cursorY);
    glfwGetWindowSize(g_GraphicsSys->GetCurrentWindow().glfw_GetWindow(), &windowWidth, &windowHeight);

    destPos.x = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;
    destPos.y = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;


    glm::vec4* verticesPos = (glm::vec4*)Posbuffer->MapBufferRange<glm::vec4>(0, NumParticles);
    for (int i = 0; i < NumParticles; i++)
    {
      float rnd = (float)rand() / (float)(RAND_MAX);
      float rndVal = (float)rand() / (float)(RAND_MAX / (360.0f * 3.14f * 2.0f));
      float rndRad = (float)rand() / (float)(RAND_MAX)* radiusMultiplier;
      radius = rndRad;
      verticesPos[i].x = destPos.x + cos(rndVal) * rndRad;
      verticesPos[i].y = destPos.y + sin(rndVal) * rndRad;
      verticesPos[i].z = 0.0f;
      verticesPos[i].w = 1.0f;

      verticesPos[i] = glm::linearRand(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
    }
    Posbuffer->UnMapBuffer();
  }


  void CS_Renderer::ResetVelocity()
  {
    glm::vec4* verticesVel = VelBuffer->MapBufferRange<glm::vec4>(0, NumParticles);
    for (int i = 0; i < NumParticles; i++)
    {
      verticesVel[i].x = 0.0f;
      verticesVel[i].y = 0.0f;
      verticesVel[i].z = 0.0f;
      verticesVel[i].w = 1.0f;
    }
    VelBuffer->UnMapBuffer();
  }

  void CS_Renderer::ResetAcceleration()
  {
    glm::vec4* verticesAcc = AccBuffer->MapBufferRange<glm::vec4>(0, NumParticles);
    for (int i = 0; i < NumParticles; i++)
    {
      verticesAcc[i].x = 0.0f;
      verticesAcc[i].y = 0.0f;
      verticesAcc[i].z = 0.0f;
      verticesAcc[i].w = 1.0f;
    }
    AccBuffer->UnMapBuffer();
  }

  static glm::vec3 c1 = glm::linearRand(glm::vec3 (0.0f), glm::vec3 (1.0f));
  static glm::vec3 c2 = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));;
  static float timer = 10.0f;
  float colorChangeTimer = 0.0f;

  void CS_Renderer::Mix_Colors()
  {
    if (colorFade)
    {
      colorChangeTimer += DeltaFrame;

      if (colorChangeTimer > timer)
      {
        c1 = c2;
        c2 = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f, 0.5f, 0.25f));
        colorChangeTimer = 0.0f;
      }

      colVec = glm::mix(c1, c2, colorChangeTimer / timer);

    }
    else
    {
      color[0] = 255.0f;
      color[1] = 64.0f;
      color[2] = 0.0f;
    }
  }

}