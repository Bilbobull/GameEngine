// Author : Gonzalo Rojo 

#include "Context.h"
#include <iostream>

Window window;

Window::Window(void)
{
  Current_Window = 0;
  major_ = 0;
  minor_ = 0;
}

void Window::Init(void)
{
  SDL_GLContext opengl3_context;

  SDL_Init(SDL_INIT_VIDEO);

  //// set the opengl context version
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  // create the sdl2 window
  Current_Window = SDL_CreateWindow("Graphics Engine", SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, 512, 512,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  // create the opengl3 context
  opengl3_context = SDL_GL_CreateContext(Current_Window);

  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_);
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_);


  std::cout << "OpenGL Version: " << major_ << "." << minor_ << std::endl;

  GLenum status = glewInit();
  if (status != GLEW_OK)
  {
    std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
  }
}

