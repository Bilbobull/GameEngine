#include "Editor.h"
#include "../Graphics/Graphics_Headers.h"
#include "../Graphics/Math_Headers.h"
#include <memory>
#include <vector>
#include "../Graphics/GraphicsSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

// NOTE: This file contains the code needed to render ImGui within an OpenGL 3
// context. This is very handy to have around, so feel free to reuse it in any
// other application you choose. Note that you may need to change the input
// bindings depending on what library you are using for windowing. Nevertheless,
// nothing in this file is necessary to understand to pass CS300, which is why
// it is provided in its entirety to you, with an overall lack of in-depth
// documentation. The OpenGL code used here is minimal and handy to know, but
// again, not critical for CS300.

struct ImGuiImpl::State
{
  // GL state representing ImGui during this render frame.
  static GLuint shaderHandle;
  static GLint textureLocation, orthoLocation;
  static GLint positionLocation, uvLocation, colorLocation;
  static GLuint vboHandle, vaoHandle;
  static GLuint fontTextureHandle;

  static size_t vboMaxSize; // initially 20,000
  static GLchar const *vertexShaderSource;
  static GLchar const *fragmentShaderSource;


  // Updates the GL state based on the current ImGui render state.
  static void RenderDrawLists(ImDrawList** const cmdLists, int cmdListsCount);

  // Initializes GL objects for rendering ImGui (the VBO, VAO, and texture).
  static void Initialize();

  // Cleans up GL objects used to render ImGui during the lifetime of the app.
  static void Cleanup();
};


void ImGuiImpl::Initialize(unsigned windowWidth, unsigned windowHeight)
{
  // This initialization is specifically to allow ImGui to track input using
  // FreeGLFW, along with all the callback funcitons following Initialize.
  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = ImVec2(static_cast<float>(windowWidth),
    static_cast<float>(windowHeight));
  io.DeltaTime = 1.0f / 60.0f;
  io.PixelCenterOffset = 0.0f;
  io.KeyMap[ImGuiKey_Tab] = 9;
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Delete] = 127;
  io.KeyMap[ImGuiKey_Backspace] = 8;
  io.KeyMap[ImGuiKey_Enter] = 13;
  io.KeyMap[ImGuiKey_Escape] = 27;
  io.KeyMap[ImGuiKey_A] = 1;
  io.KeyMap[ImGuiKey_C] = 3;
  io.KeyMap[ImGuiKey_V] = 22;
  io.KeyMap[ImGuiKey_X] = 24;
  io.KeyMap[ImGuiKey_Y] = 25;
  io.KeyMap[ImGuiKey_Z] = 26;
  io.RenderDrawListsFn = State::RenderDrawLists; /* render callback */

  State::Initialize();
  ImGui::NewFrame();
}

void ImGuiImpl::Render()
{
  ImGui::Render();
}

void ImGuiImpl::Cleanup()
{
  State::Cleanup(); // free any OpenGL resources used by ImGui
}

void ImGuiImpl::Reshape(int w, int h)
{
  ImGuiIO &io = ImGui::GetIO();
  // Display size, in pixels. For clamping windows' positions.
  io.DisplaySize = ImVec2(static_cast<float>(w), static_cast<float>(h));
}

void ImGuiImpl::Entry(int state)
{
  if (state == GLFW_MOUSE_BUTTON_LEFT) // user left the window window
  {
    ImGuiIO &io = ImGui::GetIO();
    io.MousePos.x = io.MousePos.y = -1;
  }
}

void ImGuiImpl::SpecialDown(int key, int x, int y)
{
  SpecialUpDown(key, x, y, true);
}

void ImGuiImpl::SpecialUp(int key, int x, int y)
{
  SpecialUpDown(key, x, y, false);
}

void ImGuiImpl::KeyboardDown(unsigned char key, int x, int y)
{
  KeyboardUpDown(key, x, y, true);
}

void ImGuiImpl::KeyboardUp(unsigned char key, int x, int y)
{
  KeyboardUpDown(key, x, y, false);
}

void ImGuiImpl::Mouse(int button, int state, float x, float y)
{
  ImGuiIO &io = ImGui::GetIO();
  //int const mods = GLFWGetModifiers();

  //io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
  //io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
  io.MousePos.x = static_cast<float>(x);
  io.MousePos.y = static_cast<float>(y);

  if (button >= 0 && button < 5)
  {
    io.MouseDown[button] = (state == GLFW_PRESS);

    // Manual double click handling:
    static double dblClickTimes[6] = { -FLT_MAX, -FLT_MAX, -FLT_MAX,
      -FLT_MAX, -FLT_MAX, -FLT_MAX };  // seconds
    static bool gImGuiBindingMouseDblClicked[5] = { false, false, false,
      false, false };

    if (state == GLFW_PRESS)
    {
      double time = glfwGetTime() * 1000;
      double &oldTime = dblClickTimes[button];
      bool &mouseDoubleClicked = gImGuiBindingMouseDblClicked[button];
      if (time - oldTime < io.MouseDoubleClickTime * 1000.f)
      {
        mouseDoubleClicked = true;
        oldTime = -FLT_MAX;
      }
      else
      {
        mouseDoubleClicked = false;
        oldTime = time;
      }
    }
  }
}

void ImGuiImpl::MouseWheel(int button, int state, int x, int y)
{
  ImGuiIO &io = ImGui::GetIO();
  //int const mods = GLFWGetModifiers();
  //io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
  //io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
  io.MousePos.x = static_cast<float>(x);
  io.MousePos.y = static_cast<float>(y);
  if (state == GLFW_PRESS)
  {
    io.MouseWheel = (button == GLFW_MOUSE_BUTTON_LEFT) ? 1
      : (button == GLFW_MOUSE_BUTTON_MIDDLE) ? -1 : 0;
  }
}

void ImGuiImpl::Motion(float x, float y)
{
  ImGuiIO &io = ImGui::GetIO();
  io.MousePos.x = static_cast<float>(x);
  io.MousePos.y = static_cast<float>(y);
}

void ImGuiImpl::PassiveMotion(float x, float y)
{
  ImGuiIO &io = ImGui::GetIO();
  io.MousePos.x = static_cast<float>(x);
  io.MousePos.y = static_cast<float>(y);
}

void ImGuiImpl::SpecialUpDown(int key, int x, int y, bool down)
{
  ImGuiIO &io = ImGui::GetIO();

  //int const mods = GLFWGetModifiers();
  //io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
  //io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
  io.MousePos.x = static_cast<float>(x);
  io.MousePos.y = static_cast<float>(y);

  if (key >= 0 && key < 512)
    io.KeysDown[key] = down;
}

void ImGuiImpl::KeyboardUpDown(unsigned char key, int x, int y, bool down)
{
  ImGuiIO &io = ImGui::GetIO();

  //int const mods = GLFWGetModifiers();
  //io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
  //io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
  io.MousePos.x = static_cast<float>(x);
  io.MousePos.y = static_cast<float>(y);

  if (key != GLFW_KEY_LEFT && key != GLFW_KEY_RIGHT &&
    key != GLFW_KEY_UP && key != GLFW_KEY_DOWN &&
    key != GLFW_KEY_HOME && key != GLFW_KEY_END)
    io.KeysDown[key] = down;

  if (down)
    io.AddInputCharacter(key);
}

struct
{
  glm::vec4 ambient, diffuse;
} Material;

static int debugMode = 0;
float rotationX = 0.f, rotationY = 0.f, rotationZ = 0.f;
glm::vec3 position(-2.f, 2.f, 2.f);

void ImGuiImpl::UpdateGuiButtons(void)
{
  {
    // this code is already setup to copy the model file name into a local C
    // buffer, allow ImGui to alter it, and then immediately update the file
    // name based on the changes; feel free to change this logic as you see fit
    std::string modelFile = "cube.obj";
    char fileNameBuffer[140] = { '\0' };
    std::strcat(fileNameBuffer, modelFile.c_str());
    if (ImGui::InputText("Model", fileNameBuffer, sizeof(fileNameBuffer)))
    {
      // text was changed; copy back over to C++ string
      modelFile = fileNameBuffer;
    }

    if (ImGui::Button("Load Model"))
    {
      // TODO(student): implement loading models from a file; you can use the
      // string 'modelFile' which should store the updated file name from ImGui
    }

    if (ImGui::Button("Reload Shaders"))
    {
      // we could implement reloading shaders here, but not necessary for
      // assignment 1
    }

    // TODO(student): implement GUI logic for enabling debug drawing modes
    
    std::vector<char const *> debugModeStrings = {
      "No", "FaceNormal", "VertexNormal"};
    ImGui::Combo("Debug Mode", &debugMode, debugModeStrings.data(), 3);
    if (debugMode == 0)
      g_GraphicsSys->DebugDraw(false);
    else
      g_GraphicsSys->DebugDraw(true);

    if (debugMode == 1)
      g_GraphicsSys->FaceNormalDraw(true);
    else
      g_GraphicsSys->FaceNormalDraw(false);

    if (debugMode == 2)
      g_GraphicsSys->VertexNormalDraw(true);
    else
      g_GraphicsSys->VertexNormalDraw(false);

  }


  ImGui::Separator();
  // model options
  {
    ImGui::PushID("ModelOptions");

    // TODO(student): implement Euler-angles rotation and translation

    ImGui::InputFloat3("Model Position", (float*) &position);
    ImGui::SliderAngle("Rotation X", &rotationX);
    ImGui::SliderAngle("Rotation Y", &rotationY);
    ImGui::SliderAngle("Rotation Z", &rotationZ);

    g_GraphicsSys->GetCurrentCamera().position = position;

    if (ImGui::CollapsingHeader("Material"))
    {
      // TODO: feel free to alter this code as you see fit; perhaps you wish to
      // change how materials are being handled; that's completely fine; this
      // is fully implemented just to demonstrate how to use ImGui, since none
      // of the other inputs are saved (minus the model file path input).
      ImGui::ColorEdit3("Ambient", (float*)&Material.ambient);
      ImGui::ColorEdit3("Diffuse", (float*)&Material.diffuse);
    }

    ImGui::PopID();
  }


  // light options
  ImGui::Separator();
  {
    // PushID/PopID ensures that each of the inputs for lights do not conflict
    // with each other; 'Position' below stores temporary state based on that
    // name, so each element in this window by name 'Position' could potentially
    // conflict; this stack of IDs helps prevent that

    // Light 0
    ImGui::PushID("Light 0");
    if (ImGui::CollapsingHeader("Light 0"))
    {
      // TODO(student): implement handling inputs for lights
      float dummy1[3] = { 0.f }, dummy2[3] = { 0.f }, dummy3[3] = { 0.f };
      ImGui::InputFloat3("Direction", dummy1);
      ImGui::ColorEdit3("Ambient", dummy2);
      ImGui::ColorEdit3("Diffuse", dummy3);
    }
    ImGui::PopID();
  }



}


// variable definitions
GLuint ImGuiImpl::State::shaderHandle = 0;
GLint ImGuiImpl::State::textureLocation = 0;
GLint ImGuiImpl::State::orthoLocation = 0;
GLint ImGuiImpl::State::positionLocation = 0;
GLint ImGuiImpl::State::uvLocation = 0;
GLint ImGuiImpl::State::colorLocation = 0;
GLuint ImGuiImpl::State::vboHandle = 0;
GLuint ImGuiImpl::State::vaoHandle = 0;
GLuint ImGuiImpl::State::fontTextureHandle = 0;

size_t ImGuiImpl::State::vboMaxSize = 20000;

// GLSL vertex shader used to render ImGui windows.
GLchar const *ImGuiImpl::State::vertexShaderSource =
"#version 330\n"
"uniform mat4 Ortho;\n"
"in vec2 Position;\n"
"in vec2 UV;\n"
"in vec4 Color;\n"
"out vec2 Frag_UV;\n"
"out vec4 Frag_Color;\n"
"void main()\n"
"{\n"
"  Frag_UV = UV;\n"
"  Frag_Color = Color;\n"
"  gl_Position = Ortho * vec4(Position.xy, 0, 1);\n"
"}\n";

// GLSL fragment shader used to render ImGui windows.
GLchar const *ImGuiImpl::State::fragmentShaderSource =
"#version 330\n"
"uniform sampler2D Texture;\n"
"in vec2 Frag_UV;\n"
"in vec4 Frag_Color;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"  FragColor = Frag_Color * texture(Texture, Frag_UV.st);\n"
"}\n";

void ImGuiImpl::State::RenderDrawLists(ImDrawList** const cmdLists,
  int cmdListsCount)
{
  // GL states needed to render ImGui.
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);

  // Setup texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fontTextureHandle);

  // Setup orthographic projection matrix
  float const width = ImGui::GetIO().DisplaySize.x;
  float const height = ImGui::GetIO().DisplaySize.y;
  float const orthoProjection[4][4] =
  {
    { 2.0f / width, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f / -height, 0.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f, 0.0f },
    { -1.0f, 1.0f, 0.0f, 1.0f },
  };
  glUseProgram(shaderHandle);
  glUniform1i(textureLocation, 0);
  glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, &orthoProjection[0][0]);

  // Grow our buffer according to what we need
  size_t total_vtx_count = 0;
  for (int n = 0; n < cmdListsCount; n++)
    total_vtx_count += cmdLists[n]->vtx_buffer.size();
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
  size_t neededBufferSize = total_vtx_count * sizeof(ImDrawVert);
  if (neededBufferSize > vboMaxSize)
  {
    vboMaxSize = neededBufferSize + 5000;  // Grow buffer
    glBufferData(GL_ARRAY_BUFFER, vboMaxSize, NULL, GL_STREAM_DRAW);
  }

  // Copy and convert all vertices into a single contiguous buffer
  unsigned char *bufferData = static_cast<unsigned char*>(glMapBuffer(
    GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  if (!bufferData)
    return;
  for (int n = 0; n < cmdListsCount; n++)
  {
    ImDrawList const *cmd_list = cmdLists[n];
    std::memcpy(bufferData, &cmd_list->vtx_buffer[0],
      cmd_list->vtx_buffer.size() * sizeof(ImDrawVert));
    bufferData += cmd_list->vtx_buffer.size() * sizeof(ImDrawVert);
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(vaoHandle);

  int cmdOffset = 0;
  for (int n = 0; n < cmdListsCount; n++)
  {
    ImDrawList *cmdList = cmdLists[n];
    int vtxOffset = cmdOffset;
    auto &commands = cmdList->commands;
    ImDrawCmd *pcmd_end = commands.end();
    for (ImDrawCmd const *pcmd = commands.begin(); pcmd != pcmd_end; pcmd++)
    {
      glScissor(static_cast<GLint>(pcmd->clip_rect.x),
        static_cast<GLint>(height - pcmd->clip_rect.w),
        static_cast<GLint>(pcmd->clip_rect.z - pcmd->clip_rect.x),
        static_cast<GLint>(pcmd->clip_rect.w - pcmd->clip_rect.y));
      glDrawArrays(GL_TRIANGLES, vtxOffset, pcmd->vtx_count);
      vtxOffset += pcmd->vtx_count;
    }
    cmdOffset = vtxOffset;
  }

  // Restore modified state
  glBindVertexArray(0);
  glUseProgram(0);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void ImGuiImpl::State::Initialize()
{
  shaderHandle = glCreateProgram();
  GLuint vertHandle = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertHandle, 1, &vertexShaderSource, 0);
  glShaderSource(fragHandle, 1, &fragmentShaderSource, 0);
  glCompileShader(vertHandle);
  glCompileShader(fragHandle);
  glAttachShader(shaderHandle, vertHandle);
  glAttachShader(shaderHandle, fragHandle);
  glLinkProgram(shaderHandle);
  // per OpenGL spec, shaders do not need to be kept to use the program
  glDeleteShader(vertHandle);
  glDeleteShader(fragHandle);

  textureLocation = glGetUniformLocation(shaderHandle, "Texture");
  orthoLocation = glGetUniformLocation(shaderHandle, "Ortho");
  positionLocation = glGetAttribLocation(shaderHandle, "Position");
  uvLocation = glGetAttribLocation(shaderHandle, "UV");
  colorLocation = glGetAttribLocation(shaderHandle, "Color");

  glGenBuffers(1, &vboHandle);
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
  glBufferData(GL_ARRAY_BUFFER, vboMaxSize, NULL, GL_DYNAMIC_DRAW);

  glGenVertexArrays(1, &vaoHandle);
  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
  glEnableVertexAttribArray(positionLocation);
  glEnableVertexAttribArray(uvLocation);
  glEnableVertexAttribArray(colorLocation);

  glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE,
    sizeof(ImDrawVert), reinterpret_cast<GLvoid *>(offsetof(ImDrawVert, pos)));
  glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
    reinterpret_cast<GLvoid *>(offsetof(ImDrawVert, uv)));
  glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE,
    sizeof(ImDrawVert), reinterpret_cast<GLvoid *>(offsetof(ImDrawVert, col)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenTextures(1, &fontTextureHandle);
  glBindTexture(GL_TEXTURE_2D, fontTextureHandle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  void const *pngData;
  unsigned int pngSize;
  int texX, texY, texComp;

  ImGui::GetDefaultFontData(NULL, NULL, &pngData, &pngSize);
  void* texData = stbi_load_from_memory(
    static_cast<unsigned char const *>(pngData), static_cast<int>(pngSize),
    &texX, &texY, &texComp, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texX, texY, 0, GL_RGBA,
    GL_UNSIGNED_BYTE, texData);
  stbi_image_free(texData);

  //CheckGL();
}

void ImGuiImpl::State::Cleanup()
{
  glDeleteTextures(1, &fontTextureHandle);
  glDeleteVertexArrays(1, &vaoHandle);
  glDeleteBuffers(1, &vboHandle);
  glDeleteProgram(shaderHandle);
}
