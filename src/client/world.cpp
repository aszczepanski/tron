#include <client/world.h>

namespace client{

mat4 World::_V;

mat4 World::center(){
  return mat4(1.0f);
}

mat4 World::transform(vec3 position){
  return transform(position, 0.0f, 0.0f, 0.0f);
}

mat4 World::transform(mat4 old, vec3 position){
  return transform(old, position, 0.0f, 0.0f, 0.0f);
}

mat4 World::transform(vec3 position, float ax, float ay, float az){
  mat4 M = glm::mat4(1.0f);
  return transform(M, position, ax, ay, az);
}

mat4 World::transform(mat4 old, vec3 position, float ax, float ay, float az){
  mat4 M = glm::translate(old, position);

  // Axis visible to us will change, so we adjust it
  vec3 x_axis = vec3(1.0f, 0.0f, 0.0f);
  vec3 y_axis = glm::rotate(vec3(0.0f, 1.0f, 0.0f), -ax, x_axis);
  vec3 z_axis = glm::rotate(vec3(0.0f, 0.0f, 1.0f), -ay, y_axis);

  M = glm::rotate(M, ax, x_axis);
  M = glm::rotate(M, ay, y_axis);
  M = glm::rotate(M, az, z_axis);
  return M;
}

mat4 World::getV(){
  return _V;
}

void World::setV(mat4 V){
 _V = V;
}

}
