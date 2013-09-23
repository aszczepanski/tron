#include <client/lighting.h>

namespace client {

Lighting::Lighting() {}
Lighting::~Lighting() {}


void Lighting::Init() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 0.1 };
  GLfloat light_diffuse[] = { 0.2, 0.2, 0.2, 0.1 };
  GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };
  GLfloat light_position[] = { 2.0, 2.0, 12.0, 1.0 };

  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position);

  //glEnable(GL_LIGHT2);

  GLfloat light2_ambient[] = { 1.0, 1.0, 1.0, 0.7 };
  GLfloat light2_diffuse[] = { 1.0, 1.0, 1.0, 0.7 };
  GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 0.9 };
  GLfloat light2_position[] = { 0.0, 0.0, 20.0, 1.0 };

  glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

  GLfloat global_ambient[] = { 0.3f, 0.3f, 0.3f, 0.3f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}

}  // namespace client
