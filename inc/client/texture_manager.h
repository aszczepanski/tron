#ifndef CLIENT_TEXTURE_MANAGER_H
#define CLIENT_TEXTURE_MANAGER_H

#include "opengl_headers.h"
#include <client/lodepng.h>
#include <string>
#include <map>
#include <vector>

namespace client {

class TextureManager {
 public:
  TextureManager();
  ~TextureManager();

  static void importTexture(const std::string& name, const std::string& filename);
  static void setTexture(const std::string& name);

 private:
  static std::vector<std::vector<unsigned char> > imageData_;
  static std::vector<GLuint> handles_;
  static std::map<std::string, int> names_;
};

}  // namespace client

#endif
