#include<cstdio>
#include <client/camera.h>

namespace client {

Camera::Camera(int mapSize) : mapSize_(mapSize){
  speed_ = 1.9;
  // TODO distance should change according to bike speed
  height_ = 8;
  distance_ = 15;
  rotation_ = 0;
  rotationSpeed_ = 800;
  position_ = glm::vec3(0.0f, 0.0f, 15.0f);
  direction_ = glm::vec3(0.0f, 0.0f, 0.0f);
  nose_ = glm::vec3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera() {
}

void Camera::Update(PLAYER_INFO position, int interval) {
  direction_ = glm::vec3(position.x, position.y, 0.0f);

  targetRotation_ = position.direction * 90.0 + 8;
  targetPosition_ = glm::vec3(position.x, position.y, 0);

  UpdatePosition(interval);

  KeepWithinMap();
}

void Camera::UpdatePosition(int interval) {

  double toRotate = targetRotation_ - rotation_;
  double maxRotation = rotationSpeed_ * interval / 1000.0;

  // Keep rotation from -180 to 180 deg.
  if (toRotate > 180.0) toRotate = toRotate - 360;
  if (toRotate < -180.0) toRotate = 360.0 + toRotate;

  // Keep the move smooth
  // TODO definitely can be improved
  double modifier = ((fabs(toRotate) + 20.0) / 90.0);
  maxRotation = maxRotation * modifier;


  printf("targetRotation*********** %lf ***\n", targetRotation_);
  printf("rotation*********** %lf ***\n", rotation_);
  printf("toRotate*********** %lf ***\n", toRotate);
  printf("max*********** %lf ***\n", maxRotation * 1000.0/ interval);


  if (toRotate > 0 && toRotate > maxRotation)
    toRotate = maxRotation;

  if (toRotate < 0 && toRotate < -maxRotation)
    toRotate = -maxRotation;

  rotation_ = rotation_ + toRotate;
  if ( rotation_ < 0) rotation_ += 360.0;
  if ( rotation_ > 360.0) rotation_ -= 360.0;


  position_ = glm::vec3(0, - distance_, 0);
  position_ = glm::rotate(position_, (float) rotation_, glm::vec3(0,0,1));
  position_.x += targetPosition_.x;
  position_.y += targetPosition_.y;
  position_.z = height_;
}

void Camera::KeepWithinMap() {
  if (! IsPositonOutOfMap()) return;

  // TODO OPTIMIZE
  int maxIterations = 30;
  while (IsPositonOutOfMap() && maxIterations--) {
    position_ -= targetPosition_;
    position_ /= 1.1f;
    position_ += targetPosition_;
  }
  position_.z = height_;
}

bool Camera::IsPositonOutOfMap() {
  if (position_.x < - mapSize_ / 2 ||
      position_.y < - mapSize_ / 2 ||
      position_.x > mapSize_ / 2 ||
      position_.y > mapSize_ / 2 ) return true;
  return false;
  
}

glm::mat4 Camera::LookAt() {
  return glm::lookAt(position_, direction_, nose_);
}

}  // namespace client
