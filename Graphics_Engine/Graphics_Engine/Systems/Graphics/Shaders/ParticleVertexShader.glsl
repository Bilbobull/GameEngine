#version 330

uniform mat4 matr;

struct Particle
{
  glm::vec3 position;
  float timeleft;
  glm::vec3 velocity;
  float size;
  glm::vec4 startcolor;
  glm::vec4 endcolor;
  glm::vec4 color;
  bool alive;
};

in Particle particles;

out vec4 Color;



void main(void)
{
	vec4 eyePos = vec4(particles.position , 1.0);
	gl_Position = matr * eyePos;
	Color = particles.color;
}