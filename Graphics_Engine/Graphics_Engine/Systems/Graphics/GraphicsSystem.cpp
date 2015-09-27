 #include "GraphicsSystem.h"
#include "Buffer_Manager.h"
#include "LoadShader.h"
//#include "ShapeGenerator.h"
#include "Math_Headers.h"
#include "../Input/InputSystem.h"
#include "Object.h"



GraphicsSystem* g_GraphicsSys;


GLuint FullTransformMatrixLocation;
Object* cube;
Object* arrow;
VBO* TransformationMatrixVBO;
//GLuint MatUniform;
//GLuint SimpleProgram;

void GraphicsSystem::Init(void)
{
  Current_Window.glfw_Init();
  glfwSetMouseButtonCallback(Current_Window.glfw_GetWindow(), g_InputSys->mousePressEvent);
  glfwSetCursorPosCallback(Current_Window.glfw_GetWindow(), g_InputSys->mouseMoveEvent);

  g_GraphicsSys = this;

  glEnable(GL_DEPTH_TEST);
 
  Mesh::Init_Mesh_Shader();

  cube = new Object(Cube_Mesh, glm::vec3(-2.0f, 0.0f, -3.0f));
  arrow = new Object(Plane_Mesh);

 // TransformationMatrixVBO = new VBO(sizeof(glm::mat4) * 2 , 0);
  //SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  //MatUniform = glGetUniformLocation(SimpleProgram, "FullTransformMatrix");
  //Specify_Attributes_Simple(SimpleProgram);



  Debug_Draw = false;
}

void GraphicsSystem::Update(double dt)
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

 // SetBackgroundColor(0.5f, 0.1f, 0.1f);

  glViewport(0, 0, Current_Window.GetWidth(), Current_Window.GetHeight()); // Still need to do it

  // If u do translation before rotation then it rotates around the center of the world
  // instead of around the center of the object

  // projection * translation * rotation
  //glUseProgram(SimpleProgram);
  //cube->mesh->vao->Bind();
  cube->Draw();
  //Cube

  //glm::mat4 matrix = My_Camera.getProjectionMatrix() * My_Camera.getWorldToViewMatrix() * glm::translate(glm::vec3(-2.0f, 0.0f, -3.0f)) * glm::rotate(21.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  //glUniformMatrix4fv(MatUniform, 1, GL_FALSE, &matrix[0][0]);
  //glDrawElements(GL_TRIANGLES, cube->mesh->indices.size(), GL_UNSIGNED_SHORT, 0);

  //matrix = projectionMatrix * My_Camera.getWorldToViewMatrix() * glm::translate(glm::vec3(2.0f, 0.0f, -3.75f)) * glm::rotate(30.5f, glm::vec3(0.0f, 1.0f, 0.0f));
  //glUniformMatrix4fv(MatUniform, 1, GL_FALSE, &matrix[0][0]);
  //glDrawElements(GL_TRIANGLES, cube->mesh->indices.size(), GL_UNSIGNED_SHORT, 0);

  ////Arrow
  //arrow->mesh->vao->Bind();
  //glm::mat4 arrowmatrix = glm::translate(0.0f, 0.0f, -3.0f);
  //matrix = projectionMatrix * My_Camera.getWorldToViewMatrix() * arrowmatrix;
  //glUniformMatrix4fv(MatUniform, 1, GL_FALSE, &matrix[0][0]);
  //glDrawElements(GL_TRIANGLES, arrow->mesh->indices.size(), GL_UNSIGNED_SHORT, 0);
  //glm::mat4 fullTransforms[] =
  //{
  //  projectionMatrix * My_Camera.getWorldToViewMatrix() * glm::translate(glm::vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(21.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
  //  projectionMatrix * My_Camera.getWorldToViewMatrix() * glm::translate(glm::vec3(1.0f, 0.0f, -3.75f)) * glm::rotate(30.5f, glm::vec3(0.0f, 1.0f, 0.0f)),
  //};

  //TransformationMatrixVBO->Add_Buffer_Data(sizeof(fullTransforms), fullTransforms);
  //TransformationMatrixVBO->Bind();

  //MyObject.ebo->Bind();

  //if (!Debug_Draw)
  //  glDrawElementsInstanced(GL_TRIANGLES, MyObject.NumIndices, GL_UNSIGNED_SHORT, 0, 2);
  //else
  //  glDrawElementsInstanced(GL_LINE_STRIP, MyObject.NumIndices, GL_UNSIGNED_SHORT, 0, 2);

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


