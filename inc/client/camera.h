#ifndef CLIENT_CAMERA_H
#define CLIENT_CAMERA_H

#include "opengl_headers.h"
#include <client/shared_memory.h>

namespace client{

class Camera {
 public:
  Camera();
  ~Camera();

  void Update(PLAYER_INFO position, int interval);
  glm::mat4 LookAt();

 private:
  glm::vec3 position_;
  glm::vec3 direction_;
  glm::vec3 nose_;

  int movementSpeed_;
  double height_;
  double distance_;
};

}  // namespace client

#endif  // CLIENT_CAMERA_H
