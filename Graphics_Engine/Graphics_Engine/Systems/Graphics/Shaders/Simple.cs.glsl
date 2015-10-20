#version 430

layout(std140, binding = 0) buffer Pos
{
  vec4 Positions[];
};

layout(std140, binding = 1) buffer Vel
{
  vec4 Velocities[];
};

layout(local_size_x = 16, local_size_y = 16) in;

void main()
{
  // Shader Buffer index
  uint index = gl_GlobalInvocationID.x;

  vec3 vPos = Positions[index].xyz;
  vec3 vVel = Velocities[index].xyz;

  vPos += vVel * 0.16;

  Positions[index].xyz = vPos;
  Velocities[index].xyz = vVel;
}