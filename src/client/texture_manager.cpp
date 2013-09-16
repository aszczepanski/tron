#include <client/texture_manager.h>

namespace client {

std::vector<std::vector<unsigned char> > TextureManager::imageData_;
std::vector<GLuint> TextureManager::handles_;
std::map<std::string, int> TextureManager::names_;

TextureManager::TextureManager(){}

TextureManager::~TextureManager(){}

void TextureManager::importTexture(const std::string& name, const std::string& filename) {
  names_[name] = imageData_.size();

  std::vector<unsigned char> image;
  imageData_.push_back(image);

  unsigned width, height;
  lodepng::decode(imageData_[imageData_.size()-1], width, height, filename);

  glEnable(GL_TEXTURE_2D);

  GLuint handle;
  glGenTextures(1, &handle); //Zainicjuj uchwyt tex
  handles_.push_back(handle);

  glBindTexture(GL_TEXTURE_2D, handle); //Przetwarzaj uchwyt tex
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, &imageData_[imageData_.size()-1][0]);
}

void TextureManager::setTexture(const std::string& name) {
  int index = names_[name];
  glBindTexture(GL_TEXTURE_2D, handles_[index]);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, &imageData_[index][0]);
}

} // namespace client
