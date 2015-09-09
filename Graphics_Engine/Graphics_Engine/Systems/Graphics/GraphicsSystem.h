// Author: Gonzalo Rojo

#pragma once

#include "Context.h"
#include "Graphics_Headers.h"
#include "../Systems.h"

class GraphicsSystem : public System
{
public:
  GraphicsSystem(){}
  ~GraphicsSystem(){}

  void Init(void);
  void Update(double dt);
  void Free(void);

  Window& GetCurrentWindow(void);
  void SetBackgroundColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
  void Specify_Attributes_Simple(GLuint simpleProgram);

private:
  Window Current_Window;
};

extern GraphicsSystem* g_GraphicsSys;