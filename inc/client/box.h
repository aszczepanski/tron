#ifndef CLIENT_BOX_H
#define CLIENT_BOX_H

#include "opengl_headers.h"
#include "world.h"

using glm::vec3;


namespace client {

class Box {
 public:
  Box(vec3 dimensions);
  ~Box();
  void draw(mat4 M, GLuint texture = NULL);
 private:
  vec3 _dimensions;
};

}  // namespace client

#endif  // CLIENT_BOX_H
