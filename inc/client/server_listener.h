#ifndef SERVER_LISTENER_H
#define SERVER_LISTENER_H

#include <common/ithread.h>
#include <client/client_udp.h>
#include <string>
#include <client/shared_memory.h>

namespace client
{

class ServerListener
	: public common::IThread
{
public:
	ServerListener(const std::string& hostname, const std::string& port, SharedMemory&);
private:
	void* start_routine();
	ClientUDP client;
	SharedMemory& sharedMemory;
};

}

#endif // SERVER_LISTENER_H
