#version 330

uniform mat4 uniViewProjection;

in vec3 Position;
in vec4 Color;

out vec4 vColor;



void main(void)
{
  vec4 eyePos = vec4(Position, 1.0);
  gl_Position = uniViewProjection * eyePos;
	vColor = Color;
}