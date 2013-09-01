#ifndef SERVER_UDP_LISTENER_H
#define SERVER_UDP_LISTENER_H

#include <common/ithread.h>
#include <server/server_udp.h>
#include <server/shared_memory.h>
#include <string>

namespace server
{

class ServerUDPListener
	: public common::IThread
{
public:
	ServerUDPListener(const std::string& port, SharedMemory& sharedMemory);
	void* start_routine();
private:
	ServerUDP server;
	SharedMemory& sharedMemory;
};

}

#endif // SERVER_UDP_LISTENER_H
