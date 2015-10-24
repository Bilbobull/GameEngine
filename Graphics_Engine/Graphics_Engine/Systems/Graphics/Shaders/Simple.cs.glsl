#version 430

struct Particle
{
  vec3 position;
  float timeleft;
  vec3 velocity;
  float size;
  vec4 startcolor;
  vec4 endcolor;
  vec4 color;
  float alive;

  vec3 padding;
};

layout(std140, binding = 0) buffer Part
{
  Particle particles[];
};

layout(local_size_x = 16, local_size_y = 16) in;

void main()
{
  //// Shader Buffer index
  uint index = gl_GlobalInvocationID.x;

  vec3 vPos = particles[index].position;
  vec3 vVel = particles[index].velocity;
  vec4 vColor = particles[index].color;

  //vec3 vVel = vec3 (1.0f);

  vPos += vVel * 0.16;
  //vPos = vec3(1.0, 0.0, 0.0);

  particles[index].position = vPos;
  particles[index].color = vColor;
  ////particles[index].velocity.xyz = vVel;
}