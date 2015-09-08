// Author: Gonzalo Rojo

#pragma once

#include "../Graphics/Graphics_Headers.h"
#include "../Graphics/Context.h"
#include "../Systems.h"

class InputSystem : public System
{
public:
  InputSystem(){}
  ~InputSystem(){}

  void Init(void);
  void Update(double dt);
  void Free(void);

  static void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mode);
  void CloseWindow(void);

private:

};

extern InputSystem* g_InputSys;