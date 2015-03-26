#include "GraphicsSystem.h"

bool Running;

void GraphicsSystem::Init(void)
{
  Current_Window.Init();
  Running = true;
}

void GraphicsSystem::Update(double dt)
{
  while(!Current_Window.WindowShouldClose())
  {
    continue;
  }

  Running = false;
}

void GraphicsSystem::Free(void)
{

}
