#ifndef ITHREAD_H
#define ITHREAD_H

#include <pthread.h>

namespace common
{

class IThread
{
public:
	IThread();
	void run();
	void wait();
//	void kill();
protected:
	virtual void* start_routine() = 0;
private:
	pthread_t thread;
	static void* static_routine(void* arg);
};

}

#endif // ITHREAD_H
