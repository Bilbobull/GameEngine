// Author: Gonzalo Rojo

#pragma once

#include "Context.h"
#include "Graphics_Headers.h"
#include "../Systems.h"
#include "Camera.h"

class GraphicsSystem : public System
{
public:
  GraphicsSystem(){}
  ~GraphicsSystem(){}

  void Init(void);
  void Update(double dt);
  void Free(void);

  Window& GetCurrentWindow(void);
  Camera& GetCurrentCamera(void);
  void SetBackgroundColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
  void DebugDraw(void){ Debug_Draw = !Debug_Draw; }
  bool GetDebugDraw(void){ return Debug_Draw; }

  void VertexNormalDraw(void){ Vertex_Normal_Draw = !Vertex_Normal_Draw; }
  bool GetVertexNormalDraw(void){ return Vertex_Normal_Draw; }

  void FaceNormalDraw(void){ Face_Normal_Draw = !Face_Normal_Draw; }
  bool GetFaceNormalDraw(void){ return Face_Normal_Draw; }
  glm::vec3 Random_Color(void);

private:
  Window Current_Window;
  Camera My_Camera;
  bool Debug_Draw;
  bool Vertex_Normal_Draw;
  bool Face_Normal_Draw;
};

extern GraphicsSystem* g_GraphicsSys;