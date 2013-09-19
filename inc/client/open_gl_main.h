#ifndef OPEN_GL_MAIN_H
#define OPEN_GL_MAIN_H

#include <client/shared_memory.h>
#include <vector>

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
	static void changeSize(int w, int h);
        static void loadTextures();
        static int passedTime();
        static void Init();
        static void drawBikes();
        static void drawTrails();
        static void drawPlayerTrail(PLAYER_INFO position, std::vector<TURN_INFO>& list);
	static inline void drawEverything();

	SharedMemory& sharedMemory;

	static std::vector<PLAYER_INFO> positions;
	static std::vector<TURN_INFO> turns;
	static std::vector<CRASH_INFO> crashes;

        static int lastTime_;
};

}

#endif // OPEN_GL_MAIN_H
