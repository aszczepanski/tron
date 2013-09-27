#ifndef CLIENT_TRAIL_H
#define CLIENT_TRAIL_H

#include "opengl_headers.h"
#include <vector>
#include <client/shared_memory.h>
#include <client/world.h>
#include <client/box.h>
#include <string>
#include <cstdio>
#include <client/texture_manager.h>
#include<algorithm>

namespace client{

class Trail {
 public:
  Trail(PLAYER_INFO position, std::vector<TURN_INFO>& turns, int color);
  ~Trail();

  void Draw();

 private:
  void AddX(glm::vec3 begin, glm::vec3 end);
  void AddY(glm::vec3 begin, glm::vec3 end);

  void DrawAll(std::string texture);

  PLAYER_INFO position_;
  std::vector<TURN_INFO>& turns_;
  int color_;

  std::vector<float> v_, t_, n_;
  int c_;
};

}  // namespace client

#endif  // CLIENT_TRAIL_H
