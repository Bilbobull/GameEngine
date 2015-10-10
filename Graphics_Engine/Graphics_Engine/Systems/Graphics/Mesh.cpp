#include "Mesh.h"
#include "LoadShader.h"
#include "GraphicsSystem.h"
#include "Light.h"

GLuint SimpleProgram;
GLuint SimpleMatUniform;

GLuint ModelProgram;
GLuint ModelModelToWorldUniform;
GLuint ModelWorldToViewUniform;
GLuint ModelViewToProjectionUniform;
GLuint DiffuseTextureUniform;
GLuint NormalTextureUniform;

GLuint LightDirectionArrayUniform;
GLuint LightAmbientArrayUniform;
GLuint LightDiffuseArrayUniform;
GLuint LightSpecularArrayUniform;
GLuint CamPosUniform;

GLuint MaterialAmbientUniform;
GLuint MaterialDiffuseUniform;
GLuint MaterialSpecularUniform;
GLuint ShininessUniform;

GLuint LightCountUniform;

static int Shininess = 0.8;


void Mesh::Init_Mesh_Shader(void)
{
  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  SimpleMatUniform = glGetUniformLocation(SimpleProgram, "FullTransformMatrix");



  ModelProgram = LoadShaders("ModelVertexShader.glsl", "ModelFragmentShader.glsl");

  DiffuseTextureUniform = glGetUniformLocation(ModelProgram, "Texture");
  NormalTextureUniform = glGetUniformLocation(ModelProgram, "normalTexture");

  LightDirectionArrayUniform = glGetUniformLocation(ModelProgram, "lightDirections");
  LightAmbientArrayUniform = glGetUniformLocation(ModelProgram, "lightAmbients");
  LightDiffuseArrayUniform = glGetUniformLocation(ModelProgram, "lightDifuses");
  LightSpecularArrayUniform = glGetUniformLocation(ModelProgram, "lightSpeculars");
  LightCountUniform = glGetUniformLocation(ModelProgram, "LightCount");
  MaterialDiffuseUniform = glGetUniformLocation(ModelProgram, "MaterialValues.diffuse");
  MaterialAmbientUniform = glGetUniformLocation(ModelProgram, "MaterialValues.ambient");
  MaterialSpecularUniform = glGetUniformLocation(ModelProgram, "MaterialValues.specular");
  ModelModelToWorldUniform = glGetUniformLocation(ModelProgram, "ModelToWorldMatrix");
  ModelWorldToViewUniform = glGetUniformLocation(ModelProgram, "WorldToViewMatrix");
  ModelViewToProjectionUniform = glGetUniformLocation(ModelProgram, "ViewToProjectionMatrix");
  CamPosUniform = glGetUniformLocation(ModelProgram, "CameraPosition");
  ShininessUniform = glGetUniformLocation(ModelProgram, "Shininess");
}



void Mesh::Draw(glm::mat4 ModelToWorld, glm::mat4 WorldToView, glm::mat4 ViewToProjection)
{



  glm::mat4 fullmatrix = ViewToProjection  * WorldToView * ModelToWorld;

  switch (type)
  {
  case Models:
  {
    if (g_GraphicsSys->GetDebugDraw() == true || texture == nullptr)
    {
      Debug_Draw(fullmatrix);
    }

    else
    {
      glClearColor(0, 0, 0, 0);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glUseProgram(ModelProgram);

      glActiveTexture(GL_TEXTURE0);
      texture->TexBind();
      glUniform1i(DiffuseTextureUniform, 0);

      glActiveTexture(GL_TEXTURE1);
      texture->NorBind();
      glUniform1i(NormalTextureUniform, 1);

      glActiveTexture(GL_TEXTURE0);

      vao->Bind();
      glUniform4fv(LightDirectionArrayUniform, ARRAYSIZE(Lightdirection), glm::value_ptr(Lightdirection[0]));
      glUniform4fv(LightAmbientArrayUniform, ARRAYSIZE(Lightambient), glm::value_ptr(Lightambient[0]));
      glUniform4fv(LightDiffuseArrayUniform, ARRAYSIZE(Lightdiffuse), glm::value_ptr(Lightdiffuse[0]));
      glUniform4fv(LightSpecularArrayUniform, ARRAYSIZE(Lightspecular), glm::value_ptr(Lightspecular[0]));
      glUniform4fv(MaterialDiffuseUniform, 1, glm::value_ptr(MaterialVal.diffuse));
      glUniform4fv(MaterialAmbientUniform, 1, glm::value_ptr(MaterialVal.ambient));
      glUniform4fv(MaterialSpecularUniform, 1, glm::value_ptr(MaterialVal.specular));
      glUniform1i(LightCountUniform, LightNum);
      glUniform3fv(CamPosUniform, 1, glm::value_ptr(g_GraphicsSys->GetCurrentCamera().GetPosition()));
      glUniform1i(ShininessUniform, Shininess);

      glUniformMatrix4fv(ModelModelToWorldUniform, 1, GL_FALSE, &ModelToWorld[0][0]);
      glUniformMatrix4fv(ModelWorldToViewUniform, 1, GL_FALSE, &WorldToView[0][0]);
      glUniformMatrix4fv(ModelViewToProjectionUniform, 1, GL_FALSE, &ViewToProjection[0][0]);
      glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_SHORT, 0);
      texture->unBind();
    }


    break;
  }
  case Shape:
    if (g_GraphicsSys->GetDebugDraw())
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glUseProgram(SimpleProgram);
    vao->Bind();
    glUniformMatrix4fv(SimpleMatUniform, 1, GL_FALSE, &fullmatrix[0][0]);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_SHORT, 0);
  }

}

void Mesh::Debug_Draw(glm::mat4 matrix)
{
  glClearColor(0.4f, 0.4f, 0.4f, 0.4f);


  glUseProgram(SimpleProgram);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  vao->Bind();
  glUniformMatrix4fv(SimpleMatUniform, 1, GL_FALSE, &matrix[0][0]);
  glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_SHORT, 0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if (g_GraphicsSys->GetVertexNormalDraw())
  {
    glm::vec3 pos, nor;

    for (unsigned i = 0; i < vertices.size(); ++i){

      pos = glm::vec3(vertices.at(i).position.x, vertices.at(i).position.y, vertices.at(i).position.z);
      nor = glm::vec3(vertices.at(i).normal.x / 5.0f, vertices.at(i).normal.y / 5.0f, vertices.at(i).normal.z / 5.0f);
      glLineWidth(2.5);

      glBegin(GL_LINES);
      glVertex3f(pos.x, pos.y, pos.z);
      glVertex3f(pos.x + nor.x, pos.y + nor.y, pos.z + nor.z);
      glEnd();
    }
  }

  if (g_GraphicsSys->GetFaceNormalDraw())
  {
    for (unsigned i = 0; i < triangles.size(); i += 3)
    {
      glm::vec3 nor = glm::cross((vertices.at(triangles.at(i + 1)).position - vertices.at(triangles.at(i)).position),
                                 (vertices.at(triangles.at(i + 2)).position - vertices.at(triangles.at(i)).position));

      //nor.x *= 10.0f;
      //nor.y *= 10.0f;
      //nor.z *= 10.0f;

      glm::vec3 pos = glm::vec3(vertices.at(triangles.at(i)).position + vertices.at(triangles.at(i + 1)).position + vertices.at(triangles.at(i + 2)).position);
      pos.x /= 3.0f;
      pos.y /= 3.0f;
      pos.z /= 3.0f;

      glBegin(GL_LINES);
      glVertex3f(pos.x, pos.y, pos.z);
      glVertex3f(pos.x + nor.x, pos.y + nor.y, pos.z + nor.z);
      glEnd();
    }
  }
}