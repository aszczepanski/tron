#ifndef CLIENT_WORLD_H
#define CLIENT_WORLD_H

#include "opengl_headers.h"

using glm::vec3;
using glm::mat4;

namespace client {

class World {
 public:
  static mat4 center();
  static mat4 transform(vec3 position, float ax, float ay, float az);
  static mat4 transform(vec3 position);
  static mat4 transform(mat4 old, vec3 position);
  static mat4 transform(mat4 old, vec3 position, float ax, float ay, float az);

  static mat4 getV();
  static void setV(mat4 V);
 private:
  static mat4 _V;
};

}  // namespace client

#endif
