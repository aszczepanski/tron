#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H

#include <server/shared_memory.h>
#include <common/ithread.h>

namespace server
{

class Application
	: public common::IThread
{
public:
private:
	void* start_routine();
	SharedMemory sharedMemory;
};

}

#endif // SERVER_APPLICATION_H
