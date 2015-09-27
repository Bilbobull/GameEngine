#include "Camera.h"
#include "GraphicsSystem.h"
#define MOVEMENT_SPEED 0.1f


Camera::Camera() : viewDirection(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), near(0.1f), far(20.0f)
{
}

void Camera::MouseUpdate(const glm::vec2& newMousePos)
{
  glm::vec2 Movement = newMousePos - oldMousePos;

  //rotate X
  if (glm::length(Movement) > 50.0f)
  {
    oldMousePos = newMousePos;
    return;
  }

  const float Rotate_speed = 0.001f;

  glm::vec3 RotateArround = glm::cross(viewDirection, up);

  glm::mat4 rotator = glm::rotate(-Movement.x * Rotate_speed, up) * 
                      glm::rotate(-Movement.y * Rotate_speed, RotateArround);

  viewDirection = glm::mat3(rotator) * viewDirection;

  oldMousePos = newMousePos;
}

void Camera::MoveForward()
{
  position += MOVEMENT_SPEED * viewDirection;
}

void Camera::MoveBackWard()
{
  position -= MOVEMENT_SPEED * viewDirection;
}

void Camera::MoveLeft()
{
  glm::vec3 direction = glm::cross(viewDirection, up);
  position += -MOVEMENT_SPEED * direction;
}

void Camera::MoveRight()
{
  glm::vec3 direction = glm::cross(viewDirection, up);
  position += MOVEMENT_SPEED * direction;
}

void Camera::MoveUp()
{
  position += MOVEMENT_SPEED * up;
}

void Camera::MoveDown()
{
  position += -MOVEMENT_SPEED * up;
}

glm::mat4 Camera::getWorldToViewMatrix(void) const
{
  return glm::lookAt(position, position + viewDirection, up);;
}

glm::mat4 Camera::getProjectionMatrix(void) 
{
  projectionMatrix = glm::perspective(90.0f, ((float)g_GraphicsSys->GetCurrentWindow().GetWidth()) / g_GraphicsSys->GetCurrentWindow().GetHeight(), near, far);
  return projectionMatrix; 
}



