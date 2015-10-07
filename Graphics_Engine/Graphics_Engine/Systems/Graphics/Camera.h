#pragma once
#include "Math_Headers.h"

class Camera
{
public:
  Camera();
  ~Camera(){}
  glm::mat4 getWorldToViewMatrix(void) const;
  glm::mat4 getProjectionMatrix(void);
  void MouseUpdate(const glm::vec2& newMousePos);
  void MoveForward();
  void MoveBackWard();
  void MoveLeft();
  void MoveRight();
  void MoveUp();
  void MoveDown();


  glm::vec3 position;
  glm::vec3 viewDirection; //always one unit long
  const glm::vec3 up;
  const float near;
  const float far;
  glm::vec2 oldMousePos;
  glm::mat4 projectionMatrix;
};
 