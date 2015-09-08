#version 330

layout (location = 0) in vec3 position;

int main ()
{
  gl_Position = vec4(position.xyz, 1.0f);
}
