#ifndef CLIENT_FLOOR_H
#define CLIENT_FLOOR_H

#include "opengl_headers.h"
#include "world.h"
#include <string>
#include <client/texture_manager.h>

using glm::vec3;


namespace client {

class Floor {
 public:
  Floor(vec3 dimensions);
  ~Floor();
  void draw(mat4 M, std::string texture = "");
 private:
  vec3 _dimensions;
};

}  // namespace client

#endif  // CLIENT_FLOOR_H

