#include <client/open_gl_main.h>
#include <client/shared_memory.h>
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

static float xAngle = 0.0f;
static float yAngle = 0.0f;

std::vector<PLAYER_INFO> OpenGLMain::positions;
std::vector<TURN_INFO> OpenGLMain::turns;
std::vector<CRASH_INFO> OpenGLMain::crashes;

GLuint tex;
TGAImg img;

OpenGLMain::OpenGLMain(SharedMemory& sharedMemory)
	: sharedMemory(sharedMemory)
{
}

void OpenGLMain::start()
{
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

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);


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
		//błąd
		std::cout << "error\n";
	}

	glutMainLoop();
}

void OpenGLMain::displayFrame()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V=glm::lookAt(
		glm::vec3(xAngle,yAngle,-45.0f),
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f));

	glm::mat4 P=glm::perspective(50.0f, 1.0f, 1.0f, 60.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(P));
	glMatrixMode(GL_MODELVIEW);

	for (int i=0; i<positions.size(); i++)
	{

		glm::mat4 M=glm::mat4(1.0f);
		M=glm::rotate(M,0.0f,glm::vec3(0.0f,1.0f,0.0f));
		M=glm::rotate(M,0.0f,glm::vec3(1.0f,0.0f,0.0f));

		//if ((positions[i].x != 0) || (positions[i].y != 0))
			//std::cout << "ogl1: " << positions[i].x << " " << positions[i].y << std::endl;
		M=glm::translate(M, glm::vec3((float)positions[i].x, (float)positions[i].y, 0.0f));

		glLoadMatrixf(glm::value_ptr(V*M));

//		glColor3d(1.0, 0.0, 1.0);
//		glutSolidCube(2.0);

		glColor3d(1.0, 1.0, 1.0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,cubeVertices);
		glColorPointer(3,GL_FLOAT,0,cubeColors);
		glDrawArrays(GL_QUADS,0,cubeVertexCount);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);	

	}

	for (std::vector<TURN_INFO>::iterator it = turns.begin(); it != turns.end(); it++)
	{
		if (it != turns.end()-1)
		{
			glm::mat4 M=glm::mat4(1.0f);
			M=glm::rotate(M,0.0f,glm::vec3(0.0f,1.0f,0.0f));
			M=glm::rotate(M,0.0f,glm::vec3(1.0f,0.0f,0.0f));

			M=glm::translate(M, glm::vec3(it->move.x, it->move.y, 0.0f));

			glLoadMatrixf(glm::value_ptr(V*M));

			glBindTexture(GL_TEXTURE_2D, tex);

			glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_T, GL_REPEAT);


			glEnable(GL_TEXTURE_2D);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glVertexPointer(3,GL_FLOAT,0,cubeVertices);
			glNormalPointer(GL_FLOAT, 0, cubeNormals);
			glTexCoordPointer(2,GL_FLOAT,0,cubeTexCoords);
			glDrawArrays(GL_QUADS,0,cubeVertexCount);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);	
			glDisableClientState(GL_NORMAL_ARRAY);

			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			glm::mat4 M=glm::mat4(1.0f);
			M=glm::rotate(M,0.0f,glm::vec3(0.0f,1.0f,0.0f));
			M=glm::rotate(M,0.0f,glm::vec3(1.0f,0.0f,0.0f));

			M=glm::translate(M, glm::vec3(it->move.x, it->move.y, 0.0f));

			glLoadMatrixf(glm::value_ptr(V*M));

			glColor3d(1.0, 1.0, 0.0);
			glutSolidCube(2.0);
/*

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(3,GL_FLOAT,0,cubeVertices);
			glColorPointer(3,GL_FLOAT,0,cubeColors);
			glDrawArrays(GL_QUADS,0,cubeVertexCount);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);	
*/
		}

	}

	glutSwapBuffers();
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

void OpenGLMain::nextFrame()
{
	SharedMemory::getInstance().getPositions(positions);
	SharedMemory::getInstance().getTurns(turns);
	SharedMemory::getInstance().getCrashes(crashes);

	glutPostRedisplay();
}
