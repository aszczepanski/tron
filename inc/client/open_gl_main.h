#ifndef OPEN_GL_MAIN_H
#define OPEN_GL_MAIN_H

#include <common/ithread.h>
#include <client/shared_memory.h>

namespace client
{

class OpenGLMain
	: public common::IThread
{
public:
	OpenGLMain(SharedMemory&);	
private:
	void* start_routine();
	static void keyDown(int c, int x, int y);
	static void displayFrame();
	SharedMemory& sharedMemory;
	static SharedMemory* shm;
};

}

#endif // OPEN_GL_MAIN_H
