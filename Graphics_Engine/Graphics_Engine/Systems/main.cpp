#include "Graphics\Camera\Context.h"
#include "Graphics\GraphicsSystem.h"

int main(int argc, char* argv[])
{
  GraphicsSystem gra_sys;

  gra_sys.Init();
 
  gra_sys.Update(0.16);

  return 0;
}