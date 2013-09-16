#include <client/open_gl_main.h>
#include <client/shared_memory.h>
#include <client/lighting.h>
#include <client/camera.h>
#include <client/world.h>
#include <client/box.h>
#include <client/axis.h>
#include <client/texture_manager.h>
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

std::vector<PLAYER_INFO> OpenGLMain::positions;
std::vector<TURN_INFO> OpenGLMain::turns;
std::vector<CRASH_INFO> OpenGLMain::crashes;
int OpenGLMain::lastTime_;
int nr;

Camera* camera;
Lighting* lighting;

OpenGLMain::OpenGLMain(SharedMemory& sharedMemory)
  : sharedMemory(sharedMemory)
{
}

void OpenGLMain::start(){
  lighting = new Lighting();
  camera = new Camera();

  Init();

  glutMainLoop();
}

void OpenGLMain::displayFrame() {
  glClearColor(0.05,0.05, 0.05,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 V = camera->LookAt();
  World::setV(V);

  glm::mat4 P = glm::perspective(50.0f, 1.0f, 1.0f, 60.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(glm::value_ptr(P));

  glMatrixMode(GL_MODELVIEW);

  int worldSize = 100;
  Axis::draw(worldSize/2);

  drawBikes();
  drawTrails();

  glutSwapBuffers();
}

void OpenGLMain::drawBikes() {
  for (int i = 0; i < positions.size(); i++) {
    // TODO color
    // TODO orientation
    mat4 M = World::transform(vec3(positions[i].x-0.5, positions[i].y-0.5, 0));
    Box* box = new Box(glm::vec3(1, 1, 1));
    box->draw(M);
    delete box;
  }
}

void OpenGLMain::drawTrails() {

  //TODO
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  std::map<int, std::vector<TURN_INFO> > playerTurns;

  for(int i = 0; i < turns.size(); i++)
    playerTurns[turns[i].player_no].push_back(turns[i]);

  for (std::map<int, std::vector<TURN_INFO> >::iterator it = playerTurns.begin(); it != playerTurns.end(); it++){
    int number = it->first;
    std::vector<TURN_INFO> list = it->second;

    drawPlayerTrail(positions[number], list);
  }
  //TODO
  glDisable(GL_BLEND);
}

void OpenGLMain::drawPlayerTrail(PLAYER_INFO position, std::vector<TURN_INFO> &list){
  for(int i = 0; i < list.size(); i++){
    TURN_INFO turn = list[i];

    vec3 begin = vec3(turn.move.x, turn.move.y, 0);
    vec3 end;

    if (i != list.size()-1)
      end = vec3(list[i+1].move.x, list[i+1].move.y, 0);
    else
      end = vec3(position.x, position.y, 0);

    vec3 size = end - begin;
    if( size.x == 0) size.x = 0.1;
    if( size.y == 0) size.y = 0.1;
    size.z = 1;

    mat4 M = World::transform(vec3(turn.move.x, turn.move.y, 0));
    Box* box = new Box(size);

    std::string texture;
    //TODO colorManager
    if(position.player_no == 1)
      texture = "blue";
    else
      texture = "red";

    box->draw(M, texture);
    delete box;
  }
}

void OpenGLMain::nextFrame()
{
  SharedMemory::getInstance().getPositions(positions);
  SharedMemory::getInstance().getTurns(turns);
  SharedMemory::getInstance().getCrashes(crashes);
  SharedMemory::getInstance().getPlayerNr(nr);

  int interval = passedTime();
  camera->Update(positions[nr], interval);

  glutPostRedisplay();
}

int OpenGLMain::passedTime() {
  int actTime = glutGet(GLUT_ELAPSED_TIME);
  int interval = actTime - lastTime_;
  lastTime_ = actTime;
  return interval;
}


void OpenGLMain::loadTextures(){
  TextureManager::importTexture("blue", "src/client/texture.png");
  TextureManager::importTexture("red", "src/client/red_trail.png");
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
    case GLUT_KEY_F2:
      printf("[F2 [start]]\n");
      SharedMemory::getInstance().keyStart();
      break;
    case GLUT_KEY_F1:
      printf("[F1 [escape]]\n");
      SharedMemory::getInstance().keyEsc();
      break;
  }
}

