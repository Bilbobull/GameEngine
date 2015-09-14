#include "GraphicsSystem.h"
#include "Buffer_Manager.h"
#include "LoadShader.h"
#include "ShapeGenerator.h"
#include "Math_Headers.h"

GraphicsSystem* g_GraphicsSys;


GLuint SimpleProgram;
GLuint FullTransformMatrixLocation;
Shape cube;
VBO* TransformationMatrixVBO;


void GraphicsSystem::Specify_Attributes_Simple(GLuint simpleProgram)
{
  cube.vbo->Bind();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char*)(sizeof(GL_FLOAT) * 3));

  TransformationMatrixVBO->Bind();
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 0));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 4));

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 8));

  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 12));

  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);
}

void GraphicsSystem::Init(void)
{
  Current_Window.glfw_Init();
  g_GraphicsSys = this;

  glEnable(GL_DEPTH_TEST);

  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  cube = ShapeGenerator::makeCube();
  cube.vbo->Bind();
  cube.ebo->Bind();

  // If u do translation before rotation then it rotates around the center of the world
  // instead of around the center of the object

  // projection * translation * rotation
  glm::mat4 projectionMatrix = glm::perspective(90.0f, ((float)Current_Window.GetWidth()) / Current_Window.GetHeight(), 0.1f, 10.0f);
  glm::mat4 fullTransforms[] =
  {
    projectionMatrix * glm::translate(glm::vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(21.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
    projectionMatrix * glm::translate(glm::vec3(1.0f, 0.0f, -3.75f)) * glm::rotate(30.5f, glm::vec3(0.0f, 1.0f, 0.0f)),

  };
  TransformationMatrixVBO = new VBO(sizeof(fullTransforms), fullTransforms);

  Specify_Attributes_Simple(SimpleProgram);

}

void GraphicsSystem::Update(double dt)
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  // SetBackgroundColor(0.5f, 0.1f, 0.1f);

  glViewport(0, 0, Current_Window.GetWidth(), Current_Window.GetHeight()); // Still need to do it

  glUseProgram(SimpleProgram);

  glDrawElementsInstanced(GL_TRIANGLES, cube.NumIndices, GL_UNSIGNED_SHORT, 0, 2 );

  glfwSwapBuffers(Current_Window.glfw_GetWindow());
}

void GraphicsSystem::Free(void)
{
  
}

Window& GraphicsSystem::GetCurrentWindow(void)
{
  return Current_Window;
}

void GraphicsSystem::SetBackgroundColor(float r /*= 1.0f*/, float g /*= 1.0f*/, float b /*= 1.0f*/, float a /*= 1.0f*/)
{
  glClearColor(r, g, b, a);
}


