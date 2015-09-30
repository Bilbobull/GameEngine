#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texcoord;


uniform mat4 FullTransformMatrix;
 
out vec4 Color;
out vec3 Normal;
out vec2 Texcoord;

void main ()
{
  vec4 v = vec4(position, 1.0);

  gl_Position = FullTransformMatrix * v;
  Normal = normal;
  Color = vec4(1.0, 1.0, 1.0, 0.4);
  Texcoord = texcoord;
}
