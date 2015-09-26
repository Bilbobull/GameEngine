 #include "GraphicsSystem.h"
#include "Buffer_Manager.h"
#include "LoadShader.h"
//#include "ShapeGenerator.h"
#include "Math_Headers.h"
#include "../Input/InputSystem.h"
#include "Object.h"


GraphicsSystem* g_GraphicsSys;


GLuint SimpleProgram;
GLuint FullTransformMatrixLocation;
Object* cube;
Object* arrow;
VBO* TransformationMatrixVBO;
GLuint MatUniform;


void GraphicsSystem::Specify_Attributes_Simple(GLuint program)
{
  cube->mesh->vao->Bind();
  cube->mesh->vbo->Bind();
  cube->mesh->ebo->Bind();
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 6, 0);
  //glVertexAttrib3f(1, 1, 0, 1); // U can disable the VertexAttribArray and set a static color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 6, (char*)(sizeof(GL_FLOAT)* 3));
  cube->mesh->vao->unBind();
  cube->mesh->vbo->unBind();
  cube->mesh->ebo->unBind();

  arrow->mesh->vao->Bind();
  arrow->mesh->vbo->Bind();
  arrow->mesh->ebo->Bind();
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 6, 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 6, (char*)(sizeof(GL_FLOAT)* 3));
  arrow->mesh->vao->unBind();
  arrow->mesh->vbo->unBind();
  arrow->mesh->ebo->unBind();

  //TransformationMatrixVBO->Bind();
  //for (unsigned i = 0; i < 4; ++i)
  //{
  //  glEnableVertexAttribArray(2 + i);
  //  glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * (i * 4)));
  //  glVertexAttribDivisor(2 + i, 1);
  //}
  //TransformationMatrixVBO->unBind();
}



void GraphicsSystem::Init(void)
{
  Current_Window.glfw_Init();
  glfwSetMouseButtonCallback(Current_Window.glfw_GetWindow(), g_InputSys->mousePressEvent);
  glfwSetCursorPosCallback(Current_Window.glfw_GetWindow(), g_InputSys->mouseMoveEvent);

  g_GraphicsSys = this;

  glEnable(GL_DEPTH_TEST);

  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  cube = new Object();
  arrow = new Object(Plane_Mesh);

 // TransformationMatrixVBO = new VBO(sizeof(glm::mat4) * 2 , 0);

  Specify_Attributes_Simple(SimpleProgram);
  MatUniform = glGetUniformLocation(SimpleProgram, "FullTransformMatrix");


  Debug_Draw = false;
}

void GraphicsSystem::Update(double dt)
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

 // SetBackgroundColor(0.5f, 0.1f, 0.1f);

  glViewport(0, 0, Current_Window.GetWidth(), Current_Window.GetHeight()); // Still need to do it

  glUseProgram(SimpleProgram);

  // If u do translation before rotation then it rotates around the center of the world
  // instead of around the center of the object

  // projection * translation * rotation
  cube->mesh->vao->Bind();
  glm::mat4 projectionMatrix = glm::perspective(90.0f, ((float)Current_Window.GetWidth()) / Current_Window.GetHeight(), 0.1f, 20.0f);

  //Cube

  glm::mat4 matrix = projectionMatrix * My_Camera.getWorldToViewMatrix() * glm::translate(glm::vec3(-2.0f, 0.0f, -3.0f)) * glm::rotate(21.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(MatUniform, 1, GL_FALSE, &matrix[0][0]);
  glDrawElements(GL_TRIANGLES, cube->mesh->indices.size(), GL_UNSIGNED_SHORT, 0);

  matrix = projectionMatrix * My_Camera.getWorldToViewMatrix() * glm::translate(glm::vec3(2.0f, 0.0f, -3.75f)) * glm::rotate(30.5f, glm::vec3(0.0f, 1.0f, 0.0f));
  glUniformMatrix4fv(MatUniform, 1, GL_FALSE, &matrix[0][0]);
  glDrawElements(GL_TRIANGLES, cube->mesh->indices.size(), GL_UNSIGNED_SHORT, 0);

  //Arrow
  arrow->mesh->vao->Bind();
  glm::mat4 arrowmatrix = glm::translate(0.0f, 0.0f, -3.0f);
  matrix = projectionMatrix * My_Camera.getWorldToViewMatrix() * arrowmatrix;
  glUniformMatrix4fv(MatUniform, 1, GL_FALSE, &matrix[0][0]);
  glDrawElements(GL_TRIANGLES, arrow->mesh->indices.size(), GL_UNSIGNED_SHORT, 0);
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


