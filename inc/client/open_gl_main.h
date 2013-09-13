#ifndef OPEN_GL_MAIN_H
#define OPEN_GL_MAIN_H

#include <client/shared_memory.h>

namespace client
{

class OpenGLMain
{
public:
	OpenGLMain(SharedMemory&);
	void start();
private:
	static void keyDown(int c, int x, int y);
	static void displayFrame();
	static void nextFrame();
        static void loadTextures();
        static int passedTime();
        static void Init();

	SharedMemory& sharedMemory;

	static std::vector<PLAYER_INFO> positions;
	static std::vector<TURN_INFO> turns;
	static std::vector<CRASH_INFO> crashes;

        static int lastTime_;
};

}

#endif // OPEN_GL_MAIN_H
