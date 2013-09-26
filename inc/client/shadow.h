#ifndef CLIENT_SHADOW_H
#define CLIENT_SHADOW_H

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

class Shadow {
 public:
  Shadow(PLAYER_INFO position, std::vector<TURN_INFO>& turns, int color);
  ~Shadow();

  void Draw(int placement);

 private:

  void DrawLine(glm::mat4 M, glm::vec3 dimensions, std::string texture, int placement);
  void DrawCorner(glm::mat4 M, std::string texture, int placement);

  void AddX(glm::vec3 begin, glm::vec3 end);
  void AddY(glm::vec3 begin, glm::vec3 end);
  void AddCorner(glm::vec3 pos, int x, int y);

  void DrawAll(std::string texture, int placement);

  PLAYER_INFO position_;
  std::vector<TURN_INFO>& turns_;
  int color_;

  std::vector<float> vu_, vd_, tu_, td_, nu_, nd_;
  int cu_, cd_;
};

}  // namespace client

#endif  // CLIENT_SHADOW_H
