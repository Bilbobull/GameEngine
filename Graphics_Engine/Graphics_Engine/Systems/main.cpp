#include "Graphics\Camera\Context.h"

int i = 0;

int main(int argc, char* argv[])
{
  Current_Window.Init();

  while (!Current_Window.WindowShouldClose())
  {
    ++i;
  }

  return 0;
}