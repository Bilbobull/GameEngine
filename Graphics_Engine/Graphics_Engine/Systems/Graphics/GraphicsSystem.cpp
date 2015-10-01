 #include "GraphicsSystem.h"
#include "Buffer_Manager.h"
#include "LoadShader.h"
#include "Math_Headers.h"
#include "../Input/InputSystem.h"
#include "Object.h"
#include "ObjectManager.h"



GraphicsSystem* g_GraphicsSys;

void GraphicsSystem::Init(void)
{
  Current_Window.glfw_Init();
  glfwSetMouseButtonCallback(Current_Window.glfw_GetWindow(), g_InputSys->mousePressEvent);
  glfwSetCursorPosCallback(Current_Window.glfw_GetWindow(), g_InputSys->mouseMoveEvent);

  g_GraphicsSys = this;

  glEnable(GL_DEPTH_TEST);
 
  Mesh::Init_Mesh_Shader();

  //Object* arrow = ObjectManager::CreateObject(glm::vec3(-2.0f, 0.0f, -3.0f), glm::vec3(-2.0f, 0.0f, -3.0f), 50.0f, glm::vec3(1.0f, 0.0f, 0.0f), " ", Arrow_Mesh );
  //Object* cube = ObjectManager::CreateObject(glm::vec3(-2.0f, 0.0f, -10.0f), glm::vec3(-2.0f, 0.0f, -3.0f), 50.0f, glm::vec3(1.0f, 0.0f, 0.0f));

  Object* ironman = ObjectManager::CreateObject(glm::vec3(2.0f, 0.5f, -3.0f), glm::vec3(-2.0f, 0.0f, -3.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), "Pyro");
  Object* pyro = ObjectManager::CreateObject(glm::vec3(-4.0f, 0.5f, -3.0f), glm::vec3(-2.0f, 0.0f, -3.0f), 0.0f, glm::vec3(1.0f, 1.0f, 0.0f), "Ironman");

}

void GraphicsSystem::Update(double dt)
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glViewport(0, 0, Current_Window.GetWidth(), Current_Window.GetHeight()); // Still need to do it

  auto objectList = ObjectManager::GetObjectList();
  for (auto it : objectList)
  {
    it->Draw();
  }
  
  glfwSwapBuffers(Current_Window.glfw_GetWindow());
}

void GraphicsSystem::Free(void)
{
  ObjectManager::Free();
}

Window& GraphicsSystem::GetCurrentWindow(void)
{
  return Current_Window;
}

void GraphicsSystem::SetBackgroundColor(float r /*= 1.0f*/, float g /*= 1.0f*/, float b /*= 1.0f*/, float a /*= 1.0f*/)
{
  glClearColor(r, g, b, a);
}

Camera& GraphicsSystem::GetCurrentCamera(void)
{
  return My_Camera;
}

glm::vec3 GraphicsSystem::Random_Color(void)
{
  glm::vec3 color;
  color.x = rand() / (float)RAND_MAX;
  color.y = rand() / (float)RAND_MAX;
  color.z = rand() / (float)RAND_MAX;
  return color;
}


