#ifndef CLIENT_CAMERA_H
#define CLIENT_CAMERA_H

#include "opengl_headers.h"
#include <client/shared_memory.h>

namespace client{

class Camera {
 public:
  explicit Camera(int mapSize);
  ~Camera();

  void Update(PLAYER_INFO position, int interval);
  glm::mat4 LookAt();

 private:

  void UpdatePosition(int interval);
  void KeepWithinMap();
  bool IsPositonOutOfMap();

  glm::vec3 position_;
  glm::vec3 direction_;
  glm::vec3 nose_;
  glm::vec3 targetPosition_;

  double rotation_;
  double targetRotation_;
  double rotationSpeed_;

  int mapSize_;

  int speed_;
  double height_;
  double distance_;
};

}  // namespace client

#endif  // CLIENT_CAMERA_H
