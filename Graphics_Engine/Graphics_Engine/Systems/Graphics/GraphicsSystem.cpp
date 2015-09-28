 #include "GraphicsSystem.h"
#include "Buffer_Manager.h"
#include "LoadShader.h"
//#include "ShapeGenerator.h"
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

  //Object* cube = ObjectManager::CreateObject(Arrow_Mesh, glm::vec3(-2.0f, 0.0f, -3.0f), glm::vec3(-2.0f, 0.0f, -3.0f), 21.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  //Object* arrow = ObjectManager::CreateObject(Plane_Mesh, glm::vec3(2.0f, 0.0f, -3.75f), glm::vec3(-2.0f, 0.0f, -3.0f), 30.5f, glm::vec3(0.0f, 1.0f, 0.0f));
  Object* sphere = ObjectManager::CreateMeshObject("Models/Pyro.obj", glm::vec3(-2.0f, 0.0f, -3.0f), glm::vec3(-2.0f, 0.0f, -3.0f), 21.0f, glm::vec3(1.0f, 0.0f, 0.0f));

  Debug_Draw = false;
}

void GraphicsSystem::Update(double dt)
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

 // SetBackgroundColor(0.5f, 0.1f, 0.1f);

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


