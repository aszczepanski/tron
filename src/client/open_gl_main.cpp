#include <client/open_gl_main.h>
#include <client/shared_memory.h>
#include <client/lighting.h>
#include <client/camera.h>
#include <client/world.h>
#include <client/box.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <cstring>
#include <cstdio>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "cube.h"
#include "tga.h"
#include <vector>
#include <iostream>

using namespace client;
using glm::mat4;
using glm::vec3;

static float xAngle = 0.0f;
static float yAngle = 0.0f;

std::vector<PLAYER_INFO> OpenGLMain::positions;
std::vector<TURN_INFO> OpenGLMain::turns;
std::vector<CRASH_INFO> OpenGLMain::crashes;
int OpenGLMain::lastTime_;

GLuint tex;
TGAImg img;

Camera* camera;
Lighting* lighting;



OpenGLMain::OpenGLMain(SharedMemory& sharedMemory)
  : sharedMemory(sharedMemory)
{
}

void OpenGLMain::Init(){
  int argc = 1;
  char* argv[] = { "./main" };
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800,800);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Awesome Game");

  glutDisplayFunc(displayFrame);
  glutIdleFunc(nextFrame);

  glewInit();
  glutSpecialFunc(keyDown);

  glShadeModel(GL_SMOOTH);

  lighting->Init();

  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);

  // TODO remove TGA and create class managing all textures
  loadTextures();
}

void OpenGLMain::start(){
  lighting = new Lighting();
  camera = new Camera();

  Init();

  glutMainLoop();
}

void OpenGLMain::displayFrame() {
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 V = camera->LookAt();
  World::setV(V);

  glm::mat4 P = glm::perspective(50.0f, 1.0f, 1.0f, 60.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(glm::value_ptr(P));

  glMatrixMode(GL_MODELVIEW);

  // Draw bikes
  for (int i = 0; i < positions.size(); i++) {
    // TODO color
    mat4 M = World::transform(vec3(positions[i].x-0.5, positions[i].y-0.5, 0));
    Box* box = new Box(glm::vec3(1, 1, 1));
    box->draw(M);
  }

  // Draw trails
  for (std::vector<TURN_INFO>::iterator it = turns.begin(); it != turns.end(); it++) {
    vec3 begin = vec3(it->move.x, it->move.y, 0);
    vec3 end;
    if (it != turns.end() - 1)
      end = vec3((it + 1)->move.x, (it + 1)->move.y, 0);
    else
      end = vec3(positions[0].x, positions[0].y, 0);
    vec3 size = end - begin;
    if( size.x == 0) size.x = 0.1;
    if( size.y == 0) size.y = 0.1;
    size.z = 1;

    mat4 M = World::transform(vec3(it->move.x, it->move.y, 0));
    Box* box = new Box(size);
    box->draw(M, tex);
  }

  glutSwapBuffers();
}

void OpenGLMain::nextFrame()
{
  SharedMemory::getInstance().getPositions(positions);
  SharedMemory::getInstance().getTurns(turns);
  SharedMemory::getInstance().getCrashes(crashes);

  int interval = passedTime();
  camera->Update(positions[0], interval);

  glutPostRedisplay();
}

int OpenGLMain::passedTime() {
  int actTime = glutGet(GLUT_ELAPSED_TIME);
  int interval = actTime - lastTime_;
  lastTime_ = actTime;
  return interval;
}

void OpenGLMain::keyDown(int c, int x, int y)
{
  switch(c)
  {
    case GLUT_KEY_LEFT:
      printf("[left]\n");
      SharedMemory::getInstance().keyLeft();
      break;
    case GLUT_KEY_RIGHT:
      printf("[right]\n");
      SharedMemory::getInstance().keyRight();
      break;
    case GLUT_KEY_UP:
      printf("[up]\n");
      SharedMemory::getInstance().keyUp();
      break;
    case GLUT_KEY_DOWN:
      printf("[down]\n");
      SharedMemory::getInstance().keyDown();
      break;
    case GLUT_KEY_F2:
      printf("[F2 [start]]\n");
      SharedMemory::getInstance().keyStart();
      break;
    case GLUT_KEY_F1:
      printf("[F1 [escape]]\n");
      SharedMemory::getInstance().keyEsc();
      break;
    case GLUT_KEY_PAGE_DOWN:
      yAngle += 1.0f;
      break;
    case GLUT_KEY_PAGE_UP:
      yAngle -= 1.0f;
      break;
  }
}


void OpenGLMain::loadTextures(){
  if (img.Load("src/client/bricks.tga")==IMG_OK) {
    glGenTextures(1,&tex); //Zainicjuj uchwyt tex
    glBindTexture(GL_TEXTURE_2D,tex); //Przetwarzaj uchwyt tex
    if (img.GetBPP()==24) //Obrazek 24bit
      glTexImage2D(GL_TEXTURE_2D,0,3,img.GetWidth(),img.GetHeight(),0,
          GL_RGB,GL_UNSIGNED_BYTE,img.GetImg());
    else if (img.GetBPP()==32)
      //Obrazek 32bit
      glTexImage2D(GL_TEXTURE_2D,0,4,img.GetWidth(),img.GetHeight(),0,
          GL_RGBA,GL_UNSIGNED_BYTE,img.GetImg());
    else {
      //Obrazek 16 albo 8 bit, takimi się nie przejmujemy
    }
  }
  else {
    // błąd
    std::cout << "error\n";
  }
}
