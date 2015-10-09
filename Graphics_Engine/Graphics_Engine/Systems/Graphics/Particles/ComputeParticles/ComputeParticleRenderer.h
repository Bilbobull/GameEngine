/******************************************************************************/
/*!
\file   CLParticleRenderer.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _CL_PARTICLE_RENDERER
#define _CL_PARTICLE_RENDERER

#include "../../Graphics_Headers.h"
#include "../../Math_Headers.h"
#include "ShaderStorageBufferObject.h"
#include "../../Buffer_Manager.h"
#include "ComputeShader.h"
#include "../../Texture.h"
#include <string>

#define ONE_MILLION 1000000

namespace ComputeShaders
{

  class CS_Renderer
  {
  public:

    CS_Renderer();
    CS_Renderer(const CS_Renderer&) = delete;
    CS_Renderer& operator =(const CS_Renderer&) = delete;
    virtual ~CS_Renderer();

    virtual void Initialize();
    virtual void Draw();

    void InitShaders();
    void CreateBuffers();
    void ResetBuffers();
    void LoadTexture();
    void SetMousePos(double, double);
    void ResetPosition();
    void ResetVelocity();
    void ResetAcceleration();
    void Mix_Colors();

    const static std::string Name;

  private:
    SSBO* Posbuffer, *VelBuffer, *AccBuffer;
    VAO* vao;
    ComputeShader* computeshader;
    Texture* texture;
    float radius;
    float radiusMultiplier;
    float minAlpha;
    int NumParticles;
    float ParticleSize;
    float DeltaFrame = 0.0f;
    float speedMultiplier = 0.15f;
    glm::vec4 color;
    glm::vec3 colVec;
    float colorChangeLength;
    glm::vec2 destPos;
    GLuint shader;
  };
}

#endif