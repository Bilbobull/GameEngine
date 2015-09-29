#version 330

in vec4 Color;
in vec3 Normal;
out vec4 outColor;

void main()
{
	outColor = vec4 (Normal, 1.0);
}