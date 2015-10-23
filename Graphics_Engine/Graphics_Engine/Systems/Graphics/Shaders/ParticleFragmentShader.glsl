#version 330

uniform sampler2D uniTexture;

in vec4 vColor;
out vec4 outColor;

void main (void)
{
  outColor = texture2D (uniTexture, gl_PointCoord) * vColor;
}