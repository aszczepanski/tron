#include<cstdio>
#include <client/camera.h>

namespace client {

Camera::Camera() {
  speed_ = 1;
  // TODO distance should change according to bike speed
  height_ = 5;
  distance_ = 10;
  position_ = glm::vec3(0.0f, 0.0f, 15.0f);
  direction_ = glm::vec3(0.0f, 0.0f, 0.0f);
  nose_ = glm::vec3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera() {
}

void Camera::Update(PLAYER_INFO position, int interval) {
  direction_ = glm::vec3(position.x, position.y, 0.0f);

  if (position.direction == common::NORTH)
    targetPosition_ = glm::vec3(position.x, position.y - distance_, height_);
  if (position.direction == common::SOUTH)
    targetPosition_ = glm::vec3(position.x, position.y + distance_, height_);
  if (position.direction == common::WEST)
    targetPosition_ = glm::vec3(position.x - distance_, position.y, height_);
  if (position.direction == common::EAST)
    targetPosition_ = glm::vec3(position.x + distance_, position.y, height_);

  UpdatePosition(interval);
}

void Camera::UpdatePosition(int interval) {
  glm::vec3 move = targetPosition_ - position_;
  double modifier = move.length() / speed_;

  move.x /= modifier;
  move.y /= modifier;
  move.x /= modifier;

  position_ += move;
}

glm::mat4 Camera::LookAt() {
  return glm::lookAt(position_, direction_, nose_);
}

}  // namespace client
