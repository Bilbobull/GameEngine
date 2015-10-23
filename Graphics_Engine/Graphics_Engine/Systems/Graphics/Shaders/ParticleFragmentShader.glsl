#version 330

uniform sampler2D image;

in vec4 Color;
out vec4 outColor;

void main (void)
{
  outColor = texture (image, gl_PointCoord) * Color;
}