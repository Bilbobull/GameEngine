#version 430

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

layout(std140, binding = 0) buffer Part
{
  Particle particles[];
};

layout(local_size_x = 16, local_size_y = 16) in;

void main()
{
  // Shader Buffer index
  uint index = gl_GlobalInvocationID.x;

  vec3 vPos = particles[index].position.xyz;
  vec3 vVel = particles[index].velocity.xyz;

  vPos += vVel * 0.16;

  particles[index].position.xyz = vPos;
  particles[index].velocity.xyz = vVel;
}