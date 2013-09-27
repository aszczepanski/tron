#include <client/open_gl_main.h>
#include <client/shared_memory.h>
#include <client/lighting.h>
#include <client/camera.h>
#include <client/world.h>
#include <client/box.h>
#include <client/floor.h>
#include <client/trail.h>
#include <client/shadow.h>
#include <client/axis.h>
#include <client/texture_manager.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
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
#include <common/protocol.h>

#include <client/textured_model3d.h>

using namespace client;
using glm::mat4;
using glm::vec3;

const unsigned int defaultWidth = 800, defaultHeight = 800;
unsigned int curWidth, curHeight;

std::vector<PLAYER_INFO> OpenGLMain::positions;
std::vector<TURN_INFO> OpenGLMain::turns;
std::vector<CRASH_INFO> OpenGLMain::crashes;
int OpenGLMain::lastTime_;
int nr;

Camera* camera;
Lighting* lighting;

TexturedModel3D* model1;
TexturedModel3D* model2;
TexturedModel3D* model3;

OpenGLMain::OpenGLMain(SharedMemory& sharedMemory)
  : sharedMemory(sharedMemory)
{
}

void OpenGLMain::start(){
  lighting = new Lighting();
  camera = new Camera(FIELD_SIZE);

  Init();

  glutMainLoop();
}

glm::mat4 P;

void OpenGLMain::drawEverything()
{
  glMatrixMode(GL_MODELVIEW);

  Axis::draw(FIELD_SIZE/2);

  drawBikes();

  mat4 M = World::transform(vec3(-FIELD_SIZE/2, -FIELD_SIZE/2, -0.01));
  Box* box = new Box(glm::vec3(FIELD_SIZE, FIELD_SIZE, FIELD_SIZE*12));
  box->draw(M, "wall");
  delete box;

  M = World::transform(vec3(-FIELD_SIZE/2, -FIELD_SIZE/2, -0.99));
  Floor* floor = new Floor(glm::vec3(FIELD_SIZE, FIELD_SIZE, 1));
  floor->draw(M, "floor");
  delete floor;

  drawTrails();

}

void OpenGLMain::displayFrame() {
  glClearColor(0.05f,0.05f,0.05f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 V = camera->LookAt();
  World::setV(V);

//  glm::mat4 P = glm::perspective(50.0f, 1.0f, 1.0f, 160.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(glm::value_ptr(P));
  glViewport(0,0,curWidth,curHeight);

  drawEverything();

  glClear(GL_DEPTH_BUFFER_BIT);
  V = glm::lookAt(glm::vec3(positions[nr].x,positions[nr].y,22.0f), glm::vec3(positions[nr].x,positions[nr].y,0.0f), glm::vec3(0.0f,1.0f,0.0f));
  World::setV(V);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(glm::value_ptr(P));
  glViewport(20,20,200,200);

  drawEverything();

  glutSwapBuffers();
}

void OpenGLMain::drawBikes() {
	for (int i = 0; i < positions.size(); i++) {
		// TODO color
		// TODO orientation
		mat4 M = World::transform(vec3(positions[i].x, positions[i].y, 0));
		M = glm::rotate(M, 90.0f, vec3(1.0f,0.0f,0.0f));
		const float sc = 1.0f/1.0f;
		if (positions[i].direction == common::NORTH)
		{
			M = glm::rotate(M, 90.0f, vec3(0.0f,1.0f,0.0f));
		}
		else if (positions[i].direction == common::SOUTH)
		{
			M = glm::rotate(M, 90.0f, vec3(0.0f,-1.0f,0.0f));
		}
		else if (positions[i].direction == common::EAST)
		{
			M = glm::rotate(M, 180.0f, vec3(0.0f,1.0f,0.0f));
		}
		else if (positions[i].direction == common::WEST)
		{
			M = glm::rotate(M, 0.0f, vec3(0.0f,1.0f,0.0f));
		}

		M = glm::scale(M, vec3(0.8,0.8,0.5));

		glLoadMatrixf(glm::value_ptr(World::getV() * M));

		//TextureManager::setTexture(texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		//	  glEnableClientState(GL_VERTEX_ARRAY);
		//	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//	  glEnableClientState(GL_COLOR_ARRAY);
		//	  glEnableClientState( GL_NORMAL_ARRAY );


                glEnable(GL_LIGHT2);
                glm::vec3 pos = camera->position();
                GLfloat light2_position[] = { pos.x, pos.y, pos.z, 1};
                glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

                if (positions[i].player_no == 0)
		  model1->draw();
                else if (positions[i].player_no == 1)
		  model2->draw();
                else if (positions[i].player_no == 2)
		  model3->draw();



                glDisable(GL_LIGHT2);

		//	glDisableClientState( GL_NORMAL_ARRAY );
		//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//	glDisableClientState(GL_VERTEX_ARRAY);
		//	glDisableClientState(GL_COLOR_ARRAY);


		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		//    Box* box = new Box(glm::vec3(1, 1, 1));
		//    box->draw(M);
		//    delete box;


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

    //drawPlayerTrail(positions[number], list);
    //
    Trail trail(positions[number], list, number);
    trail.Draw();

    Shadow shadow(positions[number], list, number);
    shadow.Draw(0);
    shadow.Draw(1);
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
  printf("int: %d\n",interval);
}

void OpenGLMain::changeSize(int newWidth, int newHeight)
{
	if(newHeight == 0)
	{
		newHeight = 1;
	}

	curWidth = newWidth;
	curHeight = newHeight;

	float ratio = (float)curWidth/(float)curHeight;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, curWidth, curHeight);
	// Set the correct perspective.
	P = glm::perspective(50.0f,ratio,1.0f,160.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

int OpenGLMain::passedTime() {
  int actTime = glutGet(GLUT_ELAPSED_TIME);
  int interval = actTime - lastTime_;
  lastTime_ = actTime;
  return interval;
}


void OpenGLMain::loadTextures(){
  TextureManager::importTexture("p3trail", "textures/p1trail.png");
  TextureManager::importTexture("p1trail", "textures/p2trail.png");
  TextureManager::importTexture("p2trail", "textures/p3trail.png");
  TextureManager::importTexture("p3shadow", "textures/p1shadow.png");
  TextureManager::importTexture("p1shadow", "textures/p2shadow.png");
  TextureManager::importTexture("p2shadow", "textures/p3shadow.png");
  TextureManager::importTexture("floor", "textures/floor.png");
  TextureManager::importTexture("wall", "textures/wall.png");

  model1->loadGLTextures();
  model2->loadGLTextures();
  model3->loadGLTextures();
}

void OpenGLMain::Init(){
  int argc = 1;
  char* argv[] = { "./main" };
  glutInit(&argc, argv);

  model3 = new TexturedModel3D("models/light_cycle_1/", "HQ_Movie cycle.lwo");
  model1 = new TexturedModel3D("models/light_cycle_2/", "HQ_Movie cycle.lwo");
  model2 = new TexturedModel3D("models/light_cycle_3/", "HQ_Movie cycle.lwo");

  //model = new TexturedModel3D("models/Virgin/", "Car.lwo"); /* uncomment this to see FORMULA1 */
  //model = new TexturedModel3D("models/Girl/", "girl.lwo");
  //model = new TexturedModel3D("models/Churchill tank Mk IV (A22)/", "churchill.lwo");
  //model = new TexturedModel3D("models/Sideswipe/", "RB-SideSwipe.lwo");
  //model = new TexturedModel3D("models/Ratchet/", "RB-Ratchet.lwo");

  curWidth = defaultWidth;
  curHeight = defaultHeight;

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(curWidth,curHeight);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Awesome Game");

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

  glutDisplayFunc(displayFrame);
  glutIdleFunc(nextFrame);

  glutReshapeFunc(changeSize);

  glewInit();
  glutSpecialFunc(keyDown);

  glShadeModel(GL_SMOOTH);

  lighting->Init();

  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);

  // TODO remove TGA and create class managing all textures
  loadTextures();

  SharedMemory::getInstance().getPositions(positions);
  SharedMemory::getInstance().getTurns(turns);
  SharedMemory::getInstance().getCrashes(crashes);
  SharedMemory::getInstance().getPlayerNr(nr);
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
