#include <client/open_gl_main.h>
#include <client/shared_memory.h>
#include <common/ithread.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <cstring>
#include <cstdio>

using namespace client;

OpenGLMain::OpenGLMain(SharedMemory& sharedMemory)
	: IThread(), sharedMemory(sharedMemory)
{
	int argc = 1;
	char* argv[] = { "main" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Awesome Game");        
	glutDisplayFunc(displayFrame);
//	glutIdleFunc(nextFrame);
	glewInit();
	glutSpecialFunc(keyDown);
//	glutSpecialUpFunc(keyUp);

	glEnable(GL_DEPTH_TEST);
}

void* OpenGLMain::start_routine()
{
	glutMainLoop();
}

void OpenGLMain::displayFrame()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}

void OpenGLMain::keyDown(int c, int x, int y)
{
	switch(c)
	{
	case GLUT_KEY_LEFT:
		printf("left\n");
		SharedMemory::getInstance().keyLeft();
		break;
	case GLUT_KEY_RIGHT:
		printf("right\n");
		SharedMemory::getInstance().keyRight();
		break;
	case GLUT_KEY_F1:
		printf("F1 [escape]\n");
		SharedMemory::getInstance().keyEsc();
		break;
	}
}
