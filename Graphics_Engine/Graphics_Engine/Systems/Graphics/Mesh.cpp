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
GLuint  LightTypeArrayUniform;
GLuint LightPositionArrayUniform;
GLuint LightDirectionArrayUniform;
GLuint LightAmbientArrayUniform;
GLuint LightDiffuseArrayUniform;
GLuint LightSpecularArrayUniform;
GLuint LightEmisiveArrayUniform;
GLuint LightInnerUniform;
GLuint LightOuterUniform;
GLuint LightFalloffUniform;
GLuint TexturesUniform;

GLuint CamPosUniform;

GLuint GlobalAmbientUniform;

GLuint MaterialAmbientUniform;
GLuint MaterialDiffuseUniform;
GLuint MaterialSpecularUniform;

GLuint ShininessUniform;

GLuint LightCountUniform;

GLuint DistanceAttConstUniform;
GLuint DistanceAttBoolUniform;

GLuint NearPlaneLocation;
GLuint FarPlaneLocation;
GLuint AtmosphericAttBoolUniform;
GLuint AtmosphericAttIntesityUniform;


void Mesh::Init_Mesh_Shader(void)
{
  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  SimpleMatUniform = glGetUniformLocation(SimpleProgram, "FullTransformMatrix");


#pragma region PerFragment

  ModelProgram = LoadShaders("ModelVertexShader.glsl", "ModelFragmentShader.glsl");

  DiffuseTextureUniform = glGetUniformLocation(ModelProgram, "Texture");
  NormalTextureUniform = glGetUniformLocation(ModelProgram, "normalTexture");

  GlobalAmbientUniform = glGetUniformLocation(ModelProgram, "globalAmbient");
  LightTypeArrayUniform = glGetUniformLocation(ModelProgram, "lightTypes");
  LightPositionArrayUniform = glGetUniformLocation(ModelProgram, "lightPositions");
  LightDirectionArrayUniform = glGetUniformLocation(ModelProgram, "lightDirections");
  LightAmbientArrayUniform = glGetUniformLocation(ModelProgram, "lightAmbients");
  LightDiffuseArrayUniform = glGetUniformLocation(ModelProgram, "lightDiffuses");
  LightSpecularArrayUniform = glGetUniformLocation(ModelProgram, "lightSpeculars");
  LightEmisiveArrayUniform = glGetUniformLocation(ModelProgram, "lightEmisives");
  LightInnerUniform = glGetUniformLocation(ModelProgram, "lightInners");
  LightOuterUniform = glGetUniformLocation(ModelProgram, "lightOuters");
  LightFalloffUniform = glGetUniformLocation(ModelProgram, "lightFalloffs");
  LightCountUniform = glGetUniformLocation(ModelProgram, "LightCount");
  MaterialDiffuseUniform = glGetUniformLocation(ModelProgram, "MaterialValues.diffuse");
  MaterialAmbientUniform = glGetUniformLocation(ModelProgram, "MaterialValues.ambient");
  MaterialSpecularUniform = glGetUniformLocation(ModelProgram, "MaterialValues.specular");
  ModelModelToWorldUniform = glGetUniformLocation(ModelProgram, "ModelToWorldMatrix");
  ModelWorldToViewUniform = glGetUniformLocation(ModelProgram, "WorldToViewMatrix");
  ModelViewToProjectionUniform = glGetUniformLocation(ModelProgram, "ViewToProjectionMatrix");
  CamPosUniform = glGetUniformLocation(ModelProgram, "CameraPosition");
  ShininessUniform = glGetUniformLocation(ModelProgram, "Shininess");
  DistanceAttConstUniform = glGetUniformLocation(ModelProgram, "DistanceAttConstants");
  DistanceAttBoolUniform = glGetUniformLocation(ModelProgram, "DistanceAttBool");
  NearPlaneLocation = glGetUniformLocation(ModelProgram, "NearPlane");
  FarPlaneLocation = glGetUniformLocation(ModelProgram, "FarPlane");
  AtmosphericAttBoolUniform = glGetUniformLocation(ModelProgram, "AtmosphericAttBool");
  AtmosphericAttIntesityUniform = glGetUniformLocation(ModelProgram, "AtmosphericIntensity");
  TexturesUniform = glGetUniformLocation(ModelProgram, "Textures");

#pragma endregion


#pragma region PerVertex



#pragma endregion
}



void Mesh::Draw(glm::mat4 ModelToWorld, glm::mat4 WorldToView, glm::mat4 ViewToProjection, glm::mat4 scale)
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
      glUniform1iv(LightTypeArrayUniform, ARRAYSIZE(Lighttype), &Lighttype[0]);
      glUniform4fv(LightPositionArrayUniform, ARRAYSIZE(Lightposition), glm::value_ptr(Lightposition[0]));
      glUniform4fv(LightDirectionArrayUniform, ARRAYSIZE(Lightdirection), glm::value_ptr(Lightdirection[0]));
      glUniform4fv(LightAmbientArrayUniform, ARRAYSIZE(Lightambient), glm::value_ptr(Lightambient[0]));
      glUniform4fv(LightDiffuseArrayUniform, ARRAYSIZE(Lightdiffuse), glm::value_ptr(Lightdiffuse[0]));
      glUniform4fv(LightSpecularArrayUniform, ARRAYSIZE(Lightspecular), glm::value_ptr(Lightspecular[0]));
      glUniform4fv(LightEmisiveArrayUniform, ARRAYSIZE(Lightemisive), glm::value_ptr(Lightemisive[0]));
      glUniform1fv(LightInnerUniform, ARRAYSIZE(Lightinner), &Lightinner[0]);
      glUniform1fv(LightTypeArrayUniform, ARRAYSIZE(Lightouter), &Lightouter[0]);
      glUniform1fv(LightTypeArrayUniform, ARRAYSIZE(Lightfalloff), &Lightfalloff[0]);

      glUniform4fv(MaterialDiffuseUniform, 1, glm::value_ptr(MaterialVal.diffuse));
      glUniform4fv(MaterialAmbientUniform, 1, glm::value_ptr(MaterialVal.ambient));
      glUniform4fv(MaterialSpecularUniform, 1, glm::value_ptr(MaterialVal.specular));
      glUniform1i(LightCountUniform, LightNum);
      glUniform3fv(CamPosUniform, 1, glm::value_ptr(g_GraphicsSys->GetCurrentCamera().GetPosition()));
      glUniform1f(ShininessUniform, Shininess);
      glUniform1fv(DistanceAttConstUniform, ARRAYSIZE(DistanceAttConstants), &DistanceAttConstants[0]);
      glUniform1i(DistanceAttBoolUniform, DistanceAtt);
      glUniform1f(NearPlaneLocation, NearPlane);
      glUniform1f(FarPlaneLocation, FarPlane);
      glUniform1i(AtmosphericAttBoolUniform, AtmosphericAtt);
      glUniform4fv(AtmosphericAttIntesityUniform, 1, glm::value_ptr(AtmosphericIntensity));

      glUniform4fv(GlobalAmbientUniform, 1, glm::value_ptr(GlobalAmbient));

      glUniform1i(TexturesUniform, Textures);

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
    fullmatrix = ViewToProjection  * WorldToView * ModelToWorld * scale;
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