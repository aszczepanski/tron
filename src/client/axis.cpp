#include <client/axis.h>

namespace client {

void Axis::draw(int size) {
  return;
  glLoadMatrixf(glm::value_ptr(World::getV() * World::center()));

  glLineWidth(2.4f);
  glEnable(GL_COLOR_MATERIAL);

  glBegin(GL_LINES);
    glColor3f(0.6, 0.6, 0.6);
    glVertex3f(-size, 0, 0);
    glVertex3f(size, 0, 0);

    glColor3f(0.6, 0.6, 0.6);
    glVertex3f(0, -size, 0);
    glVertex3f(0, size, 0);

    /*glColor3f(0, 0, 1);
    glVertex3f(0, 0, -100);
    glVertex3f(0, 0, 100);*/
  glEnd();

  glLineWidth(1.7f);
  glBegin(GL_LINES);
    for(int i = -size; i < size; i+=5){
      glColor3f(0.6, 0.6, 0.6);
      glVertex3f(-size, i, 0);
      glVertex3f(size, i, 0);
    }

    for(int i = -size; i < size; i+=5){
      glColor3f(0.6, 0.6, 0.6);
      glVertex3f(i, -size, 0);
      glVertex3f(i, size, 0);
    }
  glEnd();

  /*glLineWidth(1.2f);
  glBegin(GL_LINES);
    for(int i = -size; i < size; i++){
      glColor3f(0.4, 0.4, 0.4);
      glVertex3f(-size, i, 0);
      glVertex3f(size, i, 0);
    }

    for(int i = -size; i < size; i++){
      glColor3f(0.3, 0.3, 0.3);
      glVertex3f(i, -size, 0);
      glVertex3f(i, size, 0);
    }
  glEnd();*/
}

}
